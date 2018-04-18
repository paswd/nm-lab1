/*#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "complex.h"

#define STR_SIZE_BASIC 1024*/
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "complex.h"

TNum d_abs(TNum num)
{
	if (num < 0.)
		num = -num;
	return num;
}

TComplex::TComplex(void) {
	this->Re = 0;
	this->Im = 0;
}
TComplex::TComplex(TNum re, TNum im) {
	this->Re = re;
	this->Im = im;
}
TComplex::TComplex(TNum num) {
	this->Re = num;
	this->Im = 0;
}
TComplex::~TComplex(void) {}

void TComplex::Print(void) {
	if (this->IsZero()) {
		cout << 0 << endl;
		return;
	}
	if (this->Re != 0) {
		cout << this->Re;
	}
	if (this->Im == 0) {
		return;
	}
	if (this->Im > 0 && this->Re != 0) {
		cout << "+";
	}
	if (this->Im != 0) {
		cout << this->Im;
	}
	cout << endl;
}

bool TComplex::IsZero(void) {
	return this->Re == 0 && this->Im == 0;
}


TComplex ComplexSumm(TComplex num1, TComplex num2)
{
	TComplex res;
	res.Re = num1.Re + num2.Re;
	res.Im = num1.Im + num2.Im;
	return res;
}
TComplex ComplexComposition(TComplex num1, TComplex num2)
{
	TComplex res;
	res.Re = num1.Re * num1.Im - num2.Re * num2.Im;
	res.Im = num1.Re * num2.Im + num2.Re * num1.Im;
	return res;
}
TComplex ComplexQuotient(TComplex num1, TComplex num2)
{
	TComplex res;
	res.Re = (num1.Re * num2.Re + num1.Im * num2.Im) / (num2.Re * num2.Re + num2.Im * num2.Im);
	res.Im = (num2.Re * num1.Im - num2.Im * num2.Re) / (num2.Re * num2.Re + num2.Im * num2.Im);
	return res;
}

TNum ComplexDistance(TComplex num1, TComplex num2)
{
	return sqrt((num1.Re - num2.Re) * (num1.Re - num2.Re) + (num1.Im - num2.Im) * (num1.Im - num2.Im)); 
}
