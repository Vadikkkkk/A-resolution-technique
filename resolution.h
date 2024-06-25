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

private:
    Ui::resolution *ui;
};

#endif // RESOLUTION_H
