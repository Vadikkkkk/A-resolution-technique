#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add_literals.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    addLiterals = new Dialog_add_Literals;

    literals.append("a");
    literals.append("b");
    for (int i = 0; i < literals.size() ;i++) {
        ui->literals->addItem(literals[i]);
    }


    connect(ui->add_literal, &QPushButton::clicked, this, &MainWindow::showAddLiterals);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showAddLiterals()
{
    setEnabled(false);
    addLiterals->show();
}

