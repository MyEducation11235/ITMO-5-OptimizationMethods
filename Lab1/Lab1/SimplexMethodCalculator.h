#pragma once

#include "LinearTask.h"

//void writeCalculator(const std::string &fileName, const SimplexMethodCalculator &calculator, bool append);

class SimplexMethodCalculator {
	struct Row {
		Row &operator-=(const Row &other);
		Row &operator+=(const Row &other);
		Row &operator/=(const Coef val);
		Row operator*(const Coef val) const;

		Coef &b();
		size_t size() const;

		CoefVector coefs;
	};

public:
	SimplexMethodCalculator(const LinearTask &linearTask, const bool supportive);

	bool oneCalcStep();
	bool continueLikeMainTask(const LinearTask &task);
	Row &F();

	friend void writeCalculator(const std::string &fileName, const SimplexMethodCalculator &calculator, bool append);
	friend void writeCalculatorResult(const std::string &fileName, const SimplexMethodCalculator &calculator, const LinearTask::VariableIndex startVariablesCount, bool append);

private:
	LinearTask::Basis m_basis;
	LinearTask::Basis m_startBasis;
	CoefVector m_targetFunctionCoefs;
	std::vector<std::string> m_variablesNames;			// имена переменных
	std::vector<Row> m_rows;				// вся таблица, последния строчка - F
	bool m_supportive;
};