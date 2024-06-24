#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>

#include "formula.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    friend class Dialog_add_Literals;

    QString getLiteral();

    //const QSet<QString>& getLiterals() const;

private:
    Ui::MainWindow *ui;
    QVector<QString> literals;//массив литералов
    int count;
    QSet<QString> formulas;//массив допущений

    formula* addFormula;

    void updateList();


public slots:
    void addLiteral();
    void deleteLiteral();

    void showAddFormula();
    void closeAddFormula();





};
#endif // MAINWINDOW_H
