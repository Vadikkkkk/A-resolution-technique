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
    QVector<QString> l;//массив литералов
    int braketsCount;

    void updateButtons();
    void addButtonsToLayout(const QVector<QString> &vector);
    QString getFormula();
    void clearLine();
    bool check();

private:
    Ui::formula *ui;
    int row, column;//для располо

    QVector<QString> buttons;

    void connectAllButtons();

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
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // FORMULA_H
