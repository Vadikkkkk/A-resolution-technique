#include "dialog_add_literals.h"
#include "ui_dialog_add_literals.h"

Dialog_add_Literals::Dialog_add_Literals(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_Literals)
{
    ui->setupUi(this);
    ui->edit_literal->setMaxLength(1);
    QRegularExpression regex("[a-zA-Z]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, ui->edit_literal);
    ui->edit_literal->setValidator(validator);


    connect(ui->cancel_button, &QPushButton::clicked, this, &Dialog_add_Literals::close);
    connect(ui->save_button, &QPushButton::clicked, this, &Dialog_add_Literals::save);
}

Dialog_add_Literals::~Dialog_add_Literals()
{
    delete ui;
}

void Dialog_add_Literals::clearLine()
{
    ui->edit_literal->clear();
}

QString Dialog_add_Literals::getLiteral()
{
    return ui->edit_literal->text();
}

void Dialog_add_Literals::close()
{
    emit closeSignal();
}

void Dialog_add_Literals::save()
{
    if(ui->edit_literal->text().size() == 1){
        emit saveSignal();
    }
    else{
        emit closeSignal();
        QMessageBox::information(this, "Message", "Введите переменную", QMessageBox::Ok);
    }
}

void Dialog_add_Literals::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}
