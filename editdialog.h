#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "audiocontroller.h"
#include "databasecontroller.h"
#include "ratingcontroller.h"

#include <QtAwesome.h>
#include <QDialog>
#include <QToolButton>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditDialog(DatabaseController *database, QtAwesome *iconLibrary,
                        AudioController *audio, RatingController *ratings,
                        PasswordEntry entry, QWidget *parent = nullptr);
    ~EditDialog();

public slots:
    void togglePasswordVisible();

private slots:
    void on_buttonBox_accepted();

    void on_passwordEdit_textChanged(const QString &password);

    void on_EditDialog_finished(int result);

private:
    Ui::EditDialog *ui;
    DatabaseController *database;
    QtAwesome *icons;
    AudioController *audio;
    RatingController *ratings;
    QToolButton *showButton;

    PasswordEntry entry;
};

#endif  // EDITDIALOG_H
