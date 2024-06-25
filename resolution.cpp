#include "resolution.h"
#include "ui_resolution.h"

resolution::resolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resolution)
{
    ui->setupUi(this);
}

resolution::~resolution()
{
    delete ui;
}
