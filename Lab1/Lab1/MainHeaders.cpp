#include "MainHeaders.h"

float epsilon = 0.01f;

bool eq(Coef a, Coef b)
{
	return std::fabs(a - b) < epsilon;
}

bool lt(Coef a, Coef b)
{
	return (b - a) > epsilon;  // b действительно больше a
}

bool le(Coef a, Coef b)
{
	return a < b || eq(a, b);
}
