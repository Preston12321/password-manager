#ifndef PASSWORDENTRY_H
#define PASSWORDENTRY_H

#include <QDateTime>

class PasswordEntry {
public:
    PasswordEntry();

    QString getEncodedText() const;
    void setEncodedText(const QString &value);

    QDateTime getLastUpdated() const;
    void setLastUpdated(const QDateTime &value);
    void refreshLastUpdated();

    QString getNameOrURL() const;
    void setNameOrURL(const QString &value);

private:
    QString encodedText;
    QString nameOrURL;
    QDateTime lastUpdated;
};

#endif  // PASSWORDENTRY_H
