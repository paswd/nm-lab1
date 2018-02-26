#ifndef MATRIX_H
#define MATRIX_H

#include "types.h"
#include <iostream>
#include <vector>

class TMatrix {
private:
	bool InitStatus;
	size_t Height;
	size_t Wigth;

	TNum **Values;

public:
	TMatrix(void);
	TMatrix(TMatrix *sample);
	TMatrix(size_t h, size_t w);
	~TMatrix(void);
	void Init(TMatrix *sample, size_t h, size_t w);
	void Clear(void);
	void Get(void);
	void Print(void);
	TNum GetValue(size_t i, size_t j);
	void SetValue(TNum value, size_t i, size_t j);
	size_t GetWidth(void);
	size_t GetHeight(void);
	void Resize(size_t h, size_t w);
	bool IsNull(void);
	bool ReadFromFile(std::string filename);
};

TMatrix SubMatrix(TMatrix matrix, size_t i, size_t j, size_t h, size_t w);
TMatrix MatrixComposition(TMatrix m1, TMatrix m2);

TMatrix LU(TMatrix matrix);

#endif //MATRIX_H
