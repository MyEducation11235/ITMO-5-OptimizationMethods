#pragma once

#include <map>
#include "LinearCondition.h"

class LinearTask
{
public:
	static const size_t INVALID_INDEX = -1;
	using ConditionIndex = size_t;
	using VariableIndex = size_t;
	using Basis = std::map<ConditionIndex, VariableIndex>;
public:
	LinearTask(std::vector<std::string> &&variablesNames = {});

	std::string nextVariableName() const;

	const std::vector<std::string> &variablesNames() const;
	void equalizedCondition(const ConditionIndex equalizedCondIndex);
	void equalizedConditions();
	void addVariableInCond(const size_t equalizedCondIndex, const Coef value = 1);

	Basis currentBasis() const;
	//std::vector<> currentNotBasis() const;
	// INVALID_INDEX если такой строки нет
	ConditionIndex condIndexWhereVariableIsBasis(const VariableIndex variableIndex) const;

	const CoefVector &targetFunctionCoefs() const;
	void setTargetFunctionCoef(const VariableIndex variableIndex, const Coef &value);
	void setTargetFunctionCoefs(const CoefVector &value);

	bool strivesForMin() const;
	void setStrivesForMin(bool value);
	void reverseStrives();

	const std::vector<LinearCondition> &conds() const;
	void setConds(const std::vector<LinearCondition> &value);
	void addCond(const LinearCondition &value);
	void addCond(LinearCondition &&value);

	ConditionIndex conditionCount() const;
	VariableIndex variablesCount() const;

private:
	bool isRightLen(int len) const;

private:
	std::vector<std::string> m_variablesNames;			// имена переменных
	CoefVector m_targetFunctionCoefs;					// целевая функций
	bool m_strivesForMin = true;						// минимизировать?
	std::vector<LinearCondition> m_conds;				// линейные ограничения
};

