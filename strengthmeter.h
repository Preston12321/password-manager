#ifndef STRENGTHMETER_H
#define STRENGTHMETER_H

#include <QString>

namespace StrengthMeter {

int ratePassword(const QString &password);

enum CharacterClass { Uppercase, Lowercase, Digit, Symbol };

namespace ClassSizes {

const int uppercase = 26;
const int lowercase = 26;
const int digit = 10;
const int symbol = 33;

}  // namespace ClassSizes

CharacterClass classifyCharacter(const QChar &c);

}  // namespace StrengthMeter

#endif  // STRENGTHMETER_H
