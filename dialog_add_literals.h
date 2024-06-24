#ifndef DIALOG_ADD_LITERALS_H
#define DIALOG_ADD_LITERALS_H

#include <QDialog>
#include <QWidget>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace Ui {
class Dialog_add_Literals;
}

class Dialog_add_Literals : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_Literals(QWidget *parent = nullptr);
    ~Dialog_add_Literals();
    friend class MainWindow;
    void clearLine();
    QString getLiteral();

private:
    Ui::Dialog_add_Literals *ui;

public slots:

    void close();
    void save();


signals:
    void closeSignal();
    void saveSignal();
protected:
    void closeEvent(QCloseEvent* event) override;

};

#endif // DIALOG_ADD_LITERALS_H
