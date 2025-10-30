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
	SimplexMethodCalculator();
	// возвращает ту задачу, которую будет решать (это может быть вспомогательная задача)
	LinearTask setLinearTask(const LinearTask &linearTask);
	
	bool oneCalcStep();
	bool continueLikeMainTask();
	Row &F();

	void removeVariableIndex(const LinearTask::VariableIndex variablesIndex);

	bool supportive() const;

	friend void writeCalculator(const std::string &fileName, const SimplexMethodCalculator &calculator, bool append);
	friend void writeCalculatorResult(const std::string &fileName, const SimplexMethodCalculator &calculator, const LinearTask::VariableIndex startVariablesCount, const bool reversed, bool append);

private:
	LinearTask m_startTask;
	LinearTask::Basis m_basis;
	CoefVector m_mainTargetFunctionCoefs;
	std::vector<std::string> m_variablesNames;			// имена переменных
	std::vector<Row> m_rows;				// вся таблица, последния строчка - F
	bool m_supportive = false;
};