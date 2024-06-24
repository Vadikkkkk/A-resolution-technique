#include "dialog_add_literals.h"
#include "ui_dialog_add_literals.h"

Dialog_add_Literals::Dialog_add_Literals(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_Literals)
{
    ui->setupUi(this);
}

Dialog_add_Literals::~Dialog_add_Literals()
{
    delete ui;
}
