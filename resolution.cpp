#include "resolution.h"
#include "ui_resolution.h"

resolution::resolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resolution)
{
    ui->setupUi(this);

    connect(ui->close, &QPushButton::clicked, this, &resolution::closeSignal);

    ui->result->setAlignment(Qt::AlignTop | Qt::AlignJustify);
    ui->result->setWordWrap(true);
}

resolution::~resolution()
{
    delete ui;
}

void resolution::update()
{
    ui->formulas->clear();

    for (const QString& str : formulas) {
        formulaToS(str);
    }

    formulaToS(negateTheorem(theorem));

    applyResolution();
    ui->formulas->addItems(S);
    printResult();

}

QSet<QString> resolution::splitDisjunct(const QString &str)
{
    return QSet<QString>::fromList(str.split("+"));
}

QString resolution::combineDisjuncts(const QSet<QString> &disjunct1, const QSet<QString> &disjunct2)
{
    QSet<QString> tmp = disjunct1;
    QSet<QString> result = tmp.unite(disjunct2);
    return QStringList(result.toList()).join("+");
}

void resolution::applyResolution()
{
    bool newDisjunctAdded = true;

    while (newDisjunctAdded) {
        newDisjunctAdded = false;
        QStringList newDisjuncts;

        for (int i = 0; i < S.size(); ++i) {
            for (int j = i + 1; j < S.size(); ++j) {
                removeBrackets(S[i]);
                removeBrackets(S[j]);
                QSet<QString> disjunct1 = splitDisjunct(S[i]);
                QSet<QString> disjunct2 = splitDisjunct(S[j]);

                bool foundContradiction = false;
                QString literalToRemove;
                QString negatedLiteralToRemove;

                for (const QString& literal : disjunct1) {
                    QString negatedLiteral = (literal.startsWith("!") ? literal.mid(1) : "!" + literal);

                    if (disjunct2.contains(negatedLiteral)) {
                        foundContradiction = true;
                        literalToRemove = literal;
                        negatedLiteralToRemove = negatedLiteral;
                        break;
                    }
                }

                if (foundContradiction) {
                    QSet<QString> resolvent = disjunct1;
                    resolvent.unite(disjunct2);
                    resolvent.remove(literalToRemove);
                    resolvent.remove(negatedLiteralToRemove);

                    if (resolvent.isEmpty()) {
                        // В случае, если resolvent пустое, добавляем строку "0"
                        if (!S.contains("0") && !newDisjuncts.contains("0")) {
                            newDisjuncts.append("0");
                            newDisjunctAdded = true;
                        }
                    } else {
                        QString resolventStr = QStringList(resolvent.toList()).join("+");
                        if (!S.contains(resolventStr) && !newDisjuncts.contains(resolventStr)) {
                            newDisjuncts.append(resolventStr);
                            newDisjunctAdded = true;
                        }
                    }
                }
            }
        }
        S.append(newDisjuncts);
    }
}

QString resolution::negateLiteral(const QString &literal)
{
    if (literal.startsWith('!')) {
        return literal.mid(1); // Убираем '!'
    } else {
        return '!' + literal; // Добавляем '!'
    }
}

QString resolution::negateTheorem(const QString &dnf)
{
    QStringList disjuncts = dnf.split("+");
    QStringList negatedDisjuncts;

    for (const QString& disjunct : disjuncts) {
        QStringList literals = disjunct.split('*');
        QStringList negatedLiterals;

        for (const QString& literal : literals) {
            // Инвертирование литералов
            negatedLiterals.append(negateLiteral(literal));
        }
        negatedDisjuncts.append(negatedLiterals.join("+"));
    }
    QString res = negatedDisjuncts.join("*");
    return res;
}

void resolution::formulaToS(const QString &str)
{
    QStringList substrings = str.split('*');
    for (const QString& s : substrings) {
        S.append(s);
    }
    S.removeDuplicates();
}

void resolution::removeBrackets(QString &str)
{
    str.remove('(');
    str.remove(')');
    str.remove(' ');
}

QString resolution::joinDisjunct(const QSet<QString> &disjunct)
{
    return QStringList(disjunct.toList()).join('+');
}

void resolution::printResult()
{

    if (S.contains("0")){
        ui->result->setText("Формула " + theorem + " является логическим следствием "
                                                   "исходного множества формул! Так как из S"
                                                   " можно вывести пустой дизъюнкт '0'.");
    }
    else{
        ui->result->setText("Формула " + theorem + " не является логическим следствием "
                                                   "исходного множества формул! Так как из S "
                                                   "нельзя вывести пустой дизъюнкт '0'.");
    }
}

void resolution::close()
{
    emit closeSignal();
}

void resolution::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}
