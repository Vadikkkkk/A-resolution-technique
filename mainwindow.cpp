#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add_literals.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    addLiterals = new Dialog_add_Literals;



    connect(ui->add_literal, &QPushButton::clicked, this, &MainWindow::showAddLiterals);




    connect(addLiterals, &Dialog_add_Literals::closeSignal, this, &MainWindow::closeAddLiterals);
    connect(addLiterals, &Dialog_add_Literals::saveSignal, this, &MainWindow::saveNewLiteral);



}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
const QSet<QString> &MainWindow::getLiterals() const
{
    return literals;
}
*/

void MainWindow::showAddLiterals()
{
    setEnabled(false);
    addLiterals->clearLine();
    addLiterals->show();
}

void MainWindow::closeAddLiterals()
{
    setEnabled(true);
    addLiterals->reject();
}

void MainWindow::saveNewLiteral()
{
    setEnabled(true);
    ui->literals->clear();
    if (addLiterals->getLiteral().size() == 1){
        literals.insert(addLiterals->getLiteral());
        addLiterals->reject();
        for (const QString &str : literals) {
            ui->literals->addItem(str);
        }
    }
    else{
        addLiterals->reject();
    }
}




