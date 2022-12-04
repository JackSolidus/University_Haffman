#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>
#include"Three.h"

using namespace std;

void getData(string& path, vector<int>& temp, vector<int>& wind, vector<int>& rain, vector<int>& decisions);
int stringToInt(string& buff);

int main() {
	vector<int> temp;	//хранилище данных температуры
	vector<int> wind;	//хранилище данных ветра
	vector<int> rain;	//хранилище данных дождя
	vector<int> decision;	//хранилище данных решений
	string path;	//путь до файла
	three* nodes = NULL;	//Дерево
	cout << "Input path to file" << endl;	//запрашиваем путь к данным обучения
	cin >> path;	//считываем путь
	cout << endl;
	getData(path, temp, wind, rain, decision); //записываем данные из обучающего файла
	doRoot(temp, wind, rain, decision, nodes);
	doLeafs(nodes);
	printThree(nodes);
	temp.clear();
	wind.clear();
	rain.clear();
	decision.clear();
	cout << endl << "Input path to test file" << endl;	//запрашиваем путь к данным обучения
	cin >> path;	//считываем путь
	cout << endl;
	getData(path, temp, wind, rain, decision);
	vector<int> testDecision;
	for (int i = 0; i < (int)size(temp); i++) {
		getDecision(nodes, temp[i], wind[i], rain[i], testDecision);
	};
	double counterElements = 0;
	double counterOfEqual = 0;
	for (int i = 0; i < (int)size(decision); i++) {
		counterElements++;
		if (decision[i] == testDecision[i]) {
			counterOfEqual++;
		}
	}
	double precision = (counterOfEqual / counterElements) * 100;
	cout << "Presicion of decision three: " << precision << endl;
	system("pause");
	return 0;
}

void getData(string& path, vector<int>& temp, vector<int>& wind, vector<int>& rain, vector<int>& decisions) {
	string buff;
	char ch;
	int counter = 1;
	ifstream file;
	file.open(path);
	ch = file.get();
	while (ch != '\n') {
		if (ch == ' ') {
			counter++;
		}
		ch = file.get();
	}
	ch = file.get();
	counter = 1;
	while (!file.eof()) {
		if (ch == ' ') {
			switch (counter)
			{
			case 1:
				temp.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 2:
				wind.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 3:
				rain.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 4:
				decisions.push_back(stringToInt(buff));
				counter = 1;
				buff = "";
				ch = file.get();
				continue;
			default:
				break;
			}
		}
		else if (ch == '\n') {
			switch (counter)
			{
			case 1:
				temp.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 2:
				wind.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 3:
				rain.push_back(stringToInt(buff));
				counter++;
				buff = "";
				ch = file.get();
				continue;
			case 4:
				decisions.push_back(stringToInt(buff));
				counter = 1;
				buff = "";
				ch = file.get();
				continue;
			default:
				break;
			}
		}
		buff += ch;
		ch = file.get();
	}
	decisions.push_back(stringToInt(buff));
};

int stringToInt(string& buff) {
	int number = 0;
	int buffNum;
	int i = buff.length();
	int counter = 0;
	for (i >= 0; i--;) {
		buffNum = (int)buff[i];
		buffNum -= 48;
		number += buffNum * pow(10, counter);
		counter++;
	}
	return number;
};