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
    ui->formulas->clear();//отображение дизъюнктов и вывода из S
    for (QString& str : formulas) {//добавляем дизъюнкты
        str.insert(0,'(');//обернуть в скобки
        str.append(')');
        ToCNF(str);
        formulaToS(str);
    }
    QString tmp = theorem;
    theorem.insert(0,"!(");
    theorem.append(')');
    ToCNF(theorem);
    formulaToS(theorem);
    applyResolution();
    ui->formulas->addItems(S);
    printResult(tmp);
}

QSet<QString> resolution::splitDisjunct(const QString &str)
{
    return QSet<QString>::fromList(str.split("+"));
}

void resolution::applyResolution()
{
    bool newDisjunctAdded = true;

    while (newDisjunctAdded) {
        //if(S.contains("0")) return;
        newDisjunctAdded = false;
        QStringList newDisjuncts;

        for (int i = 0; i < S.size(); ++i) {
            for (int j = i + 1; j < S.size(); ++j) {

                QSet<QString> disjunct1 = splitDisjunct(S[i]);//множество слагаемых
                QSet<QString> disjunct2 = splitDisjunct(S[j]);

                bool foundContradiction = false;
                QString literalToRemove;
                QString negatedLiteralToRemove;

                for (const QString& literal : disjunct1) {
                    QString negatedLiteral = (literal.startsWith("!") ? literal.at(1) : "!" + literal);

                    if (disjunct2.contains(negatedLiteral)) {
                        foundContradiction = true;
                        literalToRemove = literal;
                        negatedLiteralToRemove = negatedLiteral;
                        break;
                    }
                }

                if (foundContradiction) {//елси найдены
                    QSet<QString> resolvent = disjunct1;
                    resolvent.unite(disjunct2);
                    resolvent.remove(literalToRemove);
                    resolvent.remove(negatedLiteralToRemove);

                    if (resolvent.isEmpty()) {
                        // В случае, если resolvent пустое, добавляем строку "0"
                        if (!S.contains("0")) {
                            newDisjuncts.append("0");
                            newDisjunctAdded = true;
                        }
                    } else {
                        QString resolventStr = QStringList(resolvent.toList()).join("+");
                        removeDuplicateLiterals(resolventStr);
                        if (!S.contains(resolventStr) && hasNoContradiction(resolventStr)) {
                            newDisjuncts.append(resolventStr);
                            newDisjunctAdded = true;
                        }
                    }
                }
            }
        }
        newDisjuncts.removeDuplicates();
        S.append(newDisjuncts);
    }
}

void resolution::formulaToS(const QString &str)
{
    if(!str.isEmpty()){
        QStringList substrings = str.split('*');
        for (QString& s : substrings) {
            removeBrackets(s);
            S.append(s);
        }
        S.removeDuplicates();
    }

}

void resolution::removeBrackets(QString &str)
{
    str.remove('(');
    str.remove(')');
}


void resolution::printResult(QString t)
{

    if (S.contains("0")){
        ui->result->setText("Формула " + t + " является логическим следствием "
                                                   "исходного множества формул! Так как из S"
                                                   " можно вывести пустой дизъюнкт '0'.");
    }
    else{
        ui->result->setText("Формула " + t + " не является логическим следствием "
                                                   "исходного множества формул! Так как из S "
                                                   "нельзя вывести пустой дизъюнкт '0'.");
    }
}

