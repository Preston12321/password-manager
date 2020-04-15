#ifndef UNLOCKDIALOG_H
#define UNLOCKDIALOG_H

#include "databasecontroller.h"

#include <QDialog>

namespace Ui {
class UnlockDialog;
}

class UnlockDialog : public QDialog {
    Q_OBJECT

public:
    explicit UnlockDialog(DatabaseController *database,
                          QWidget *parent = nullptr);
    ~UnlockDialog();

private slots:
    void attemptUnlock();
    void unlockAttempted(const bool &success);

private:
    Ui::UnlockDialog *ui;
    DatabaseController *database;
};

#endif  // UNLOCKDIALOG_H
