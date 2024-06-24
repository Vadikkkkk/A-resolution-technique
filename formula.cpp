#include "formula.h"
#include "ui_formula.h"

formula::formula(QWidget *parent) :
    QDialog(parent), row(0), column(0),
    ui(new Ui::formula)
{
    ui->setupUi(this);

    connect(ui->close_button, &QPushButton::clicked, this, &formula::close);
    buttons << "!" << "+" << "*" << "->" << "(" << ")";
    addButtonsToLayout(buttons);

}


void formula::addButtonsToLayout(const QVector<QString> &vector)
{
    for (const QString &buttonText : vector) {
        QPushButton *button = new QPushButton(buttonText, this);
        connect(button, &QPushButton::clicked, this, [this, buttonText]() {
            onButtonClicked(buttonText);
        });
        ui->gridLayout->addWidget(button, row, column);
        column++;
        if (column >= 4) {
            column = 0;
            row++;
        }
    }
}

formula::~formula()
{
    delete ui;
}

void formula::updateButtons()
{
    QLayoutItem *item;
    while ((item = ui->gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    row = 0;
    column = 0;

    addButtonsToLayout(buttons);
    addButtonsToLayout(l);
}



void formula::close()
{
    emit closeSignal();
}

void formula::onButtonClicked(const QString &text)
{
    QString currentText = ui->lineEdit->text();
    ui->lineEdit->setText(currentText + text);
}

void formula::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}
