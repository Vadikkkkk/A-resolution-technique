#include "formula.h"
#include "ui_formula.h"

formula::formula(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formula)
{
    ui->setupUi(this);
}

formula::~formula()
{
    delete ui;
}
