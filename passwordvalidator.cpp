#include "passwordvalidator.h"

PasswordValidator::PasswordValidator(QObject *parent) : QValidator(parent) {
    // TODO: Implement constructor
}

QValidator::State PasswordValidator::validate(QString &input, int &pos) const {
    Q_UNUSED(pos);

    auto utf8 = input.toUtf8();

    for (auto c : utf8) {
        // Reject non-ASCII characters
        if (c & 0x80) {
            return QValidator::Invalid;
        }

        unsigned char cu = c;

        // Reject non-space whitespace and non-printing characters
        if (cu <= 31U || cu == 127U) {
            return QValidator::Invalid;
        }
    }

    return QValidator::Acceptable;
}
