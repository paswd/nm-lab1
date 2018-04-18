#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <complex>
#include "types.h"
#include "matrix.h"

using namespace std;

const string INCORRECT_SELECTION = "Неверная опция";

/*
===============
BASIC FUNCTIONS
===============
*/

TNum TSign(TNum num) {
	if (num > 0) {
		return 1.;
	}
	if (num < 0) {
		return -1.;
	}
	return 0.;
}

pair<complex <TNum>, complex <TNum>> SqEquRoot(TNum a, TNum b, TNum c) {
	//cout << "SQ::Begin" << endl;
	//cout << a << " " << b << " " << c << endl;
	complex <TNum> d = b * b - 4 * a * c;
	//cout << "d = " << d << endl;
	complex <TNum> x1, x2;
	x1 = (-b + sqrt(d)) / (2 * a);
	x2 = (-b - sqrt(d)) / (2 * a);
	//cout << "x1 = " << x1 << endl;
	//cout << "x2 = " << x2 << endl;
	//cout << "SQ::End" << endl;

	return make_pair(x1, x2);
}

/*
=======
METHODS
=======
*/

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
	try {
		bool readres = matrix.ReadFromFile(fin, true);

		if (!readres) {
			throw 1;
		}

		size = matrix.GetHeight();
		b.resize(size);
		

		for (size_t i = 0; i < size; i++) {
			if (!(fin >> b[i])) {
				throw 1;
			}
		}

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

	fin.close();
	TMatrix lu;
	LU(&matrix, &lu);

	vector <TNum> x = LUSolveFunc(&lu, b);

	cout << "Решение СЛАУ:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
	TNum det = 1.;
	for (size_t i = 0; i < size; i++) {
		det *= lu.GetValue(i, i);
	}

	cout << "Определитель: " << det << endl;
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
	if (lu->IsNull()) {
		cout << "Error LU" << endl;
		vector <TNum> tmp(0);
		return tmp;
	}
	size_t size = b.size();

	vector <TNum> y(size, 0);
	for (size_t i = 0; i < size; i++) {
		y[i] = b[i];
		for (size_t j = 0; j < i; j++) {
			y[i] -= y[j] * lu->GetValue(i, j);
		}
	}

	vector <TNum> x(size, 0);
	for (size_t it = b.size(); it > 0; it--) {
		size_t i = it - 1;
		x[i] = y[i];
		for (size_t j = it; j < size; j++) {
			x[i] -= x[j] * lu->GetValue(i, j);
		}
		x[i] /= lu->GetValue(i, i);
	}
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
	vector <TNum> a(n);
	vector <TNum> b(n);
	vector <TNum> c(n);
	vector <TNum> d(n);
	for (size_t i = 0; i < n; i++) {
		if (i > 0) {
			fin >> a[i - 1];
			cout << a[i - 1] << " ";
		}
		fin >> b[i];
		cout << b[i] << " ";
		if (i < n - 1) {
			fin >> c[i];
			cout << c[i] << " ";
		}
		fin >> d[i];
		cout << d[i] << endl;
	}
	fin.close();
	cout << endl;

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
}

vector <TNum> BasicIterationFunc(TMatrix *matrix, vector <TNum> b);

