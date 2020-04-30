#ifndef RATINGCONTROLLER_H
#define RATINGCONTROLLER_H

#include <QObject>
#include <QSet>

class RatingController : public QObject {
    Q_OBJECT
public:
    explicit RatingController(QObject *parent = nullptr);

    int ratePassword(const QString &password);
    bool passwordIsKnown(const QString &password);

    enum CharacterClass { Uppercase, Lowercase, Digit, Symbol };

    CharacterClass classifyCharacter(const QChar &c);

    static const int uppercase = 26;
    static const int lowercase = 26;
    static const int digit = 10;
    static const int symbol = 33;

private:
    QList<QString> knownPasswords;

    void insertKnownPassword(const QString &password);
};

#endif  // RATINGCONTROLLER_H
