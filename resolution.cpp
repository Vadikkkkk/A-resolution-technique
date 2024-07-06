#include "resolution.h"
#include "ui_resolution.h"

resolution::resolution(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resolution)
{
    ui->setupUi(this);

    connect(ui->close, &QPushButton::clicked, this, &resolution::closeSignal);

    ui->result->setAlignment(Qt::AlignTop | Qt::AlignJustify);
    ui->result->setWordWrap(true);
}

resolution::~resolution()
{
    delete ui;
}

void resolution::update()
{
    ui->formulas->clear();

    //старое

    /*
    for (const QString& str : formulas) {
        formulaToS(str);
    }

    formulaToS(negateTheorem(theorem));

    applyResolution();
    ui->formulas->addItems(S);
    printResult();
    */

    //новое

    for (QString& str : formulas) {
        ui->formulas->addItem(str);
        removeEq(str);
        removeImplication(str);
        removeDoubleNot(str);
        ui->formulas->addItem(str);
    }
}

QSet<QString> resolution::splitDisjunct(const QString &str)
{
    return QSet<QString>::fromList(str.split("+"));
}

QString resolution::combineDisjuncts(const QSet<QString> &disjunct1, const QSet<QString> &disjunct2)
{
    QSet<QString> tmp = disjunct1;
    QSet<QString> result = tmp.unite(disjunct2);
    return QStringList(result.toList()).join("+");
}

void resolution::applyResolution()
{
    bool newDisjunctAdded = true;

    while (newDisjunctAdded) {
        newDisjunctAdded = false;
        QStringList newDisjuncts;

        for (int i = 0; i < S.size(); ++i) {
            for (int j = i + 1; j < S.size(); ++j) {
                removeBrackets(S[i]);
                removeBrackets(S[j]);
                QSet<QString> disjunct1 = splitDisjunct(S[i]);
                QSet<QString> disjunct2 = splitDisjunct(S[j]);

                bool foundContradiction = false;
                QString literalToRemove;
                QString negatedLiteralToRemove;

                for (const QString& literal : disjunct1) {
                    QString negatedLiteral = (literal.startsWith("!") ? literal.mid(1) : "!" + literal);

                    if (disjunct2.contains(negatedLiteral)) {
                        foundContradiction = true;
                        literalToRemove = literal;
                        negatedLiteralToRemove = negatedLiteral;
                        break;
                    }
                }

                if (foundContradiction) {
                    QSet<QString> resolvent = disjunct1;
                    resolvent.unite(disjunct2);
                    resolvent.remove(literalToRemove);
                    resolvent.remove(negatedLiteralToRemove);

                    if (resolvent.isEmpty()) {
                        // В случае, если resolvent пустое, добавляем строку "0"
                        if (!S.contains("0") && !newDisjuncts.contains("0")) {
                            newDisjuncts.append("0");
                            newDisjunctAdded = true;
                        }
                    } else {
                        QString resolventStr = QStringList(resolvent.toList()).join("+");
                        if (!S.contains(resolventStr) && !newDisjuncts.contains(resolventStr)) {
                            newDisjuncts.append(resolventStr);
                            newDisjunctAdded = true;
                        }
                    }
                }
            }
        }
        S.append(newDisjuncts);
    }
}

QString resolution::negateLiteral(const QString &literal)
{
    if (literal.startsWith('!')) {
        return literal.mid(1); // Убираем '!'
    } else {
        return '!' + literal; // Добавляем '!'
    }
}

QString resolution::negateTheorem(const QString &dnf)
{
    QStringList disjuncts = dnf.split("+");
    QStringList negatedDisjuncts;

    for (const QString& disjunct : disjuncts) {
        QStringList literals = disjunct.split('*');
        QStringList negatedLiterals;

        for (const QString& literal : literals) {
            // Инвертирование литералов
            negatedLiterals.append(negateLiteral(literal));
        }
        negatedDisjuncts.append(negatedLiterals.join("+"));
    }
    QString res = negatedDisjuncts.join("*");
    return res;
}