void BasicIterationSolve(string filename) {
	TMatrix matrix;
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t size = 0;
	vector <TNum> b;
	try {
		bool readres = matrix.ReadFromFile(fin, true);

		if (!readres) {
			throw 1;
		}

		size = matrix.GetHeight();
		b.resize(size);

		for (size_t i = 0; i < size; i++) {
			if (!(fin >> b[i])) {
				throw 1;
			}
		}

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

	fin.close();
	
	vector <TNum> x = BasicIterationFunc(&matrix, b);
	cout << "Решение:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
}

vector <TNum> BasicIterationFunc(TMatrix *matrix, vector <TNum> b) {
	size_t size = b.size();
	vector <TNum> x(size, 1);
	size_t n;
	vector <TNum> e(size);
	vector <TNum> d(size);
	bool is_set_first = true;

	size_t iter = 0;
	while (true) {
		iter++;
		if (is_set_first) {
			n = 1;
		}
		is_set_first = true;
		vector <TNum> xn(size);
		for (size_t i = 0; i < size; i++) {
			e[i] = 0.;
			for (size_t j = 0; j < size; j++) {
				e[i] += matrix->GetValue(i, j) * x[j];
			}
			e[i] -= b[i];
			d[i] = e[i] / matrix->GetValue(i, i);
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
		if (max_d < EPS) {
			break;
		}
	}
	cout << "Число итераций: " << iter << endl;
	return x;
}

vector <TNum> ZeidelFunc(TMatrix *matrix, vector <TNum> b);

void ZeidelSolve(string filename) {
	TMatrix matrix;
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t size = 0;
	vector <TNum> b;
	try {
		bool readres = matrix.ReadFromFile(fin, true);

		if (!readres) {
			throw 1;
		}

		size = matrix.GetHeight();
		b.resize(size);		

		for (size_t i = 0; i < size; i++) {
			if (!(fin >> b[i])) {
				throw 1;
			}
		}

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

	fin.close();
	vector <TNum> x = ZeidelFunc(&matrix, b);
	cout << "Решение:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "X" << i + 1 << " = " << x[i] << endl;
	}
}
vector <TNum> ZeidelFunc(TMatrix *matrix, vector <TNum> b) {
	size_t size = b.size();
	vector <TNum> x(size, 1);
	size_t n;
	vector <TNum> e(size);
	vector <TNum> d(size);
	bool is_set_first = true;
	vector <TNum> xn(size, 1);

	size_t iter = 0;
	while (true) {
		iter++;
		if (is_set_first) {
			n = 1;
		}
		is_set_first = true;
		
		for (size_t i = 0; i < size; i++) {
			e[i] = 0.;
			for (size_t j = 0; j < size; j++) {
				if (j < i) {
					e[i] += matrix->GetValue(i, j) * xn[j];
				} else {
					e[i] += matrix->GetValue(i, j) * x[j];
				}
			}
			e[i] -= b[i];
			d[i] = e[i] / matrix->GetValue(i, i);
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
		if (max_d < EPS) {
			break;
		}
	}
	cout << "Число итераций: " << iter << endl;
	return x;
}

void JakobiRotation(TMatrix *matrix, TMatrix *rot, size_t i, size_t j, TNum accuracy);

void RotateSolve(string filename) {
	TMatrix matrix;
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t size = 0;
	TNum accuracy = EPS;
	try {
		bool readres = matrix.ReadFromFile(fin, true);

		if (!readres) {
			throw 1;
		}

		size = matrix.GetHeight();
		if (!(fin >> accuracy)) {
			throw 1;
		}

	} catch (int a) {
		fin.close();
		cout << "Ошибка чтения из файла" << endl;
		return;
	}
	cout << "Точность успешно импортирована:" << endl;
	cout << accuracy << endl << endl;

	fin.close();

	TMatrix origin(&matrix);
	//size_t ccc = 0;
	TMatrix eigen;
	eigen.SetUnit(size);

	while (true) {
		size_t h = 0;
		size_t w = 0;
		TNum max = 0.;
		bool is_max_empty = true;

		for (size_t i = 0; i < size - 1; i++) {
			for (size_t j = i + 1; j < size; j++) {
				if (i == j) {
					continue;
				}
				TNum tmp = abs(matrix.GetValue(i, j));
				//cout << tmp << ":" << max << endl;
				if (tmp > max || is_max_empty) {
					max = tmp;
					h = i;
					w = j;
					is_max_empty = false;
					//cout << "changed" << endl;
				}
				//JakobiRotation(&matrix, i, j, accuracy);
			}
		}
		if (!is_max_empty) {
			TMatrix rot;
			JakobiRotation(&matrix, &rot, h, w, accuracy);
			TMatrix tmp_matr;
			MatrixComposition(&eigen, &rot, &tmp_matr);
			eigen.Init(&tmp_matr, size, size);
		}
		TNum summ = 0.;
		for (size_t i = 0; i < size - 1; i++) {
			for (size_t j = i + 1; j < size; j++) {
				if (i == j) {
					continue;
				}
				TNum tmp = matrix.GetValue(i, j);
				summ += tmp * tmp;
			}
		}
		summ = sqrt(summ);
		//cout << "summ = " << summ << endl;
		if (summ < accuracy) {
			break;
		}
	}
	vector <TNum> l(size);
	for (size_t i = 0; i < size; i++) {
		l[i] = matrix.GetValue(i, i);
	}
	for (size_t i = 0; i < size; i++) {
		cout << "l" << i + 1 << " = " << l[i] << endl;
		cout << "Собственный вектор для l" << i + 1 << ":" << endl;
		for (size_t j = 0; j < size; j++) {
			cout << eigen.GetValue(j, i) << " ";
		}
		cout << endl << endl;
	}
}

void JakobiRotation(TMatrix *matrix, TMatrix *rot, size_t i, size_t j, TNum accuracy) {
	if (abs(matrix->GetValue(i, j)) < accuracy) {
		return;
	}
	TNum size = matrix->GetHeight();
	TNum tsin, tcos;
	if (matrix->GetValue(i, i) == matrix->GetValue(j, j)) {
		TNum theta = PI / 4.;
		tsin = sin(theta);
		tcos = cos(theta);
	} else {
		TNum tmp = 2 * matrix->GetValue(i, j) / (matrix->GetValue(i, i) - matrix->GetValue(j, j));
		TNum theta = .5 * atan(tmp);
		tsin = sin(theta);
		tcos = cos(theta);
	}
	rot->SetUnit(size);
	rot->SetValue(tcos, i, i);
	rot->SetValue(tcos, j, j);
	rot->SetValue(tsin, j, i);
	rot->SetValue(-tsin, i, j);

	TMatrix rot_t(rot);
	rot_t.Transpose();
	TMatrix m_tmp1, m_tmp2;
	MatrixComposition(&rot_t, matrix, &m_tmp1);
	MatrixComposition(&m_tmp1, rot, &m_tmp2);
	matrix->Init(&m_tmp2, size, size);
	//matrix->Print();
	//cout << endl;
}

vector <complex <TNum>> QRFunc(TMatrix *matrix, TNum accuracy);

void QRSolve(string filename) {
	TMatrix matrix;
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
        cout << "Ошибка чтения файла" << endl;
        return;
	}
	size_t size = 0;
	TNum accuracy = EPS;
	try {
		bool readres = matrix.ReadFromFile(fin, true);

		if (!readres) {
			throw 1;
		}

		size = matrix.GetHeight();
		if (!(fin >> accuracy)) {
			throw 1;
		}

	} catch (int a) {
		fin.close();
		cout << "Ошибка чтения из файла" << endl;
		return;
	}
	cout << "Точность успешно импортирована:" << endl;
	cout << accuracy << endl << endl;

	fin.close();

	vector <complex <TNum>> l = QRFunc(&matrix, accuracy);

	cout << "Результат:" << endl;
	for (size_t i = 0; i < size; i++) {
		cout << "l" << i + 1 << " = " << l[i] << endl;
	}
}

void QRDecomposition(TMatrix *matrix, TMatrix *q, TMatrix *r) {
	size_t size = matrix->GetHeight();
	r->Init(matrix, size, size);
	q->SetUnit(size);
	TMatrix v, h;
	//int cnt = 0;
	for (size_t i = 0; i < size - 1; i++) {
		//cout << "CNT: " << cnt << endl;
		//cnt++;
		//V start
		v.Init(NULL, size, 1);
		TNum summ = 0.;
		for (size_t j = 0; j < size; j++) {
			if (j < i) {
				v.SetValue(0., j, 0);
			} else {
				TNum tmp = r->GetValue(j, i);
				v.SetValue(tmp, j, 0);
				summ += tmp * tmp;
			}
		}
		TNum vect_tmp = v.GetValue(i, 0) + copysign(sqrt(summ), r->GetValue(i, i));
		v.SetValue(vect_tmp, i, 0);
		//V end
		//v.Print();//

		//H begin

		TMatrix vt(&v);
		vt.Transpose();
		//vt.Print();

		TMatrix znam_matrix;
		MatrixComposition(&vt, &v, &znam_matrix);
		//znam_matrix.Print();
		//cout << endl;
		TNum koeff = znam_matrix.GetValue(0, 0);

		koeff = 2. / koeff;
		//cout << "koeff = " << koeff << endl;
		TMatrix vm;
		MatrixComposition(&v, &vt, &vm);
		//vm.Print();
		//cout << endl;
		vm.MultiplyNum(koeff);
		//vm.Print();
		//cout << endl;
		vm.Minus();
		//vm.Print();
		//cout << endl;
		h.SetUnit(size);
		h.Summ(&vm);
		//H end
		//h.Print();//

		TMatrix m_tmp;
		MatrixComposition(q, &h, &m_tmp);
		q->Init(&m_tmp, size, size);
		MatrixComposition(&h, r, &m_tmp);
		r->Init(&m_tmp, size, size);
		//q->Print();
		//r->Print();
		//cout << "3 3" << endl;
		//q->Print();
		//cout << endl << endl;
		//cout << "3 3" << endl;
		//r->Print();
		//cout << endl << "===" << endl;
		//cout << "===" << endl;
	}
}

bool QRIsContinueIteration(TMatrix *matrix, TNum accuracy) {
	size_t size = matrix->GetHeight();
	TNum summ = 0.;

	for (size_t i = 0; i < size - 1; i++) {
		for (size_t j = i + 2; j < size; j++) {
			TNum tmp = matrix->GetValue(i, j);
			summ += tmp * tmp;
		}
	}
	//cout << "SUMM = " << summ << endl;
	//cout << "SQRT = " << sqrt(summ) << endl;
	return sqrt(summ) > accuracy;
}

vector <TNum> QRGetV(TMatrix *matrix, size_t idx) {
	size_t size = matrix->GetHeight();
	vector <TNum> res(size, 1.);
	TNum summ = 0.;

	for (size_t i = 0; i < size; i++) {
		if (i < idx) {
			res[i] = 0.;
		} else {
			TNum tmp = matrix->GetValue(i, idx);
			res[i] = tmp;
			summ += tmp * tmp;
		}
	}
	res[idx] += copysign(sqrt(summ), matrix->GetValue(idx, idx));

	return res;
}

vector <complex <TNum>> QRFunc(TMatrix *matrix, TNum accuracy) {
	TMatrix q, r;
	TMatrix ak(matrix);

	//ak.Print();
	//int cnt = 0;
	while (QRIsContinueIteration(&ak, accuracy)) {
		//cout << "CNT: " << cnt << endl;
		//cnt++;
		//cout << "TEST" << endl;
		QRDecomposition(&ak, &q, &r);
		//cout << "3 3" << endl;
		//q.Print();
		//cout << endl << endl;
		//cout << "3 3" << endl;
		//r.Print();
		//cout << endl << "===" << endl;
		//cout << endl;
		MatrixComposition(&r, &q, &ak);
	}
	//ak.Print();
	//cout << "POINT" << endl;

	size_t size = ak.GetHeight();
	vector <complex <TNum>> res(size, 1.);
	pair<complex<TNum>, complex<TNum>> root;
	//cout << "~~~" << endl;

	for (size_t i = 0; i < size; i++) {
		if (i + 1 < size && !(ak.GetValue(i + 1, i) < accuracy)) {
			root = SqEquRoot(
				1.,
				-(ak.GetValue(i, i) + ak.GetValue(i + 1, i + 1)), 
				ak.GetValue(i, i) * ak.GetValue(i + 1, i + 1) -
					ak.GetValue(i, i + 1) * ak.GetValue(i + 1, i)
			);
			//cout << root.first << endl;
			//cout << root.second << endl;
			res[i] = root.first;
			i++;
			res[i] = root.second;
		} else {
			res[i] = ak.GetValue(i, i);
			//cout << ak.GetValue(i, i) << endl;
		}
	}
	//cout << "===" << endl;

	return res;
}

int main(void) {
	cout << "=================" << endl;
	cout << "Выберите задание:" << endl;
	cout << "1 - LU-разложение" << endl;
	cout << "2 - Метод прогонки" << endl;
	cout << "3 - Метод простых итераций / Метод Зейделя" << endl;
	cout << "4 - Метод вращений" << endl;
	cout << "5 - QR-разложение" << endl;
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