void resolution::removeImplication(QString &str)
{
    while(str.contains("->")){
        for (int i = 0; i < str.size(); i++ ) {
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
                    for (int j = i + 2; j < str.size(); j++ ) {//перебираем символы вправо
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
                    for (int j = i + 3; j < str.size(); j++ ) {//перебираем символы вправо
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
        for (int i = 0; i < str.size(); i++){
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
                    for (int j = i + 2; j < str.size(); j++ ) {//перебираем символы вправо
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
                    for (int j = i + 3; j < str.size(); j++ ) {//перебираем символы вправо
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
                    for (int j = i + 2; i < str.size(); j++) {
                        if (str.at(j) == '('){
                            level++;
                            openBrackets++;
                        }
                        if (str.at(j) == ')'){
                            level--;
                            closedBrackets++;
                        }
                        if(level == 0 && (str.at(j) == '+' || str.at(j) == '*')){
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
    str.replace(re,"");
}

void resolution::replaceParentheses(QString &str)
{
    QString previous = str;
    do{
        previous = str;

        for(int i = 0; i < str.size(); i++){
            int endIndex;
            QString subString;
            QString oprationsOnLevelZero;
            int openBrackets = 1;
            int closedBrackets = 0;
            int level = 0;
            int lastOpIndex = i + 1;
            QStringList slag;
            QStringList mul;
            QString res;
            QString lastOp;

            if(str.at(i) == '('){
                for(int j = i + 1; j < str.size(); j++){
                    subString.append(str.at(j));
                    if (str.at(j) == '('){
                        level++;
                        openBrackets++;
                    }
                    if (str.at(j) == ')'){
                        level--;
                        closedBrackets++;
                    }
                    if(level == 0 && str.at(j) == '+'){
                        oprationsOnLevelZero.append('+');
                    }
                    if(level == 0 && str.at(j) == '*'){
                        oprationsOnLevelZero.append('*');
                    }
                    if (openBrackets == closedBrackets){
                        subString.chop(1);
                        endIndex = j;
                        break;
                    }
                }
                if(oprationsOnLevelZero.contains('*') && !oprationsOnLevelZero.contains('+')){//CNF
                    str.replace(i, endIndex - i + 1, subString);
                    continue;
                }
                if(oprationsOnLevelZero.isEmpty()){//лишние скобки
                    str.replace(i, endIndex - i + 1, subString);
                    continue;
                }
                if(oprationsOnLevelZero.contains('+') && !oprationsOnLevelZero.contains('*')){//толко плюсы
                    continue;
                }
                if(oprationsOnLevelZero.contains('*') && oprationsOnLevelZero.contains('+')){
                    QString tmp;
                    int level = 0;
                    bool lastOpIsMul = false;
                    for(int j = 0; j < subString.size(); j++){
                        if(subString.at(j) == '(') level++;
                        if(subString.at(j) == ')') level--;
                        tmp.append(subString.at(j));
                        if((subString.at(j) == '*' || lastOpIsMul) && level == 0 && (subString.at(j) == '*' || subString.at(j) == '+')){
                            tmp.chop(1);
                            mul.append(tmp);
                            tmp.clear();
                            lastOpIndex = j;
                            if(subString.at(j) == '+'){
                                lastOpIsMul = false;
                            }
                            if(subString.at(j) == '*'){
                                lastOpIsMul = true;
                            }
                            continue;
                        }
                        if(subString.at(j) == '+' && level == 0){
                            tmp.chop(1);
                            slag.append(tmp);
                            tmp.clear();
                            lastOpIndex = j;
                            lastOpIsMul = false;
                        }
                    }
                    if(subString.at(lastOpIndex) == '+'){
                        slag.append(subString.mid(lastOpIndex+1));
                    }
                    if(subString.at(lastOpIndex) == '*'){
                        mul.append(subString.mid(lastOpIndex+1));
                    }
                    res = "(" + slag.join("+") + "+" + mul.at(0) + ")*(";
                    mul.removeFirst();
                    res += slag.join("+") + "+" + mul.join('*') + ")";
                }
                str.replace(i, subString.size()+2, res);
                res.clear();
                slag.clear();
                mul.clear();
            }
        }
    }while(str != previous);
}

void resolution::ToCNF(QString &str)
{
    removeEq(str);
    removeImplication(str);
    removeNotBrackets(str);
    removeDoubleNot(str);
    replaceParentheses(str);

    QStringList res;
    QStringList tmp = str.split('*');
    for (QString& s : tmp) {
        removeBrackets(s);
        if(hasNoContradiction(s)){
            removeDuplicateLiterals(s);
            res.append(s);
        }
    }

    str = res.join('*');

}

bool resolution::hasNoContradiction(const QString &formula)
{
    QSet<QString> literals;
    QStringList parts = formula.split('+');
    for (const QString &part : parts) {
        QString literal = part.trimmed();
        if (literal.startsWith('!')) {
            // Проверка на наличие отрицательного литерала
            QString positiveLiteral = literal.mid(1);
            if (literals.contains(positiveLiteral)) {
                return false;
            }
            literals.insert(literal);
        } else {
            // Проверка на наличие положительного литерала
            QString negativeLiteral = '!' + literal;
            if (literals.contains(negativeLiteral)) {
                return false;
            }
            literals.insert(literal);
        }
    }
    return true;
}

void resolution::removeDuplicateLiterals(QString &formula)
{
    QSet<QString> uniqueLiterals;
    QStringList parts = formula.split('+');
    QStringList result;

    for (const QString &part : parts) {
        QString literal = part.trimmed();
        if (!uniqueLiterals.contains(literal)) {
            uniqueLiterals.insert(literal);
            result.append(literal);
        }
    }

    formula = result.join('+');
}



void resolution::close()
{
    emit closeSignal();
}

void resolution::closeEvent(QCloseEvent *event)
{
    emit closeSignal();
}
