#ifndef Three
#define Three

#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<iomanip>

using namespace std;
void getGini(vector<int>& data, vector<int>& decision, vector<double>& ginis, vector<double>& values, vector<double>& conditions);

//структура дерева
struct three {
    double value = NULL;
    char condition = NULL;
    double gini = NULL;
    int decision;
    int param;
    vector<int> temp;
    vector<int> wind;
    vector<int> rain;
    vector<int> decisions;
    three* left = NULL;
    three* right = NULL;
};

void getDecision(three* node, int& temp, int& wind, int& rain, vector<int>& testDesicion) {
    int param;
    if (node->decision == 0 || node->decision == 1) {
        testDesicion.push_back(node->decision);
        return;
    }
    param = node->param;
    switch (param)
    {
        case 0:
            if (temp < node->value) {
                getDecision(node->left, temp, wind, rain, testDesicion);
            }
            else if (temp >= node->value) {
                getDecision(node->right, temp, wind, rain, testDesicion);
            }
        case 1:
            if (wind < node->value) {
                getDecision(node->left, temp, wind, rain, testDesicion);
            }
            else if (wind >= node->value) {
                getDecision(node->right, temp, wind, rain, testDesicion);
            }
        case 2:
            if (rain == 1) {
                getDecision(node->left, temp, wind, rain, testDesicion);
            }
            else if (rain != 1) {
                getDecision(node->right, temp, wind, rain, testDesicion);
            }
    }
};

void printBuild(three* node, vector<string>& name, vector<int>& rows, vector<int>& idents, vector<double>& value, int ident = 70, int row = 0) {
    if (node->decision == 0 || node->decision == 1) {
        name.push_back("Decision: ");
        idents.push_back(ident);
        rows.push_back(row);
        value.push_back(node->decision);
        return;
    }
    else {
        name.push_back("Value: ");
        idents.push_back(ident);
        rows.push_back(row);
        value.push_back(node->value);
    }
    printBuild(node->left, name, rows, idents, value, ident - 10, row + 1);
    printBuild(node->right, name, rows, idents, value, ident + 10, row + 1);
};

void printThree(three* node) {
    vector<string> name;
    vector<int> row;
    vector<int> ident;
    vector<double> value;
    printBuild(node, name, row, ident, value);
    int buffRow = row[0];
    for (int i = 0; i < (int)size(row); i++) {
        if (buffRow < row[i]) {
            buffRow = row[i];
        }
    }
    int backslash;
    int identif = 0;
    for (int i = 0; i <= buffRow; i++) {
        for (int c = 0; c < (int)size(row); c++) {
            if (row[c] == i) {
                cout << setw(ident[c] - identif) << name[c] << value[c];
                identif = ident[c];
                if (name[c] == "Value: ") {
                    backslash = ident[c];
                }
            }
        }
        identif = 0;
        if (i != buffRow) {
            cout << endl;
            cout << setw(backslash - 5) << "/" << setw(7) << "\\" << endl;
        }
    }
};


void pushData(three*& node, int data, int flag) {
    if (node == NULL) {
        node = new three;
    }
    switch (flag)
    {
        case 0:
            node->temp.push_back(data);
            return;
        case 1:
            node->wind.push_back(data);
            return;
        case 2:
            node->rain.push_back(data);
            return;
        case 3:
            node->decisions.push_back(data);
            return;
    }
};


void pushNode(three*& node, double value, char condition, double gini, int param) {
    if (node == NULL) {
        node = new three;
    }
    node->value = value;
    node->condition = condition;
    node->gini = gini;
    node->param = param;
};

void pushRoot(three*& node, double value, char condition, double gini, int param, vector<int> temp, vector<int> wind, vector<int> rain, vector<int> decisions) {
    if (node == NULL) {
        node = new three;
    }
    node->value = value;
    node->condition = condition;
    node->gini = gini;
    node->temp = temp;
    node->wind = wind;
    node->rain = rain;
    node->decisions = decisions;
    node->param = param;
};

void pushLeaf(three*& node, int decision) {
    if (node == NULL) {
        node = new three;
    }
    node->decision = decision;
};

