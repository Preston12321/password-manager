#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "databasecontroller.h"

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
                        PasswordEntry entry, QWidget *parent = nullptr);
    ~EditDialog();

public slots:
    void togglePasswordVisible();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_passwordEdit_textChanged(const QString &password);

private:
    Ui::EditDialog *ui;
    DatabaseController *database;
    QtAwesome *icons;
    QToolButton *showButton;

    PasswordEntry entry;
};

#endif  // EDITDIALOG_H
