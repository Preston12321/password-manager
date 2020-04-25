#include "mainwindow.h"
#include "editdialog.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QtDebug>

MainWindow::MainWindow(DatabaseController *database, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), database(database) {
    ui->setupUi(this);

    icons = new QtAwesome(this);
    icons->initFontAwesome();

    audio = new AudioController(this);

    ui->actionAdd->setIcon(icons->icon(fa::plus));
    ui->actionEdit->setIcon(icons->icon(fa::edit));
    ui->actionDelete->setIcon(icons->icon(fa::trash));

    ui->tableWidget->horizontalHeader()->resizeSection(0, 200);
    ui->tableWidget->horizontalHeader()->resizeSection(1, 300);
    ui->tableWidget->insertColumn(3);
    ui->tableWidget->hideColumn(3);

    for (int i = 0; i < database->entryCount(); i++) {
        insertEntry(database->getEntry(i));
    }

    connect(database, &DatabaseController::entryInserted, this,
            &MainWindow::insertEntry);
    connect(database, &DatabaseController::entryDeleted, this,
            &MainWindow::deleteEntry);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionAdd_triggered() {
    auto entry = database->createEntry();

    EditDialog d(database, icons, audio, entry, this);
    auto result = d.exec();

    if (result == QDialog::Rejected) {
        database->deleteEntry(entry.id.toString());
    }
}

void MainWindow::on_actionDelete_triggered() {
    auto indexes = ui->tableWidget->selectionModel()->selectedRows();

    if (indexes.size() > 0) {
        auto answer = QMessageBox::warning(
            this, "Are you sure you want to delete?",
            "Items you delete cannot be recovered once deleted.",
            QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);

        if (answer != QMessageBox::Yes) return;
    }

    for (auto iter = indexes.rbegin(); iter != indexes.rend(); iter++) {
        auto index = *iter;
        auto item = ui->tableWidget->item(index.row(), 3);
        database->deleteEntry(item->text());
    }
}

void MainWindow::on_actionEdit_triggered() {
    auto selection = ui->tableWidget->selectionModel()->selection();

    if (selection.isEmpty()) return;

    int row = selection.first().top();

    auto item = ui->tableWidget->item(row, 3);

    EditDialog d(database, icons, audio, database->getEntry(item->text()),
                 this);
    d.exec();
}

void MainWindow::insertEntry(const PasswordEntry &entry) {
    auto table = ui->tableWidget;
    int index = table->rowCount();
    table->insertRow(index);
    table->setItem(index, 0, new QTableWidgetItem(entry.name));
    table->setItem(index, 1, new QTableWidgetItem(entry.url));
    table->setItem(index, 2,
                   new QTableWidgetItem(entry.lastUpdated.toString()));
    table->setItem(index, 3, new QTableWidgetItem(entry.id.toString()));

    ui->tableWidget->sortItems(0);
}

void MainWindow::deleteEntry(const PasswordEntry &entry) {
    auto matches = ui->tableWidget->findItems(
        entry.id.toString(), Qt::MatchFlag::MatchCaseSensitive);

    if (matches.size() == 0) {
        qDebug() << "Could not find entry in table:" << entry.id.toString();
        return;
    }

    ui->tableWidget->removeRow(matches.at(0)->row());
}
