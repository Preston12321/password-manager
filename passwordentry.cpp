#include "passwordentry.h"

PasswordEntry::PasswordEntry() {
    // MAYBE: Implement constructor
}

QString PasswordEntry::getEncodedText() const { return encodedText; }

void PasswordEntry::setEncodedText(const QString &value) {
    encodedText = value;
    refreshLastUpdated();
}

QDateTime PasswordEntry::getLastUpdated() const { return lastUpdated; }

void PasswordEntry::setLastUpdated(const QDateTime &value) {
    lastUpdated = value;
}

QString PasswordEntry::getNameOrURL() const { return nameOrURL; }

void PasswordEntry::setNameOrURL(const QString &value) {
    nameOrURL = value;
    refreshLastUpdated();
}

void PasswordEntry::refreshLastUpdated() {
    auto now = QDateTime::currentDateTime();
    lastUpdated = now;
}
