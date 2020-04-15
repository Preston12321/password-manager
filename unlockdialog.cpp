#include "unlockdialog.h"
#include "databasecontroller.h"
#include "ui_unlockdialog.h"

#include <QCryptographicHash>

UnlockDialog::UnlockDialog(DatabaseController *database, QWidget *parent)
    : QDialog(parent), ui(new Ui::UnlockDialog), database(database) {
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this,
            &UnlockDialog::attemptUnlock);
    connect(ui->pushButton, &QPushButton::pressed, this,
            &UnlockDialog::attemptUnlock);
    connect(database, &DatabaseController::unlockAttempted, this,
            &UnlockDialog::unlockAttempted);
}

UnlockDialog::~UnlockDialog() { delete ui; }

void UnlockDialog::attemptUnlock() {
    QString password = ui->lineEdit->text();

    database->attemptUnlock(password);
}

void UnlockDialog::unlockAttempted(const bool &success) {
    if (success) {
        accept();
        return;
    }
    // MAYBE: show some kind of feedback when password is incorrect
}
