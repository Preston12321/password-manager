#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    icons = new QtAwesome(this);
    icons->initFontAwesome();

    model = new PasswordTableModel();
    ui->tableView->setModel(model);

    ui->actionAdd->setIcon(icons->icon(fa::plus));
    ui->actionEdit->setIcon(icons->icon(fa::edit));
    ui->actionDelete->setIcon(icons->icon(fa::trash));
}

MainWindow::~MainWindow() { delete ui; }
