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

    int count;
    QVector<QString> literals;//массив литералов
    QVector<QString> formulas;//массив допущений
    QString theorem;//теорема

    QString lastButton;

    formula* addFormula;
    resolution *res;

    void updateList();//обновляет список переменных
    void updateFormulas();//обновляет список формул
    void updateTheorem();//обновляет теорему

    void connectAllButtons();


public slots:
    void addLiteral();
    void deleteLiteral();

    void showAddFormula();
    void closeAddFormula();

    void saveFormula();
    void deleteFormula();

    void deleteTheorem();

    void showResolution();
    void closeResolution();

};
#endif // MAINWINDOW_H
