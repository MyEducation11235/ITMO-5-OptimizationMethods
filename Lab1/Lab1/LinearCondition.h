#pragma once

#include "MainHeaders.h"

struct LinearCondition
{
public:
	enum Condition : int8_t {
		Unknown = 0,
		eq = (1 << 0), // проверяет, равны ли два числа.
		ne = (~eq), // проверяет, различаются ли два числа.
		gt = (1 << 1), // проверяет, больше ли первое число, чем второе.
		ge = (1 << 1) | eq, // выводит истину, если первое число больше или равно второму.
		lt = (1 << 2), // проверяет, меньше ли первое число, чем второе.
		le = (1 << 2) | eq, // выводит истину, если первое число меньше или равно второму.
	};

	CoefVector coefs;
	Condition cond;
	Coef target = Unknown;
};

