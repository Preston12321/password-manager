#include "strengthmeter.h"

#include <QtMath>

int StrengthMeter::ratePassword(const QString &password) {
    if (password.size() == 0) return 0;

    bool upper = false;
    bool lower = false;
    bool digit = false;
    bool symbol = false;

    int pool = 0;

    for (auto c : password) {
        switch (classifyCharacter(c)) {
            case CharacterClass::Uppercase:
                if (!upper) {
                    pool += ClassSizes::uppercase;
                    upper = true;
                }
                break;
            case CharacterClass::Lowercase:
                if (!lower) {
                    pool += ClassSizes::lowercase;
                    lower = true;
                }
                break;
            case CharacterClass::Digit:
                if (!digit) {
                    pool += ClassSizes::digit;
                    digit = true;
                }
                break;
            case CharacterClass::Symbol:
                if (!symbol) {
                    pool += ClassSizes::symbol;
                    symbol = true;
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

StrengthMeter::CharacterClass StrengthMeter::classifyCharacter(const QChar &c) {
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
