#include "setupdialog.h"
#include "ui_setupdialog.h"

#include <QDialogButtonBox>

SetupDialog::SetupDialog(DatabaseController *database, QWidget *parent)
    : QDialog(parent), ui(new Ui::SetupDialog), database(database) {
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this,
            &SetupDialog::attemptSetup);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
            &SetupDialog::attemptSetup);
    connect(database, &DatabaseController::setupSucceeded, this,
            &SetupDialog::setupSucceeded);
    connect(database, &DatabaseController::setupFailed, this,
            &SetupDialog::setupFailed);
}

SetupDialog::~SetupDialog() { delete ui; }

void SetupDialog::attemptSetup() {
    auto password = ui->lineEdit->text();
    database->attemptSetup(password);
}

void SetupDialog::setupSucceeded() { accept(); }

void SetupDialog::setupFailed(const QString &message) {
    Q_UNUSED(message);
    reject();
}
