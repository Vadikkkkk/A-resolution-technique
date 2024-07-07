#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <QDialog>


namespace Ui {
class resolution;
}

class resolution : public QDialog
{
    Q_OBJECT

public:
    explicit resolution(QWidget *parent = nullptr);
    ~resolution();
    friend class MainWindow;

    void update();

private:
    Ui::resolution *ui;
    QVector<QString> literals;//массив литералов
    QVector<QString> formulas;//массив допущений
    QStringList S;//дизъюнкты
    QString theorem;


    QSet<QString> splitDisjunct(const QString& str);



    void formulaToS(const QString& str);
    void applyResolution();

    void removeBrackets(QString& str);

    void printResult(QString t);

    //приведение к кнф
    void removeImplication(QString& str);//избавляемся от -> Рабочая
    void removeEq(QString& str);//избавляемся от ==  Рабочая

    void removeNotBrackets(QString& str);//избавляемся от !( Рабочая

    void removeDoubleNot(QString& str);//избавляемся от двойного отрицания Рабочая
    void replaceParentheses(QString& str);//упрощение

    void ToCNF(QString& str);

    bool hasNoContradiction(const QString &formula);
    void removeDuplicateLiterals(QString &formula);


public slots:
    void close();

signals:
    void closeSignal();

protected:
    void closeEvent(QCloseEvent* event) override;
};

#endif // RESOLUTION_H
