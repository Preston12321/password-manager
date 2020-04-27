#include "databasecontroller.h"

#include <QCborArray>
#include <QCborMap>
#include <QCborValue>
#include <QCryptographicHash>
#include <QDir>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QStandardPaths>

DatabaseController::DatabaseController(QObject *parent) : QObject(parent) {
    auto path =
        QStandardPaths::locate(QStandardPaths::AppDataLocation, "pdatabase");

    if (path.isEmpty()) {
        setupNeeded = true;
    } else {
        database = new QFile(path, this);

        bool opened = database->open(QFile::OpenModeFlag::ReadWrite);

        if (!opened) {
            // TODO: Fail gracefully when unable to open database file
        }

        setupNeeded = database->size() < 32;
    }

    unlocked = false;

    entries = QList<PasswordEntry *>();

    encoder = new QAESEncryption(QAESEncryption::AES_256, QAESEncryption::CBC);
}

DatabaseController::~DatabaseController() {
    database->close();
    delete encoder;
    for (auto entry : entries) {
        delete entry;
    }
}

bool DatabaseController::isUnlocked() const { return unlocked; }

bool DatabaseController::needsSetup() const { return setupNeeded; }

PasswordEntry DatabaseController::createEntry() {
    auto entry = new PasswordEntry();
    entry->id = QUuid::createUuid();
    entry->name = "";
    entry->url = "";
    auto now = QDateTime::currentDateTime();
    entry->created = now;
    entry->lastUpdated = now;
    entry->encodedPassword = QByteArray();

    while (!insertEntry(entry)) {
        entry->id = QUuid::createUuid();
    }

    return *entry;
}

void DatabaseController::updateEntry(const PasswordEntry &entry,
                                     const QString &password) {
    int index = indexOf(entry.id.toString());

    if (index == -1) return;

    auto target = entries.takeAt(index);
    emit entryDeleted(entry);

    target->lastUpdated = QDateTime::currentDateTime();
    target->name = entry.name;
    target->url = entry.url;
    target->initialVector = generateIV();
    target->encodedPassword =
        encoder->encode(password.toUtf8(), masterKey, target->initialVector);

    insertEntry(target);
}

void DatabaseController::deleteEntry(const QString &id) {
    deleteEntry(indexOf(id));
}

void DatabaseController::deleteEntry(const int &index) {
    if (index < 0 || index >= entries.size()) return;

    auto removed = entries.takeAt(index);
    PasswordEntry entry(*removed);
    delete removed;

    emit entryDeleted(entry);
}

QString DatabaseController::decryptPassword(const PasswordEntry &entry) {
    auto temp =
        encoder->decode(entry.encodedPassword, masterKey, entry.initialVector);
    return QString(encoder->removePadding(temp));
}

int DatabaseController::entryCount() const { return entries.size(); }

PasswordEntry DatabaseController::getEntry(const int &index) {
    return *(entries.at(index));
}

PasswordEntry DatabaseController::getEntry(const QString &id) {
    int index = indexOf(id);

    if (index == -1) {
        return PasswordEntry();
    }

    return *(entries.at(index));
}

void DatabaseController::attemptUnlock(const QString &password) {
    if (unlocked) {
        emit unlockAttempted(true);
        return;
    }

    if (setupNeeded) {
        emit unlockAttempted(false);
        return;
    }

    auto hashedPassword = hash(password);

    database->seek(0);
    auto iv = database->read(16);
    auto encodedChallenge = database->read(16);

    auto decodedChallenge =
        encoder->decode(encodedChallenge, hashedPassword, iv);

    if (QString(decodedChallenge) == CHALLENGE) {
        masterKey = hashedPassword;

        iv = database->read(16);
        auto raw = encoder->removePadding(
            encoder->decode(database->readAll(), hashedPassword, iv));

        bool result = bytesToEntries(raw);

        // TODO: Fail gracefully when unable to parse database file

        unlocked = result;
        emit unlockAttempted(result);
    } else {
        emit unlockAttempted(false);
    }
}

