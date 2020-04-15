#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <qaesencryption.h>
#include <QCryptographicHash>
#include <QFile>
#include <QObject>
#include <QString>

class DatabaseController : public QObject {
    Q_OBJECT

public:
    explicit DatabaseController(QObject *parent = nullptr);
    ~DatabaseController();

    bool isUnlocked();
    bool needsSetup();

public slots:
    void attemptUnlock(const QString &password);
    void attemptSetup(const QString &password);

signals:
    void unlockAttempted(const bool &success);
    void setupSucceeded();
    void setupFailed(const QString &message);

private:
    QByteArray hash(const QString &data);
    QByteArray generateIV();

    QFile *database;
    QJsonDocument *json;

    bool unlocked;
    bool setupNeeded;

    QCryptographicHash *hashFunction;
    QAESEncryption *encoder;

    QByteArray initialVector;
    QByteArray masterKey;

    const QString CHALLENGE = QString("Send cat pics!!!");
};

#endif  // DATABASECONTROLLER_H
