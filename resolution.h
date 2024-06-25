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

private:
    Ui::resolution *ui;
    QVector<QString> literals;//массив литералов
    QVector<QString> formulas;//массив допущений
    QString theorem;
};

#endif // RESOLUTION_H
