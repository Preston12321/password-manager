#ifndef PASSWORDVALIDATOR_H
#define PASSWORDVALIDATOR_H

#include <QValidator>

class PasswordValidator : public QValidator {
    Q_OBJECT
public:
    explicit PasswordValidator(QObject* parent = nullptr);

    virtual State validate(QString& input, int& pos) const override;
};

#endif  // PASSWORDVALIDATOR_H
