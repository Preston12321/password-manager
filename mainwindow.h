#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtAwesome.h>
#include <QMainWindow>

#include "passwordtablemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // TODO: Add slots to detect New, Delete, and Edit actions

private:
    Ui::MainWindow *ui;
    PasswordTableModel *model;
    QtAwesome *icons;
};
#endif  // MAINWINDOW_H
