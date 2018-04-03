#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include "types.h"
#include "matrix.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";
const TNum EPS = (TNum) std::numeric_limits<TNum>::epsilon() * 10000000.;

vector <TNum> LUSolveFunc(TMatrix *lu, vector <TNum> b);

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
	TMatrix lu;
	LU(&matrix, &lu);

	vector <TNum> x = LUSolveFunc(&lu, b);

	cout << "Решение СЛАУ:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
	//cout << "POINT11" << endl;
	TNum det = 1.;
	for (size_t i = 0; i < size; i++) {
		det *= lu.GetValue(i, i);
	}

	cout << "Определитель: " << det << endl;

	//lu.Print();
	cout << "Обратная матрица" << endl;

	vector <vector <TNum>> e_matrix;
	e_matrix.resize(size);
	for (size_t i = 0; i < size; i++) {
		e_matrix[i].resize(size);
		for (size_t j = 0; j < size; j++) {
			if (i == j) {
				e_matrix[i][j] = 1;
			} else {
				e_matrix[i][j] = 0;
			}
		}
	}
	TMatrix back(size, size);
	for (size_t i = 0; i < size; i++) {
		vector <TNum> tmp = LUSolveFunc(&lu, e_matrix[i]);
		for (size_t j = 0; j < size; j++) {
			back.SetValue(tmp[j], j, i);
		}
	}
	back.Print();

	cout << endl << "Проверка:" << endl;
	TMatrix cmp;
	MatrixComposition(&matrix, &back, &cmp);
	cmp.Print();
}

vector <TNum> LUSolveFunc(TMatrix *lu, vector <TNum> b) {

	
	//cout << "POINT6" << endl;
	
	//cout << "POINT7" << endl;
	if (lu->IsNull()) {
		cout << "Error LU" << endl;
		vector <TNum> tmp(0);
		return tmp;
	}
	//cout << "POINT8" << endl;
	size_t size = b.size();

	vector <TNum> y(size, 0);
	for (size_t i = 0; i < size; i++) {
		y[i] = b[i];
		for (size_t j = 0; j < i; j++) {
			y[i] -= y[j] * lu->GetValue(i, j);
		}
	}
	//cout << "POINT9" << endl;

	vector <TNum> x(size, 0);
	for (size_t it = b.size(); it > 0; it--) {
		size_t i = it - 1;
		x[i] = y[i];
		for (size_t j = it; j < size; j++) {
			x[i] -= x[j] * lu->GetValue(i, j);
		}
		x[i] /= lu->GetValue(i, i);
	}
	//cout << "POINT10" << endl;
	return x;
}

