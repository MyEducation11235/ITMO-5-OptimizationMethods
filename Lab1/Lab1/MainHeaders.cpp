#include "MainHeaders.h"

float epsilon = 0.01f;

bool eq(Coef a, Coef b)
{
	return std::fabs(a - b) < epsilon;
}

bool ne(Coef a, Coef b) {
	return !eq(a, b);
}

bool gt(Coef a, Coef b) {
	return lt(b, a);
}

bool ge(Coef a, Coef b) {
	return le(b, a);
}

bool lt(Coef a, Coef b)
{
	return (b - a) > epsilon;  // b действительно больше a
}

bool le(Coef a, Coef b)
{
	return a < b || eq(a, b);
}
