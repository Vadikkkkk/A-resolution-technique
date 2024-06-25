#include "resolution.h"
#include "ui_resolution.h"

resolution::resolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resolution)
{
    ui->setupUi(this);

    connect(ui->close, &QPushButton::clicked, this, &resolution::closeSignal);
}

resolution::~resolution()
{
    delete ui;
}

void resolution::update()
{
    //removeImpFromFormulas();
    ui->formulas->clear();

    for (const QString& str : formulas) {
        ui->formulas->addItem(str);
    }


}

QString resolution::eliminateImplication(const QString &str)
{

}

QString resolution::eliminateEq(const QString &str)
{

}

QString resolution::toNegationNormalForm(const QString &str)
{

}

QVector<QSet<QString> > resolution::toCNF(const QString &str)
{

}

bool resolution::Resolution(const QVector<QSet<QString> > &clauses)
{

}

bool resolution::checkTheorem(const QVector<QString> &literals, const QVector<QString> &formulas, const QString &theorem)
{

}

void resolution::close()
{
    emit closeSignal();
}

void resolution::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}







/*
QString resolution::removeImplication(const QString &str)
{
    QString result = str;
        QString implication = "->";
        int pos = 0;

        while ((pos = result.indexOf(implication, pos)) != -1) {
            int start = pos;

            // Найти начало левой части
            int leftStart = start - 1;
            int leftParens = 0;
            while (leftStart >= 0 && (result[leftStart].isLetterOrNumber() || result[leftStart] == ')' || (result[leftStart] == '(' && leftParens > 0))) {
                if (result[leftStart] == ')') leftParens++;
                if (result[leftStart] == '(') leftParens--;
                leftStart--;
            }
            leftStart++;

            // Найти конец правой части
            int rightEnd = start + implication.length();
            int rightParens = 0;
            while (rightEnd < result.length() && (result[rightEnd].isLetterOrNumber() || result[rightEnd] == '(' || (result[rightEnd] == ')' && rightParens > 0))) {
                if (result[rightEnd] == '(') rightParens++;
                if (result[rightEnd] == ')') rightParens--;
                rightEnd++;
            }

            // Получить левую и правую части
            QString leftPart = result.mid(leftStart, start - leftStart);
            QString rightPart = result.mid(start + implication.length(), rightEnd - (start + implication.length()));

            // Заменить импликацию
            QString replacement = "(!" + leftPart + "+" + rightPart + ")";
            result.replace(leftStart, rightEnd - leftStart, replacement);

            // Продолжить поиск после текущей замены
            pos = leftStart + replacement.length();
        }

        return result;
}

void resolution::removeImpFromFormulas()
{
    QVector<QString> tmp;
    for (const QString& str : formulas) {
        tmp.append(removeImplication(str));
    }
    formulas.clear();
    formulas = tmp;
}
*/