void doLeafs(three*& node) {
    if (node->decision == 0 || node->decision == 1) {
        return;
    }
    int pointerOfClass = 0, param = 0;
    vector<double> allGini, values, conditions;
    bool flagLeft = 1, flagRight = 1;
    double countTrue0 = 0;
    double countTrue1 = 0;
    double countFalse0 = 0;
    double countFalse1 = 0;
    double allTrue = 0, allFalse = 0;
    if (node->value == NULL) {
        getGini(node->temp, node->decisions, allGini, values, conditions);
        getGini(node->wind, node->decisions, allGini, values, conditions);
        getGini(node->rain, node->decisions, allGini, values, conditions);
        double gini = allGini[0];
        for (int i = 0; i < (int)size(allGini); i++) {
            if (gini > allGini[i]) {
                gini = allGini[i];
                pointerOfClass = i;
            }
        }
        pushNode(node, values[pointerOfClass], conditions[pointerOfClass], allGini[pointerOfClass], pointerOfClass);
    }
    double value = node->value;
    param = node->param;
    switch (param)
    {
        case 0:
            for (int i = 0; i < (int)size(node->temp); i++) {
                if (node->temp[i] < value) {
                    if (node->decisions[i] == 1) {
                        countTrue1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countTrue0++;
                    }
                }
            }
            for (int i = 0; i < (int)size(node->temp); i++) {
                if (node->temp[i] >= value) {
                    if (node->decisions[i] == 1) {
                        countFalse1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countFalse0++;
                    }
                }
            }
            allTrue = countTrue0 + countTrue1;
            allFalse = countFalse0 + countFalse1;
            if (countTrue0 == allTrue) {
                pushLeaf(node->left, 0);
                flagLeft = false;
            }
            else if (countTrue1 == allTrue) {
                pushLeaf(node->left, 1);
                flagLeft = false;
            }
            if (countFalse0 == allFalse) {
                pushLeaf(node->right, 0);
                flagRight = false;
            }
            else if (countFalse1 == allFalse) {
                pushLeaf(node->right, 1);
                flagRight = false;
            }
            if (flagLeft) {
                for (int i = 0; i < (int)size(node->temp); i++) {
                    if (node->temp[i] < value) {
                        pushData(node->left, node->temp[i], 0);
                        pushData(node->left, node->wind[i], 1);
                        pushData(node->left, node->rain[i], 2);
                        pushData(node->left, node->decisions[i], 3);
                    }
                }
            }
            if (flagRight) {
                for (int i = 0; i < (int)size(node->temp); i++) {
                    if (node->temp[i] >= value) {
                        pushData(node->right, node->temp[i], 0);
                        pushData(node->right, node->wind[i], 1);
                        pushData(node->right, node->rain[i], 2);
                        pushData(node->right, node->decisions[i], 3);
                    }
                }
            }
            break;
        case 1:
            for (int i = 0; i < (int)size(node->wind); i++) {
                if (node->wind[i] < value) {
                    if (node->decisions[i] == 1) {
                        countTrue1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countTrue0++;
                    }
                }
            }
            for (int i = 0; i < (int)size(node->wind); i++) {
                if (node->wind[i] >= value) {
                    if (node->decisions[i] == 1) {
                        countFalse1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countFalse0++;
                    }
                }
            }
            allTrue = countTrue0 + countTrue1;
            allFalse = countFalse0 + countFalse1;
            if (countTrue0 == allTrue) {
                pushLeaf(node->left, 0);
                flagLeft = false;
            }
            else if (countTrue1 == allTrue) {
                pushLeaf(node->left, 1);
                flagLeft = false;
            }
            if (countFalse0 == allFalse) {
                pushLeaf(node->right, 0);
                flagRight = false;
            }
            else if (countFalse1 == allFalse) {
                pushLeaf(node->right, 1);
                flagRight = false;
            }
            if (flagLeft) {
                for (int i = 0; i < (int)size(node->wind); i++) {
                    if (node->wind[i] < value) {
                        pushData(node->left, node->temp[i], 0);
                        pushData(node->left, node->wind[i], 1);
                        pushData(node->left, node->rain[i], 2);
                        pushData(node->left, node->decisions[i], 3);
                    }
                }
            }
            if (flagRight) {
                for (int i = 0; i < (int)size(node->wind); i++) {
                    if (node->wind[i] >= value) {
                        pushData(node->right, node->temp[i], 0);
                        pushData(node->right, node->wind[i], 1);
                        pushData(node->right, node->rain[i], 2);
                        pushData(node->right, node->decisions[i], 3);
                    }
                }
            }
            break;
        case 3:
            for (int i = 0; i < (int)size(node->rain); i++) {
                if (node->rain[i] == value) {
                    if (node->decisions[i] == 1) {
                        countTrue1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countTrue0++;
                    }
                }
            }
            for (int i = 0; i < (int)size(node->rain); i++) {
                if (node->rain[i] != value) {
                    if (node->decisions[i] == 1) {
                        countFalse1++;
                    }
                    else if (node->decisions[i] != 1) {
                        countFalse0++;
                    }
                }
            }
            allTrue = countTrue0 + countTrue1;
            allFalse = countFalse0 + countFalse1;
            if (countTrue0 == allTrue) {
                pushLeaf(node->left, 0);
                flagLeft = false;
            }
            else if (countTrue1 == allTrue) {
                pushLeaf(node->left, 1);
                flagLeft = false;
            }
            if (countFalse0 == allFalse) {
                pushLeaf(node->right, 0);
                flagRight = false;
            }
            else if (countFalse1 == allFalse) {
                pushLeaf(node->right, 1);
                flagRight = false;
            }
            if (flagLeft) {
                for (int i = 0; i < (int)size(node->rain); i++) {
                    if (node->rain[i] == value) {
                        pushData(node->left, node->temp[i], 0);
                        pushData(node->left, node->wind[i], 1);
                        pushData(node->left, node->rain[i], 2);
                        pushData(node->left, node->decisions[i], 3);
                    }
                }
            }
            if (flagRight) {
                for (int i = 0; i < (int)size(node->rain); i++) {
                    if (node->rain[i] != value) {
                        pushData(node->right, node->temp[i], 0);
                        pushData(node->right, node->wind[i], 1);
                        pushData(node->right, node->rain[i], 2);
                        pushData(node->right, node->decisions[i], 3);
                    }
                }
            }
    }
    doLeafs(node->right);
    doLeafs(node->left);
};

