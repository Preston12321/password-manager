#include "databasecontroller.h"

#include <QCryptographicHash>
#include <QDir>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QtDebug>

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

    json = new QJsonDocument();

    hashFunction = new QCryptographicHash(QCryptographicHash::Sha256);
    encoder = new QAESEncryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    initialVector = QByteArray();
}

DatabaseController::~DatabaseController() {
    database->close();
    delete hashFunction;
    delete encoder;
}

bool DatabaseController::isUnlocked() { return unlocked; }

bool DatabaseController::needsSetup() { return setupNeeded; }

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
        // TODO: Decode remainder of database

        unlocked = true;
        emit unlockAttempted(true);
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
    }

    initialVector = generateIV();
    database->write(initialVector);
    database->write(
        encoder->encode(CHALLENGE.toUtf8(), masterKey, initialVector));
    database->flush();

    unlocked = true;
    emit setupSucceeded();
}

QByteArray DatabaseController::hash(const QString &data) {
    hashFunction->reset();
    hashFunction->addData(data.toUtf8());
    auto result = hashFunction->result();
    hashFunction->reset();
    return result;
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
