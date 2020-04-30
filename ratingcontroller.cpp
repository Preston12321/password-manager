#include "ratingcontroller.h"

#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QtMath>

RatingController::RatingController(QObject *parent) : QObject(parent) {
    knownPasswords = QList<QString>();

    QFile list("resources/wordlists/10k-most-common.txt");
    if (!list.exists() || !list.open(QFile::ReadOnly)) {
        // TODO: Fail gracefully upon failure to open/read wordlist
        qDebug() << "Couldn't open word list file";
    }

    QTextStream stream(list.readAll());

    list.close();

    QString line;
    while (stream.readLineInto(&line)) {
        insertKnownPassword(line);
    }
}

int RatingController::ratePassword(const QString &password) {
    if (password.size() == 0) return 0;

    if (passwordIsKnown(password)) return 1;

    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSymbol = false;

    int pool = 0;

    for (auto c : password) {
        switch (classifyCharacter(c)) {
            case CharacterClass::Uppercase:
                if (!hasUppercase) {
                    pool += uppercase;
                    hasUppercase = true;
                }
                break;
            case CharacterClass::Lowercase:
                if (!hasLowercase) {
                    pool += lowercase;
                    hasLowercase = true;
                }
                break;
            case CharacterClass::Digit:
                if (!hasDigit) {
                    pool += digit;
                    hasDigit = true;
                }
                break;
            case CharacterClass::Symbol:
                if (!hasSymbol) {
                    pool += symbol;
                    hasSymbol = true;
                }
                break;
        }
    }

    double entropy = log2(pow(pool, password.size()));

    if (entropy >= 128) return 5;
    if (entropy >= 60) return 4;
    if (entropy >= 36) return 3;
    if (entropy >= 28) return 2;
    return 1;
}

RatingController::CharacterClass RatingController::classifyCharacter(
    const QChar &c) {
    if (c.isUpper()) {
        return CharacterClass::Uppercase;
    }

    if (c.isLower()) {
        return CharacterClass::Lowercase;
    }

    if (c.isNumber()) {
        return CharacterClass::Digit;
    }

    return CharacterClass::Symbol;
}

void RatingController::insertKnownPassword(const QString &password) {
    int left = 0;
    int right = knownPasswords.size() - 1;
    int index = 0;
    while (left <= right) {
        index = (left + right) / 2;
        int compare = QString::compare(password, knownPasswords.at(index));

        if (compare == 0) {
            return;
        }

        if (compare < 0) {
            right = index - 1;
        }

        if (compare > 0) {
            left = ++index;
        }
    }

    knownPasswords.insert(index, password);
}

bool RatingController::passwordIsKnown(const QString &password) {
    // Doesn't make sense to check strings less than 3 characters long
    if (password.size() < 3) return false;

    QSet<QString> checked;
    for (int i = 0; i <= password.size() - 3; i++) {
        for (int j = password.size() - 1; j >= i + 2; j--) {
            QString substring = password.mid(i, j - i + 1);

            if (checked.contains(substring)) continue;

            int left = 0;
            int right = knownPasswords.size() - 1;
            while (left <= right) {
                int index = (left + right) / 2;
                int compare =
                    QString::compare(substring, knownPasswords.at(index));

                if (compare == 0) {
                    return true;
                }

                if (compare < 0) {
                    right = index - 1;
                }

                if (compare > 0) {
                    left = index + 1;
                }
            }

            checked.insert(substring);
        }
    }

    return false;
}