void DatabaseController::attemptSetup(const QString &password) {
    auto dir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (dir.isEmpty()) {
        emit setupFailed("No writable location available for database file.");
        return;
    }

    masterKey = hash(password);

    QDir d;
    if (!d.exists(dir)) {
        d.mkpath(dir);
    }
    QDir::setCurrent(dir);
    database = new QFile("pdatabase", this);
    auto opened = database->open(QFile::OpenModeFlag::ReadWrite);

    if (!opened) {
        emit setupFailed("Could not open database file.");
        return;
    }

    attemptSave();

    unlocked = true;
    emit setupSucceeded();
}

void DatabaseController::attemptSave() {
    database->seek(0);
    database->resize(0);

    auto iv = generateIV();
    database->write(iv);
    database->write(encoder->encode(CHALLENGE.toUtf8(), masterKey, iv));

    iv = generateIV();
    database->write(iv);
    database->write(encoder->encode(entriesToBytes(), masterKey, iv));
}

QByteArray DatabaseController::hash(const QString &data) {
    return QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256);
}

QByteArray DatabaseController::generateIV() {
    QVector<quint32> vector;
    vector.resize(4);
    QRandomGenerator random;
    random.fillRange(vector.data(), vector.size());

    QByteArray result;
    for (auto num : vector) {
        for (int i = 0; i <= 24; i += 8) {
            quint8 byte = num >> i;
            result.append(byte);
        }
    }

    return result;
}

QByteArray DatabaseController::entriesToBytes() const {
    QCborArray list;

    for (auto entry : entries) {
        QCborMap m{
            {CborKeys::EntryId, QCborValue(entry->id)},
            {CborKeys::EntryName, QCborValue(entry->name)},
            {CborKeys::EntryUrl, QCborValue(entry->url)},
            {CborKeys::EntryCreated, QCborValue(entry->created)},
            {CborKeys::EntryUpdated, QCborValue(entry->lastUpdated)},
            {CborKeys::EntryPassword, QCborValue(entry->encodedPassword)},
            {CborKeys::EntryInitialVector, QCborValue(entry->initialVector)},
        };

        list.append(m.toCborValue());
    }

    QCborMap map{
        {CborKeys::Version, QString("1.0")},
        {CborKeys::EntriesList, list.toCborValue()},
    };

    return map.toCborValue().toCbor();
}

bool DatabaseController::bytesToEntries(const QByteArray &bytes) {
    auto cbor = QCborValue::fromCbor(bytes);

    if (cbor.type() != QCborValue::Map) {
        return false;
    }

    auto map = cbor.toMap();

    if (!map.contains(CborKeys::EntriesList)) {
        return false;
    }

    auto list = map.value(CborKeys::EntriesList);

    if (list.type() != QCborValue::Array) {
        return false;
    }

    for (auto item : list.toArray()) {
        if (item.type() != QCborValue::Map) continue;

        auto data = item.toMap();

        if (!data.contains(CborKeys::EntryId)) continue;

        auto entry = new PasswordEntry();
        entry->id = data.value(CborKeys::EntryId).toUuid();
        entry->name = data.value(CborKeys::EntryName).toString();
        entry->url = data.value(CborKeys::EntryUrl).toString();
        entry->created = data.value(CborKeys::EntryCreated).toDateTime();
        entry->lastUpdated = data.value(CborKeys::EntryUpdated).toDateTime();
        entry->initialVector =
            data.value(CborKeys::EntryInitialVector).toByteArray();
        entry->encodedPassword =
            data.value(CborKeys::EntryPassword).toByteArray();

        insertEntry(entry);
    }

    return true;
}

int DatabaseController::indexOf(const QString &id) const {
    int left = 0;
    int right = entries.size() - 1;
    while (left <= right) {
        int index = (left + right) / 2;
        int compare = QString::compare(id, entries.at(index)->id.toString());

        if (compare == 0) {
            return index;
        }

        if (compare < 0) {
            right = index - 1;
        }

        if (compare > 0) {
            left = index + 1;
        }
    }

    return -1;
}

bool DatabaseController::insertEntry(PasswordEntry *entry) {
    int left = 0;
    int right = entries.size() - 1;
    int index = 0;
    while (left <= right) {
        index = (left + right) / 2;
        int compare = QString::compare(entry->id.toString(),
                                       entries.at(index)->id.toString());

        if (compare == 0) {
            return false;
        }

        if (compare < 0) {
            right = index - 1;
        }

        if (compare > 0) {
            left = ++index;
        }
    }

    entries.insert(index, entry);
    emit entryInserted(*entry);
    return true;
}