void RunSolve(string filename) {
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t n;
	fin >> n;
	//cout << "n = " << n << endl;
	vector <TNum> a(n);
	vector <TNum> b(n);
	vector <TNum> c(n);
	vector <TNum> d(n);
	for (size_t i = 0; i < n; i++) {
		//cout << i << endl;
		if (i > 0) {
			fin >> a[i - 1];
			cout << a[i - 1] << " ";
		}
		fin >> b[i];
		cout << b[i] << " ";
		//cout << "b[" << i << "] = " << b[i] << endl;
		if (i < n - 1) {
			fin >> c[i];
			cout << c[i] << " ";
			//cout << "c[" << i << "] = " << c[i] << endl;
		}
		fin >> d[i];
		cout << d[i] << endl;
		//cout << "d[" << i << "] = " << d[i] << endl;
	}
	fin.close();
	cout << endl;
	/*vector <TNum> cn(n);
	vector <TNum> dn(n);
	cn[0] = c[0] / b[0];
	dn[0] = d[0] / b[0];
	for (size_t i = 1; i < n; i++) {
		TNum tmp = 1 / (b[i] - cn[i - 1] * a[i]);
		cn[i] = c[i] * tmp;
		dn[i] = (d[i] - dn[i - 1] * a[i]) * tmp;
		//cout << "dn[" << i << "] = " << dn[i] << endl;
	}
	vector <TNum> x(n);
	x[n - 1] = dn[n - 1];
	for (size_t in = n - 1; in > 0; in--) {
		size_t i = in - 1;
		x[i] = dn[i] - cn[i] * x[i + 1];
	}*/

	c[0] /= b[0];
	d[0] /= b[0];
	for (size_t i = 1; i < n; i++){
		TNum tmp = 1 / (b[i] - c[i-1] * a[i]);
		c[i] *= tmp;
		d[i] = (d[i] - d[i-1] * a[i]) * tmp;
	}
 
	vector <TNum> x(n);
	x[n - 1] = d[n - 1];
	for (size_t in = n - 1; in > 0; in--) {
		size_t i = in - 1;
		x[i] = d[i] - c[i] * x[i + 1];
	}
	for (size_t i = 0; i < n; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
	//cout << filename << endl;
}

void BasicIterationSolve(string filename) {
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
	
	vector <TNum> x(size, 1);
	
	size_t n;
	vector <TNum> e(size);
	vector <TNum> d(size);
	bool is_set_first = true;
	//bool is_continue = true;

	while (true) {
		if (is_set_first) {
			n = 1;
		}
		is_set_first = true;
		vector <TNum> xn(size);
		for (size_t i = 0; i < size; i++) {
			e[i] = 0.;
			for (size_t j = 0; j < size; j++) {
				e[i] += matrix.GetValue(i, j) * x[j];
			}
			e[i] -= b[i];
			d[i] = e[i] / matrix.GetValue(i, i);
			xn[i] = x[i] - d[i];

		}
		n++;
		if (n <= size) {
			is_set_first = false;
			continue;
		}
		for (size_t i = 0; i < size; i++) {
			x[i] = xn[i];
		}
		TNum max_d = 0;
		bool max_empty = true;
		for (size_t i = 0; i < size; i++) {
			TNum dn = abs(d[i]);
			if (dn > max_d || max_empty) {
				max_d = dn;
				max_empty = false;
			}
		}
		//cout << max_d << endl;
		if (max_d < EPS) {
			break;
		}
	}
	cout << "Решение:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
}

void ZeidelSolve(string filename) {
	//cout << "ZEIDEL" << endl;
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
	
	vector <TNum> x(size, 1);
	
	size_t n;
	vector <TNum> e(size);
	vector <TNum> d(size);
	bool is_set_first = true;
	//bool is_continue = true;
	vector <TNum> xn(size, 1);

	while (true) {
		if (is_set_first) {
			n = 1;
		}
		is_set_first = true;
		
		for (size_t i = 0; i < size; i++) {
			e[i] = 0.;
			for (size_t j = 0; j < size; j++) {
				if (j < i) {
					e[i] += matrix.GetValue(i, j) * xn[j];
				} else {
					e[i] += matrix.GetValue(i, j) * x[j];
				}
			}
			e[i] -= b[i];
			d[i] = e[i] / matrix.GetValue(i, i);
			xn[i] = x[i] - d[i];

		}
		n++;
		if (n <= size) {
			is_set_first = false;
			continue;
		}
		for (size_t i = 0; i < size; i++) {
			x[i] = xn[i];
		}
		TNum max_d = 0;
		bool max_empty = true;
		for (size_t i = 0; i < size; i++) {
			TNum dn = abs(d[i]);
			if (dn > max_d || max_empty) {
				max_d = dn;
				max_empty = false;
			}
		}
		//cout << max_d << endl;
		if (max_d < EPS) {
			break;
		}
	}
	cout << "Решение:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
}

void RotateSolve(string filename) {
	cout << filename << endl;
}

void QRSolve(string filename) {
	cout << filename << endl;
}

int main(void) {
	//cout << "EPS = " << EPS << endl;
	cout << "=================" << endl;
	cout << "Выберите задание:" << endl;
	cout << "1 - LU-разложение" << endl;
	cout << "2 - Метод прогонки" << endl;
	cout << "3 - Метод простых итераций / Метод Зейделя" << endl;
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
			cout << "2 - Метод Зейделя (не реализовано)" << endl;
			cin >> in_selection;
			switch (in_selection) {
				case 1:
					BasicIterationSolve(filename);
					break;
				case 2:
					ZeidelSolve(filename);
					break;
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
