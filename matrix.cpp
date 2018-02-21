#include "matrix.h"
#include <algorithm>

TMatrix::TMatrix(TMatrix *sample) {
	this->InitStatus = false;
	this->Init(sample, sample->GetHeight(), sample->GetWidth());
}

TMatrix::TMatrix(size_t h, size_t w) {
	this->InitStatus = false;
	this->Init(NULL, h, w);
}
TMatrix::~TMatrix(void) {
	this->Clear();
}

void TMatrix::Inint(TMatrix *sample, size_t h, size_t w) {
	if (this->InitStatus) {
		this->Clear();
	}
	this->Height = h;
	this->Width = w;
	this->Values = new TNum*[this->Height];
	for (size_t i = 0; i < this->Height; i++) {
		this->Values[i] = new TNum[this->Width];
		for (size_t j = 0; j < this->Width; j++) {
			if (sample == NULL) {
				this->Values[i][j] = 0.;
			} else {
				this->Values[i][j] = sample->GetValue(i, j);
			}
		}
	}
	this->InitStatus = true;
}
void TMatrix::Clear(void) {
	if (!this->InitStatus) {
		return;
	}
	for (size_t i = 0; i < this->Height; i++) {
		delete [] this->Values[i];
	}
	delete [] this->Values;
	this->Height = 0;
	this->Width = 0;
	this->InitStatus = false;
}

void TMatrix::Get(void) {
	for (size_t i = 0; i < this->Height; i++) {
		for (size_t j = 0; j < this->Width; j++) {
			cin >> this->Values[i][j];
		}
	}
}
void TMatrix::Print(void) {
	for (size_t i = 0; i < this->Height; i++) {
		for (size_t j = 0; j < this->Width; j++) {
			cout << this->Values[i][j] << " ";
		}
		cout << endl;
	}
}

TMum TMatrix::GetValue(size_t i, size_t j) {
	if (i >= this->Height || j >= this->Width) {
		return 0;
	}
	return this->Values[i][j];
}

void TMatrix::SetValue(TNum value, size_t i, size_t j) {
	if (i >= this->Height || j >= this->Width) {
		return;
	}
	this->Values[i][j] = value;
}
size_t TMatrix::GetWidth(void) {
	return this->Width;
}
size_t TMatrix::GetHeight(void) {
	return this->Height;
}

void TMatrix::Resize(size_t h, size_t w) {
	/*TMatrix *tmp = new TMatrix(h, w);
	for (size_t i = 0; i < min(this->Height, h); i++) {
		for (size_t j = 0; j < min(this->Width, w); j++) {
			tmp->SetValue(this->Values[i][j], i, j);
		}
	}*/
	TNum **new_values = new TNum*[h];
	for (size_t i = 0; i < h; i++) {
		new_values[i] = new TNum[w];
		for (size_t j = 0; j < w; j++) {
			if (i < this->Height && j < this->Width) {
				new_values[i][j] = this->Values[i][j]
			} else {
				this->Values[i][j] = 0.;
			}
		}
	}
	this->Clear();
	this->Values = new_values;
	this->Height = h;
	this->Width = w;
}


TMatrix SubMatrix(TMatrix matrix, size_t i, size_t j, size_t h, size_t w) {
	if (i + h > matrix.GetHeight() || j + w > matrix.GetWidth()) {
		return TMatrix(0, 0);
	}
	TMatrix new_matrix(h, w);
	for (size_t x = 0; x < h; x++) {
		for (size_t y = 0; y < w; y++) {
			new_matrix.SetValue(matrix.GetValue(x - i, y - j), x, y);
		}
	}
	return new_matrix;
}

TMatrix MatrixComposition(TMatrix m1, TMatrix m2) {
	
}

