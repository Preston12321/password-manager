#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QDateTime>
#include <QString>
#include <QUuid>

struct PasswordEntry {
    QUuid id;
    QByteArray encodedPassword;
    QByteArray initialVector;
    QString name;
    QString url;
    QDateTime created;
    QDateTime lastUpdated;
};

#endif  // PASSWORDENTRY_H
