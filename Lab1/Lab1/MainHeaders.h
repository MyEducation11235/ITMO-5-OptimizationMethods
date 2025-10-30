#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

using Coef = float;
using CoefVector = std::vector<Coef>;

extern float epsilon; // точность до 3 знаков

bool eq(Coef a, Coef b);

bool ne(Coef a, Coef b);

bool gt(Coef a, Coef b);

bool ge(Coef a, Coef b);

bool lt(Coef a, Coef b);

bool le(Coef a, Coef b);
