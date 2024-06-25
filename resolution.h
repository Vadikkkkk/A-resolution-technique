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
    QString combineDisjuncts(const QSet<QString>& disjunct1, const QSet<QString>& disjunct2);
    void applyResolution();
    QString negateLiteral(const QString& literal);

    void negateCNF(const QString& cnf);
    void formulaToS(const QString& str);


    void removeBrackets(QString& str);




public slots:
    void close();

signals:
    void closeSignal();

    //QString removeImplication(const QString& str);
    //void removeImpFromFormulas();
protected:
    void closeEvent(QCloseEvent* event) override;
};

#endif // RESOLUTION_H
