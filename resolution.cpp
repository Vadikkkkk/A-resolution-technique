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
    ui->formulas->clear();
    for (const QString& str : formulas) {
        formulaToS(str);
    }
    negateCNF(theorem);


    applyResolution();
    ui->formulas->addItems(S);
    if (S.contains("0")) ui->result->setText("Теорема " + theorem + " верна!");
    else ui->result->setText("Теорема " + theorem + " не верна!");

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

void resolution::negateCNF(const QString &cnf)
{
    QStringList conjuncts = cnf.split('*');
    QStringList negatedConjuncts;

    for (const QString& conjunct : conjuncts) {
        // Разделение конъюнкта на дизъюнкты
        QStringList literals = conjunct.split('+');
        QStringList negatedLiterals;

        for (const QString& literal : literals) {
            // Инвертирование литералов
            negatedLiterals.append(negateLiteral(literal));
        }

        // Объединение инвертированных литералов в новый дизъюнкт

        //negatedConjuncts.append('(' + negatedLiterals.join('*') + ')');
        S.append(negatedLiterals);
    }

    // Объединение инвертированных конъюнктов в новую дизъюнкцию
    /*
    QString res = negatedConjuncts.join('+');
    res.remove('(');
    res.remove(')');
    return res;
    */
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
}

void resolution::close()
{
    emit closeSignal();
}

void resolution::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}






