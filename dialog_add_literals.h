#ifndef DIALOG_ADD_LITERALS_H
#define DIALOG_ADD_LITERALS_H

#include <QDialog>

namespace Ui {
class Dialog_add_Literals;
}

class Dialog_add_Literals : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_Literals(QWidget *parent = nullptr);
    ~Dialog_add_Literals();

private:
    Ui::Dialog_add_Literals *ui;
};

#endif // DIALOG_ADD_LITERALS_H
