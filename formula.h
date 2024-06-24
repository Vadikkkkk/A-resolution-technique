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
    QVector<QString> l;
    void updateButtons();
    void addButtonsToLayout(const QVector<QString> &vector);


private:
    Ui::formula *ui;
    int row, column;
    bool check;

    QVector<QString> buttons;


public slots:
    void close();
    void onButtonClicked(const QString &text);

signals:
    void closeSignal();

protected:
    void closeEvent(QCloseEvent* event) override;

};

#endif // FORMULA_H
