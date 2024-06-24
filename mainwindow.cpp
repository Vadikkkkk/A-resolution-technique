#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), count(0)
{


    ui->setupUi(this);


    addFormula = new formula;



    connect(ui->add_literal, &QPushButton::clicked, this, &MainWindow::addLiteral);
    connect(ui->add_formula, &QPushButton::clicked, this, &MainWindow::showAddFormula);

    connect(ui->delete_literal, &QPushButton::clicked, this, &MainWindow::deleteLiteral);



    connect(addFormula, &formula::closeSignal, this, &MainWindow::closeAddFormula);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateList()
{
    ui->literals->clear();
    for (const QString& str : literals) {
        ui->literals->addItem(str);
    }
}

void MainWindow::addLiteral()
{
    if(count < 26){
        QChar letter = 'A';
        literals.append(QString(letter.unicode() + count));
        count++;
        updateList();
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Слишком много литералов!");
    }
}



void MainWindow::deleteLiteral()
{
    if(count > 0){
        literals.pop_back();
        count--;
        updateList();
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Литералов нет!");
    }
}

void MainWindow::showAddFormula()
{
    setEnabled(false);
    addFormula->l = literals;
    addFormula->updateButtons();
    addFormula->show();
}

void MainWindow::closeAddFormula()
{
    setEnabled(true);
    addFormula->reject();
}




