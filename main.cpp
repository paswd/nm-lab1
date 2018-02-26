#include <iostream>
#include <string>
#include "matrix.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";

void LUSolve(TMatrix matrix) {

}

void RunSolve(TMatrix matrix) {

}

void BasicIterationSolve(TMatrix matrix) {

}

void ZeidelSolve(TMatrix matrix) {

}

void RotateSolve(TMatrix matrix) {

}

void QRSolve(TMatrix matrix) {

}

int main(void) {
	cout << "Имя файла входных данных:" << endl;
	string filename = "";
	cin >> filename;
	TMatrix matrix;
	matrix.ReadFromFile(filename);
	
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

	switch (selection) {
		case 1:
			LUSolve();
			break;
		case 2:
			RunSolve();
			break;
		case 3:
			cout << "Выберите метод:" << endl;
			cout << "1 - Метод простых итераций" << endl;
			cout << "2 - Метод Зейделя" << endl;
			cin >> in_selection;
			switch (in_selection) {
				case 1:
					BasicIterationSolve();
					break;
				case 2:
					break;
					ZeidelSolve();
				default:
					cout << INCORRECT_SELECTION << endl;
					break;
			}
			break;
		case 4:
			RotateSolve();
			break;
		case 5:
			QRSolve();
			break;
		default:
			cout << INCORRECT_SELECTION << endl;
			break;
	}
	return 0;
}
