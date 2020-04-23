#include "editdialog.h"
#include "passwordvalidator.h"
#include "strengthmeter.h"
#include "ui_editdialog.h"

#include <QAction>
#include <QToolButton>
#include <QtDebug>

EditDialog::EditDialog(DatabaseController *database, QtAwesome *iconLibrary,
                       PasswordEntry entry, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      database(database),
      icons(iconLibrary),
      entry(entry) {
    ui->setupUi(this);

    ui->passwordEdit->setValidator(new PasswordValidator(this));

    ui->nameEdit->setText(entry.name);
    ui->urlEdit->setText(entry.url);
    ui->passwordEdit->setText(database->decryptPassword(entry));

    auto action = ui->passwordEdit->addAction(icons->icon(fa::eyeslash),
                                              QLineEdit::TrailingPosition);
    showButton =
        qobject_cast<QToolButton *>(action->associatedWidgets().last());

    connect(showButton, &QToolButton::clicked, this,
            &EditDialog::togglePasswordVisible);
}

EditDialog::~EditDialog() { delete ui; }

void EditDialog::togglePasswordVisible() {
    if (ui->passwordEdit->echoMode() == QLineEdit::Password) {
        ui->passwordEdit->setEchoMode(QLineEdit::Normal);
        showButton->setIcon(icons->icon(fa::eye));
    } else {
        ui->passwordEdit->setEchoMode(QLineEdit::Password);
        showButton->setIcon(icons->icon(fa::eyeslash));
    }
}

void EditDialog::on_buttonBox_accepted() {
    if (!ui->passwordEdit->hasAcceptableInput()) return;

    entry.name = ui->nameEdit->text();
    entry.url = ui->urlEdit->text();

    database->updateEntry(entry, ui->passwordEdit->text());

    accept();
}

void EditDialog::on_buttonBox_rejected() {
    database->deleteEntry(entry.id.toString());

    reject();
}

void EditDialog::on_passwordEdit_textChanged(const QString &password) {
    int rating = StrengthMeter::ratePassword(password);
    qDebug() << "Password rating:" << rating;
    // TODO: Play sound to signal password rating
}
