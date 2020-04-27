#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include "passwordentry.h"

#include <qaesencryption.h>
#include <QCryptographicHash>
#include <QFile>
#include <QObject>
#include <QString>

enum CborKeys {
    Version,
    EntriesList,
    EntryName,
    EntryPassword,
    EntryId,
    EntryInitialVector,
    EntryUrl,
    EntryCreated,
    EntryUpdated
};

class DatabaseController : public QObject {
    Q_OBJECT

public:
    explicit DatabaseController(QObject *parent = nullptr);
    ~DatabaseController();

    bool isUnlocked() const;
    bool needsSetup() const;

    PasswordEntry createEntry();
    PasswordEntry getEntry(const int &index);
    PasswordEntry getEntry(const QString &id);
    void updateEntry(const PasswordEntry &entry, const QString &password);
    void deleteEntry(const QString &id);
    void deleteEntry(const int &index);

    QString decryptPassword(const PasswordEntry &entry);

    int entryCount() const;

public slots:
    void attemptUnlock(const QString &password);
    void attemptSetup(const QString &password);
    void attemptSave();

signals:
    void unlockAttempted(const bool &success);
    void setupSucceeded();
    void setupFailed(const QString &message);
    void entriesUpdated();
    void entryDeleted(const PasswordEntry &entry);
    void entryInserted(const PasswordEntry &entry);

private:
    static QByteArray hash(const QString &data);
    static QByteArray generateIV();

    QByteArray entriesToBytes() const;
    bool bytesToEntries(const QByteArray &bytes);

    int indexOf(const QString &id) const;
    bool insertEntry(PasswordEntry *entry);

    QFile *database;
    QList<PasswordEntry *> entries;

    bool unlocked;
    bool setupNeeded;

    QAESEncryption *encoder;

    QByteArray masterKey;

    const QString CHALLENGE = QString("Send cat pics!!!");
};

#endif  // DATABASECONTROLLER_H
