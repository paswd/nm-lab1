#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "types.h"
#include "matrix.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";

void LUSolve(string filename) {

	TMatrix matrix;
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t size = 0;
	vector <TNum> b;
	//cout << b.size() << endl;
	try {
		bool readres = matrix.ReadFromFile(fin, true);
		//cout << "POINT1" << endl;

		if (!readres) {
			throw 1;
		}

		//cout << "POINT2" << endl;

		size = matrix.GetHeight();
		b.resize(size);

		//cout << "POINT3" << endl;
		

		for (size_t i = 0; i < size; i++) {
			if (!(fin >> b[i])) {
				throw 1;
			}
		}

		//cout << "POINT4" << endl;
	} catch (int a) {
		fin.close();
		cout << "Ошибка чтения из файла" << endl;
		return;
	}
	cout << "Результирующий вектор успешно импортирован:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << b[i] << " ";
	}
	cout << endl;

	//cout << "POINT5" << endl;
	fin.close();
	//cout << "POINT6" << endl;
	TMatrix lu;
	LU(&matrix, &lu);
	//cout << "POINT7" << endl;
	if (lu.IsNull()) {
		cout << "Error LU" << endl;
		return;
	}
	//cout << "POINT8" << endl;

	vector <TNum> y(size, 0);
	for (size_t i = 0; i < size; i++) {
		y[i] = b[i];
		for (size_t j = 0; j < i; j++) {
			y[i] -= y[j] * lu.GetValue(i, j);
		}
	}
	//cout << "POINT9" << endl;

	vector <TNum> x(size, 0);
	for (size_t it = size; it > 0; it--) {
		size_t i = it - 1;
		x[i] = y[i];
		for (size_t j = it; j < size; j++) {
			x[i] -= x[j] * lu.GetValue(i, j);
		}
		x[i] /= lu.GetValue(i, i);
	}
	//cout << "POINT10" << endl;

	cout << "Решение СЛАУ:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
	//cout << "POINT11" << endl;
}

void RunSolve(string filename) {
	cout << filename << endl;
}

void BasicIterationSolve(string filename) {
	cout << filename << endl;
}

void ZeidelSolve(string filename) {
	cout << filename << endl;
}

void RotateSolve(string filename) {
	cout << filename << endl;
}

void QRSolve(string filename) {
	cout << filename << endl;
}

int main(void) {	
	cout << "=================" << endl;
	cout << "Выберите задание:" << endl;
	cout << "1 - LU-разложение" << endl;
	cout << "2 - Метод прогонки (не реализовано)" << endl;
	cout << "3 - Метод простых итераций / Метод Зейделя (не реализовано)" << endl;
	cout << "4 - Метод вращений (не реализовано)" << endl;
	cout << "5 - QR-разложение (не реализовано)" << endl;
	cout << "=================" << endl;

	cout << "Ваш выбор: ";
	size_t selection = 0;
	size_t in_selection = 0;
	cin >> selection;
	cout << "=================" << endl;

	cout << "Имя файла входных данных:" << endl;
	string filename = "";
	cin >> filename;

	switch (selection) {
		case 1:
			LUSolve(filename);
			break;
		case 2:
			RunSolve(filename);
			break;
		case 3:
			cout << "Выберите метод:" << endl;
			cout << "1 - Метод простых итераций" << endl;
			cout << "2 - Метод Зейделя" << endl;
			cin >> in_selection;
			switch (in_selection) {
				case 1:
					BasicIterationSolve(filename);
					break;
				case 2:
					break;
					ZeidelSolve(filename);
				default:
					cout << INCORRECT_SELECTION << endl;
					break;
			}
			break;
		case 4:
			RotateSolve(filename);
			break;
		case 5:
			QRSolve(filename);
			break;
		default:
			cout << INCORRECT_SELECTION << endl;
			break;
	}
	return 0;
}
