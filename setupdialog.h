#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include "databasecontroller.h"

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog {
    Q_OBJECT

public:
    explicit SetupDialog(DatabaseController *database,
                         QWidget *parent = nullptr);
    ~SetupDialog();

private slots:
    void attemptSetup();
    void setupSucceeded();
    void setupFailed(const QString &message);

private:
    Ui::SetupDialog *ui;
    DatabaseController *database;
};

#endif  // SETUPDIALOG_H
