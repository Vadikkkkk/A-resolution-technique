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
    QString theorem;




    QString eliminateImplication(const QString &str);
    QString eliminateEq (const QString &str);
    QString toNegationNormalForm(const QString &str);
    QVector<QSet<QString>> toCNF(const QString &str);
    bool Resolution(const QVector<QSet<QString>> &clauses);

    bool checkTheorem(const QVector<QString> &literals, const QVector<QString> &formulas,
                      const QString &theorem);

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
