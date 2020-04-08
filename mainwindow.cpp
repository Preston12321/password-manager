#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Disable the default status bar
    ui->statusbar->hide();
    //    ui->tableView->
}

MainWindow::~MainWindow() { delete ui; }
