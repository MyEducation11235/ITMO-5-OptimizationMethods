#pragma once

#include "LinearCondition.h"

class LinearTask
{
public:
	LinearTask(std::vector<std::string> &&variablesNames);

	const std::vector<std::string> &variablesNames() const;

	const CoefVector &targetFunctionCoefs() const;
	void setTargetFunctionCoefs(const CoefVector &value);

	bool strivesForMin() const;
	void setStrivesForMin(bool value);

	const std::vector<LinearCondition> &conds() const;
	void setConds(const std::vector<LinearCondition> &value);
	void addCond(const LinearCondition &value);
	void addCond(LinearCondition &&value);

private:
	bool isRightLen(int len) const;

private:
	const std::vector<std::string> m_variablesNames;	// имена переменных
	CoefVector m_targetFunctionCoefs;					// целевая функций
	bool m_strivesForMin;								// минимизировать?
	std::vector<LinearCondition> m_conds;				// линейные ограничения
};

