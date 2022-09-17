#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "DB.hpp"
#include "ui_mainwindow.h"

Database g_db;  // NOLINT

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_ABOUT_triggered() {
    QMessageBox::about(this, "About",
                       "Курсовая работа по ООП.\n(c) Крылов Артём, ИКПИ-14");
}

void MainWindow::on_opendb_triggered() {
    QString str{QFileDialog::getOpenFileName(this)};

    if (str.isEmpty()) return;

    if (!g_db.isEmpty()) g_db.clear();
    g_db.read(str.toStdString());
}

void MainWindow::on_savedb_triggered() {
    QString str{QFileDialog::getSaveFileName(this)};

    g_db.save(str.toStdString());
}
