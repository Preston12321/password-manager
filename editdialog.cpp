#include "editdialog.h"
#include "passwordvalidator.h"
#include "strengthmeter.h"
#include "ui_editdialog.h"

#include <QAction>
#include <QToolButton>

EditDialog::EditDialog(DatabaseController *database, QtAwesome *iconLibrary,
                       AudioController *audio, PasswordEntry entry,
                       QWidget *parent)
    : QDialog(parent),
      ui(new Ui::EditDialog),
      database(database),
      icons(iconLibrary),
      audio(audio),
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

    audio->stopPlayback();

    entry.name = ui->nameEdit->text();
    entry.url = ui->urlEdit->text();

    database->updateEntry(entry, ui->passwordEdit->text());

    accept();
}

void EditDialog::on_passwordEdit_textChanged(const QString &password) {
    if (password.size() == 0) {
        if (audio->isPlaying()) {
            audio->stopPlayback();
        }
        return;
    }

    int rating = StrengthMeter::ratePassword(password);

    audio->changePlaylist(rating - 1);

    if (!audio->isPlaying()) {
        audio->beginPlayback();
    }
}

void EditDialog::on_EditDialog_finished(int result) {
    Q_UNUSED(result);

    audio->stopPlayback();
}