void doRoot(vector<int>& temp, vector<int>& wind, vector<int>& rain, vector<int>& decision, three*& node) {
    double gini;
    int pointerOfClass = 0;
    vector<double> ginis;
    vector<double> values;
    vector<double> conditions;
    getGini(temp, decision, ginis, values, conditions);
    getGini(wind, decision, ginis, values, conditions);
    getGini(rain, decision, ginis, values, conditions);
    gini = ginis[0];
    for (int i = 1; i < (int)size(ginis); i++) {
        if (gini > ginis[i]) {
            gini = ginis[i];
            pointerOfClass = i;
        }
    }
    pushRoot(node, values[pointerOfClass], conditions[pointerOfClass], ginis[pointerOfClass], pointerOfClass, temp, wind, rain, decision);
};

void getGini(vector<int>& data, vector<int>& decision, vector<double>& allGini, vector<double>& values, vector<double>& conditions) {
    char condition = '=';	//Условие сравнение данныз
    double countTrue0 = 0;	//Счётчик нулей при положительном ответе
    double countTrue1 = 0;	//Счётчик единиц при положительном ответе
    double countFalse0 = 0;	//Счётчик нулей при отрицательном ответе
    double countFalse1 = 0;	//Счётчик единиц при отрицательном ответе
    double allTrue, allFalse;
    double giniTrue, giniFalse, gini;
    vector<double> ginis;
    vector<double> average;
    vector<double> dataCopy;

    int buff = (int)size(data);	//Проверяем данные и если значение больше 1 или меньше 0 то ставим условие меньше и выходим из цикла
    for (int i = 0; i < buff; i++) {
        if (data[i] > 1 || data[i] < 0) {
            condition = '<';
            break;
        }
    }
    switch (condition)	//проверяем условие
    {
        case '=':
            conditions.push_back(condition);	//записываем услорвие
            for (int i = 0; i < buff; i++) {		//Считаем количество единиц и нулей отдельно для положительного и отрицательного ответа
                if (data[i] == 1) {
                    if (decision[i] == 1) {
                        countTrue1++;
                    }
                    else if (decision[i] != 1) {
                        countTrue0++;
                    }
                }
                else if (data[i] != 1) {
                    if (decision[i] == 1) {
                        countFalse1++;
                    }
                    else if (decision[i] != 1) {
                        countFalse0++;
                    }
                }
            };
            allTrue = countTrue1 + countTrue0;
            allFalse = countFalse1 + countFalse0;
            giniTrue = 1 - pow(countTrue1 / (allTrue), 2) - pow(countTrue0 / (allTrue), 2);	//Считаем джинни для положительного ответа
            giniFalse = 1 - pow(countFalse1 / (allFalse), 2) - pow(countFalse0 / (allFalse), 2);	//Считаем джинни для отрицательного ответа
            gini = ((allTrue / (allTrue + allFalse)) * giniTrue) + ((allFalse / (allTrue + allFalse)) * giniFalse);	//Считаем джинни теста
            values.push_back(1);	//записываем тест
            allGini.push_back(gini);	//записываем значение джинни
            break;
        case '<':
            conditions.push_back(condition);
            buff = (int)size(data);
            for (int i = 0; i < buff; i++) {	//Копируем дату для сортировки и удаления повторных элементов
                dataCopy.push_back(data[i]);
            }
            sort(dataCopy.begin(), dataCopy.end());	//сортируем по возрастанию
            int compare = dataCopy[0];
            for (int i = 1; i < (int)size(dataCopy); i++) {		//удаляем повторные жлемениы
                if (compare == dataCopy[i]) {
                    dataCopy.erase(dataCopy.begin() + i);
                    i--;
                }
                else {
                    compare = dataCopy[i];
                }
            }
            buff = (int)size(dataCopy);
            for (int i = 0; i < buff - 1; i++) {	//ищем среднии значения между элементами
                int c = i + 1;
                average.push_back((dataCopy[i] + dataCopy[c]) / 2);
            }
            if (buff == 1) {
                average.push_back(dataCopy[0] + 0.5);
            }

            buff = (int)size(average);
            for (int i = 0; i < buff; i++) {		//Считаем кол-во единиц и нулей отдельно для положительного и отрицательного ответа
                int buff2 = (int)size(data);
                for (int c = 0; c < buff2; c++) {
                    if (data[c] < average[i]) {
                        if (decision[c] == 1) {
                            countTrue1++;
                        }
                        else if (decision[c] != 1) {
                            countTrue0++;
                        }
                    }
                    else if (data[c] >= average[i]) {
                        if (decision[c] == 1) {
                            countFalse1++;
                        }
                        else if (decision[c] != 1) {
                            countFalse0++;
                        }
                    }
                }
                allTrue = countTrue1 + countTrue0;
                allFalse = countFalse1 + countFalse0;
                giniTrue = 1 - pow(countTrue1 / (allTrue), 2) - pow(countTrue0 / (allTrue), 2);
                giniFalse = 1 - pow(countFalse1 / (allFalse), 2) - pow(countFalse0 / (allFalse), 2);
                gini = ((allTrue / (allTrue + allFalse)) * giniTrue) + ((allFalse / (allTrue + allFalse)) * giniFalse);
                countTrue1 = 0;
                countTrue0 = 0;
                countFalse1 = 0;
                countFalse0 = 0;
                ginis.push_back(gini);
            }

            int position = NULL;
            buff = (int)size(ginis);
            gini = ginis[0];
            for (int i = 0; i < buff; i++) {	//определяем наименьший джинни
                if (gini > ginis[i]) {
                    gini = ginis[i];
                    position = i;
                }
            }
            allGini.push_back(gini);
            values.push_back(average[position]);
            break;
    }
};

#endif //Three