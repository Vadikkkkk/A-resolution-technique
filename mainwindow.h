#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include "dialog_add_literals.h"


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
    QSet<QString> literals;//массив литералов
    QSet<QString> formulas;//массив допущений

    Dialog_add_Literals* addLiterals;


public slots:
    void showAddLiterals();
    void closeAddLiterals();
    void saveNewLiteral();





};
#endif // MAINWINDOW_H
