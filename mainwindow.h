#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>

#include "formula.h"
#include "resolution.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QString> literals;//массив литералов
    int count;
    QVector<QString> formulas;//массив допущений
    QString theorem;
    QString lastButton;

    formula* addFormula;
    resolution *res;

    void updateList();
    void updateFormulas();
    void updateTheorem();


public slots:
    void addLiteral();
    void deleteLiteral();

    void showAddFormula();
    void closeAddFormula();

    void saveFormula();
    void deleteFormula();

    void deleteTheorem();

    void showResolution();


};
#endif // MAINWINDOW_H
