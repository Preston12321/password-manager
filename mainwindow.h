#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtAwesome.h>
#include <QCloseEvent>
#include <QMainWindow>

#include "audiocontroller.h"
#include "databasecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(DatabaseController *database, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionAdd_triggered();

    void on_actionDelete_triggered();

    void on_actionEdit_triggered();

    void insertEntry(const PasswordEntry &entry);
    void deleteEntry(const PasswordEntry &entry);

private:
    Ui::MainWindow *ui;
    DatabaseController *database;
    QtAwesome *icons;
    AudioController *audio;
};
#endif  // MAINWINDOW_H
