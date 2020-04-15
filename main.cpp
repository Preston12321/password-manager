#include "mainwindow.h"
#include "setupdialog.h"
#include "unlockdialog.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DatabaseController database;

    if (database.needsSetup()) {
        SetupDialog setup(&database);
        auto result = setup.exec();

        if (result == QDialog::Rejected) {
            exit(2);
        }
    }

    if (!database.isUnlocked()) {
        UnlockDialog unlock(&database);

        auto result = unlock.exec();

        if (result == QDialog::Rejected) {
            exit(1);
        }
    }

    MainWindow w;
    w.show();
    return a.exec();
}
