#ifndef FORMULA_H
#define FORMULA_H

#include <QDialog>
#include <QSet>
#include <QString>
#include <QKeyEvent>



namespace Ui {
class formula;
}

class formula : public QDialog
{
    Q_OBJECT

public:
    explicit formula(QWidget *parent = nullptr);
    ~formula();
    QVector<QString> l;
    void updateButtons();
    void addButtonsToLayout(const QVector<QString> &vector);
    QString getFormula();
    void clearLine();
    bool check();
    int braketsCount;

private:
    Ui::formula *ui;
    int row, column;



    QVector<QString> buttons;





public slots:
    void close();
    void onButtonClicked(const QString &text);
    void backspace();

    void save();

signals:
    void closeSignal();
    void saveSignal();
    void backspacePressed();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Backspace) {
            emit backspacePressed();
        } else {
            QWidget::keyPressEvent(event);
        }
    }

};

#endif // FORMULA_H
