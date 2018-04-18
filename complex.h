#ifndef COMPLEX_H
#define COMPLEX_H

#include "types.h"

/*bool is_space(char sym);

int CharToInt(char sym);
int StrToInt(char *str, int start);
double d_abs(double num);

//typedef double complex Item;
typedef double Element;

typedef struct {
	Element re;
	Element im;
} Complex;

Complex *complex_create();
void complex_destroy(Complex **num);

Complex complex_summ(Complex num1, Complex num2);
Complex complex_composition(Complex num1, Complex num2);
Complex complex_quotient(Complex num1, Complex num2);
double complex_distance(Complex num1, Complex num2);
void complex_print(Complex num);

Complex complex_set_value(char *str, int start);
bool complex_is_zero(Complex num);*/

class TComplex {
public:
	TNum Re;
	TNum Im;

	TComplex(void);
	TComplex(TNum re, TNum im);
	TComplex(TNum num);
	~TComplex(void);

	void Print(void);
	bool IsZero(void);
}

TComplex ComplexSumm(TComplex num1, TComplex num2);
TComplex ComplexComposition(TComplex num1, TComplex num2);
TComplex ComplexQuotient(TComplex num1, TComplex num2);
TNum ComplexDistance(TComplex num1, TComplex num2);


#endif //COMPLEX_H