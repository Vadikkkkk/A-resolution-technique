#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), count(0)
{
    ui->setupUi(this);

    addFormula = new formula;
    res = new resolution;

    connectAllButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete addFormula;
    delete res;
}

void MainWindow::updateList()
{
    ui->literals->clear();
    for (const QString& str : literals) {
        ui->literals->addItem(str);
    }
}

void MainWindow::updateFormulas()
{
    ui->formulas->clear();
    for (const QString& str : formulas) {
        ui->formulas->addItem(str);
    }
}

void MainWindow::updateTheorem()
{
    ui->theorem->clear();
    ui->theorem->addItem(this->theorem);
}

void MainWindow::connectAllButtons()
{
    connect(ui->add_literal, &QPushButton::clicked, this, &MainWindow::addLiteral);
    connect(ui->add_formula, &QPushButton::clicked, this, &MainWindow::showAddFormula);
    connect(ui->addTheorem,&QPushButton::clicked, this, &MainWindow::showAddFormula);

    connect(ui->delete_literal, &QPushButton::clicked, this, &MainWindow::deleteLiteral);
    connect(ui->delete_formula, &QPushButton::clicked, this, &MainWindow::deleteFormula);
    connect(ui->deleteTheorem, &QPushButton::clicked, this, &MainWindow::deleteTheorem);
    connect(ui->resolution, &QPushButton::clicked, this, &MainWindow::showResolution);

    connect(addFormula, &formula::closeSignal, this, &MainWindow::closeAddFormula);
    connect(addFormula, &formula::saveSignal, this, &MainWindow::saveFormula);

    connect(res, &resolution::closeSignal, this, &MainWindow::closeResolution);
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
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button == ui->addTheorem && theorem.isEmpty()) {
        lastButton = ui->addTheorem->text();
        setEnabled(false);
        addFormula->setDNFText();
        addFormula->clearLine();
        addFormula->braketsCount = 0;
        addFormula->l = literals;
        addFormula->updateButtons();
        addFormula->show();
    }
    else if (button == ui->addTheorem && !theorem.isEmpty()){
        QMessageBox::information(this, "Ошибка!", "Гипотеза уже введена!");
    }
    else if (button == ui->add_formula) {
        lastButton = ui->add_formula->text();
        setEnabled(false);
        addFormula->setCNFText();
        addFormula->clearLine();
        addFormula->braketsCount = 0;
        addFormula->l = literals;
        addFormula->updateButtons();
        addFormula->show();
    }

}

void MainWindow::closeAddFormula()
{
    setEnabled(true);
    addFormula->reject();
}

void MainWindow::saveFormula()
{
    if (lastButton == ui->addTheorem->text()){
        if (!addFormula->getFormula().isEmpty()){
            theorem = addFormula->getFormula();
            updateTheorem();
            setEnabled(true);
            addFormula->reject();
        }
        else{
            QMessageBox::information(this, "Ошибка!", "Формула пуста!");
        }
    }
    else if(lastButton == ui->add_formula->text()){
        if (!addFormula->getFormula().isEmpty() && !formulas.contains(addFormula->getFormula())){
            formulas.append(addFormula->getFormula());
            updateFormulas();
            setEnabled(true);
            addFormula->reject();
        }
        else if(formulas.contains(addFormula->getFormula())){
            QMessageBox::information(this, "Ошибка!", "Такая формула уже есть!");
            showAddFormula();
        }
        else{
            QMessageBox::information(this, "Ошибка!", "Формула пуста!");
            setEnabled(true);
            addFormula->reject();
        }
    }
}

void MainWindow::deleteFormula()
{
    if(formulas.size() > 0){
        formulas.pop_back();
        updateFormulas();
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Формул нет!");
    }
}

void MainWindow::deleteTheorem()
{
    theorem.clear();
    ui->theorem->clear();
}

void MainWindow::showResolution()
{
    if (ui->theorem->count()){
        setEnabled(false);
        res->literals = this->literals;
        res->formulas = this->formulas;
        res->theorem = this->theorem;
        res->S.clear();
        res->update();
        res->show();
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Введите гипотезу!");
    }
}

void MainWindow::closeResolution()
{
    setEnabled(true);
    res->reject();
}
