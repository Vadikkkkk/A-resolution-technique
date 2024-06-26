#include "formula.h"
#include "ui_formula.h"
#include <QMessageBox>

formula::formula(QWidget *parent) :
    QDialog(parent), row(0), column(0), braketsCount(0),
    ui(new Ui::formula)
{
    ui->setupUi(this);

    connectAllButtons();
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

QString formula::getFormula()
{
    return ui->lineEdit->text();
}

void formula::clearLine()
{
    ui->lineEdit->clear();
}

bool formula::check()
{
    auto isPartOfSet = [](const QString &ch, const QVector<QString> &set) -> bool {
            return set.contains(ch);
        };

    QVector<QString> specialSet = {"*", "+", "->", "=="};
    QString str = ui->lineEdit->text();
    int eq = 0;
    bool ch = true;

    if(braketsCount || str[0] == "+" || str[0] == "*" || str.contains("!+") ||
            str.contains("!*") || str.contains("!->") || str.contains("(*")
            || str.contains("(+") || str.contains("(-")) return false;

    if(str.contains("-") && !str.contains("->")) return false;

    for (int i = 0; i < str.length(); ++i) {
        // Check if the current character and the next character are both in l
        if (i < str.length() - 1 && isPartOfSet(str.mid(i, 1), l) && isPartOfSet(str.mid(i + 1, 1), l)) {
            return false;
        }
        if (str[i] == "=") eq++;
        if (l.contains(QString(str[i]))) ch = false;

        // Check if the current character and the next character are both in specialSet
        for (const QString &special : specialSet) {
            if (i < str.length() - special.length() && str.mid(i, special.length()) == special &&
                str.mid(i + special.length(), special.length()) == special) {
                return false;
            }
            if (str.endsWith(special) || str.endsWith("!")) return false;
        }
    }
    if(eq % 2 || ch) return false;

    return true;
}

void formula::connectAllButtons()
{
    connect(ui->close_button, &QPushButton::clicked, this, &formula::close);
    connect(ui->backspace, &QPushButton::clicked, this, &formula::backspace);
    connect(ui->save_button, &QPushButton::clicked, this, &formula::save);
    connect(this, &formula::backspacePressed, this, &formula::backspace);

    buttons << "!" << "+" << "*" << "->" << "==" << "(" << ")";
    addButtonsToLayout(buttons);
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

    if(text == "("){
        braketsCount++;
        ui->lineEdit->setText(currentText + text);
    }
    else if (text == ")" && braketsCount == 0){
        QMessageBox::information(this, "Ошибка!", "Нет открытых скобок!");
    }
    else if (text == ")" && braketsCount > 0){
        braketsCount--;
        ui->lineEdit->setText(currentText + text);
    }
    else if (currentText.isEmpty() && !l.contains(text) && text != "!"){

    }
    else {
        ui->lineEdit->setText(currentText + text);
    }
}

void formula::backspace()
{
    QString currentText = ui->lineEdit->text();
    if(!currentText.isEmpty()){
        if(currentText.at(currentText.length() - 1) == ')') braketsCount++;
        if(currentText.at(currentText.length() - 1) == '(') braketsCount--;
        currentText.chop(1);
        ui->lineEdit->clear();
        ui->lineEdit->setText(currentText);
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Формула пуста!");
    }
}

void formula::save()
{
    if(check()){
        emit saveSignal();
    }
    else{
        QMessageBox::information(this, "Ошибка!", "Неверная формула!");
    }

}

void formula::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}

void formula::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace) {
        emit backspacePressed();
    }
    else if (event->key() == Qt::Key_Return){
        emit saveSignal();
    }
    else if (event->key() == Qt::Key_Escape){
        emit closeSignal();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
