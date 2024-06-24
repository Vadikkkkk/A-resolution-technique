#ifndef FORMULA_H
#define FORMULA_H

#include <QDialog>
#include <QSet>
#include <QString>


namespace Ui {
class formula;
}

class formula : public QDialog
{
    Q_OBJECT

public:
    explicit formula(QWidget *parent = nullptr);
    ~formula();

private:
    Ui::formula *ui;
    QSet<QString> l;
};

#endif // FORMULA_H