void resolution::formulaToS(const QString &str)
{
    QStringList substrings = str.split('*');
    for (const QString& s : substrings) {
        S.append(s);
    }
    S.removeDuplicates();
}

void resolution::removeBrackets(QString &str)
{
    str.remove('(');
    str.remove(')');
}

QString resolution::joinDisjunct(const QSet<QString> &disjunct)
{
    return QStringList(disjunct.toList()).join('+');
}

void resolution::printResult()
{

    if (S.contains("0")){
        ui->result->setText("Формула " + theorem + " является логическим следствием "
                                                   "исходного множества формул! Так как из S"
                                                   " можно вывести пустой дизъюнкт '0'.");
    }
    else{
        ui->result->setText("Формула " + theorem + " не является логическим следствием "
                                                   "исходного множества формул! Так как из S "
                                                   "нельзя вывести пустой дизъюнкт '0'.");
    }
}

void resolution::removeImplication(QString &str)
{
    while(str.contains("->")){
        for (int i = 0; i < str.size()-1; i++ ) {
            if(str.at(i) == '-'){//импликация
                QString left;
                QString right;
                int indexToReplace;

                str.replace(i, 2, '+');//заменяем на +
                if(str.at(i-1) == ')'){//если перед импликпцией скобка, то нужно вставить отрицание перед
                    //скобкой
                    int closedBrackets = 1;
                    int openBrackets = 0;
                    left += ")";
                    for (int j = i - 2;j >= 0 ; j--) {
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;
                        left.insert(0, str.at(j));
                        if (closedBrackets == openBrackets){//нужно вставить отрицание
                            indexToReplace = j;
                            //str.insert(j, "(!");
                            break;
                        }
                    }
                }
                if(left.isEmpty()){//если слева буква или ее отрицание
                    left += str.at(i-1);
                    if(i-2 >= 0){
                        if (str.at(i-2) == '!'){
                            left.insert(0, '!');
                            indexToReplace = i-2;
                        }
                        else{
                            indexToReplace = i-1;
                        }
                    }
                    else{
                        indexToReplace = i-1;
                    }
                }
                if(str.at(i+1) == '('){//если справа скобка
                    int closedBrackets = 0;
                    int openBrackets = 1;
                    right += "(";
                    for (int j = i + 2; j < str.size()-1; j++ ) {//перебираем символы вправо
                        right.append(str.at(j));
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;
                        if (openBrackets == closedBrackets){
                            break;
                        }
                    }
                }
                if(str.at(i+1) == '!' && str.at(i+2) != '('){//если справа отрицание буквы
                    right = '!' + str.at(i+2);
                }
                if(str.at(i+1) != '!' && str.at(i+1) != '('){//если справа просто буква
                    right += str.at(i+1);
                }
                if(str.at(i+1) == '!' && str.at(i+2) == '('){//если справа отрицание скобки
                    int openBrackets = 1;
                    int closedBrackets = 0;
                    right += "!(";
                    for (int j = i + 3; j < str.size()-1; j++ ) {//перебираем символы вправо
                        right.append(str.at(j));
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;
                        if (openBrackets == closedBrackets){
                            break;
                        }
                    }
                }
                QString newStr = "(!" + left + "+" + right + ")";
                str.replace(indexToReplace, left.size() + 1 + right.size(), newStr);
                QRegularExpression re("!!");
                str.replace(re, "");
            }
        }
    }
}

void resolution::removeEq(QString &str)
{
    while(str.contains("==")){
        for (int i = 0; i < str.size()-1; i++){
            if(str.at(i) == '='){//индекс эквивалентности
                QString left;
                QString right;
                int indexToReplace;
                str.replace(i, 2, '*');
                //
                if(str.at(i-1) == ')'){//если слева скобка
                    int closedBrackets = 1;
                    int openBrackets = 0;

                    left += ")";
                    for (int j = i - 2;j >= 0 ; j--){//перебираем символы влево
                        left.insert(0,str.at(j));//добавляем в левую часть
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;

                        if (closedBrackets == openBrackets){//нужно вставить отрицание
                            indexToReplace = j;
                            break;
                        }
                    }
                }
                if(str.at(i+1) == '('){//если справа скбока
                    int openBrackets = 1;
                    int closedBrackets = 0;
                    right += "(";
                    for (int j = i + 2; j < str.size()-1; j++ ) {//перебираем символы вправо
                        right.append(str.at(j));
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;
                        if (openBrackets == closedBrackets){
                            break;
                        }
                    }
                }
                if(str.at(i+1) == '!' && str.at(i+2) != '('){//если справа отрицание буквы
                    right = '!' + str.at(i+2);
                }
                if(str.at(i+1) == '!' && str.at(i+2) == '('){//если справа отрицание скобки
                    int openBrackets = 1;
                    int closedBrackets = 0;
                    right += "!(";
                    for (int j = i + 3; j < str.size()-1; j++ ) {//перебираем символы вправо
                        right.append(str.at(j));
                        if (str.at(j) == ')') closedBrackets++;
                        if (str.at(j) == '(') openBrackets++;
                        if (openBrackets == closedBrackets){
                            break;
                        }
                    }
                }
                if (str.at(i+1) != '(' && str.at(i+1) != '!'){//если справа просто буква
                    right += str.at(i+1);
                }
                if(left.isEmpty()){//если слева буква или ее отрицание
                    left += str.at(i-1);
                    if(i-2 >= 0){
                        if (str.at(i-2) == '!'){
                            left.insert(0, '!');
                            indexToReplace = i-2;
                        }
                        else{
                            indexToReplace = i-1;
                        }
                    }
                    else{
                        indexToReplace = i-1;
                    }
                }


                QString newStr = "((!" + left + "+" + right + ")*(" + left +"+!" + right + "))";
                str.replace(indexToReplace, left.size() + 1 + right.size(), newStr);
                QRegularExpression re("!!");
                str.replace(re, "");
            }
        }
    }
}

void resolution::removeNotBrackets(QString &str)
{
    while(str.contains("!(")){
        for (int i = 0; i < str.size(); i++) {
            if(str.at(i) == '!'){//отрицание

                QString subString;
                QString left;
                QString right;
                QString operation;
                int operationIndex = 0;
                int endIndex;
                if(str.at(i+1) != '('){//если отрицание литерала, то ничего не делаем

                }
                else{//если отрицание скобки
                    int openBrackets = 1;
                    int closedBrackets = 0;
                    int level = 0;
                    for (int j = i + 2; i < str.size() - 1; j++) {
                        if (str.at(j) == '('){
                            level++;
                            openBrackets++;
                        }
                        if (str.at(j) == ')'){
                            level--;
                            closedBrackets++;
                        }
                        if(level == 0 && (str.at(j) == '+' || str.at(j) == '*')){
                            //operation = str.at(j);
                            if(str.at(j) == '+') operation = "*";
                            else operation = "+";
                            operationIndex = j;
                        }
                        subString.append(str.at(j));
                        if (openBrackets == closedBrackets){
                            subString.chop(1);
                            endIndex = j;
                            break;
                        }
                    }
                    if(operationIndex != 0){//внутри есть операция
                        left = str.mid(i+2, operationIndex - i - 2);//левая часть внутри скобки
                        right = str.mid(operationIndex + 1, endIndex - operationIndex - 1);
                        str.replace(i, endIndex - i + 1, "(!(" + left + ")" + operation + "!(" + right + "))");
                        //str.remove(i+1, endIndex - i + 1);
                    }
                    else{//внутри нет операции
                        str.remove(i+1, 1);
                        str.remove(endIndex-1, 1);
                    }
                }
            }
        }
    }
}

void resolution::removeDoubleNot(QString &str)
{
    QRegularExpression re("!!");
    str.replace(re, "");
}

void resolution::close()
{
    emit closeSignal();
}

void resolution::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}
