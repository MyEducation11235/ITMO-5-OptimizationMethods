#include "LinearTask.h"

LinearTask::LinearTask(std::vector<std::string> &&variablesNames) : m_variablesNames(std::move(variablesNames))
{
}

std::string LinearTask::nextVariableName() const
{
	int index = variablesCount();
	std::string nextName;
	do {
		++index;
		nextName = "x" + std::to_string(index);
	} while (std::find(m_variablesNames.begin(), m_variablesNames.end(), nextName) != m_variablesNames.end());
	return nextName;
}

const std::vector<std::string> &LinearTask::variablesNames() const
{
	return m_variablesNames;
}

void LinearTask::equalizedCondition(const ConditionIndex equalizedCondIndex)
{
	LinearCondition &cond = m_conds[equalizedCondIndex];
	Coef newCoef = 0;
	if (cond.cond & LinearCondition::gt)
		--newCoef;
	if (cond.cond & LinearCondition::lt)
		++newCoef;

	if (newCoef == 0)
		return;

	cond.cond = LinearCondition::eq;
	addVariableInCond(equalizedCondIndex, newCoef);
}

void LinearTask::equalizedConditions()
{
	for (ConditionIndex i = 0; i < conditionCount(); i++)
		equalizedCondition(i);
}

void LinearTask::addVariableInCond(const size_t equalizedCondIndex, const Coef value)
{
	m_variablesNames.push_back(nextVariableName());
	m_targetFunctionCoefs.push_back(0);
	for (auto &el : m_conds)
		el.coefs.push_back(0);

	m_conds[equalizedCondIndex].coefs.back() = value;
}

LinearTask::Basis LinearTask::currentBasis() const
{
	Basis res;
	for (VariableIndex i = 0; i < variablesCount(); i++)
	{
		ConditionIndex condIndex = condIndexWhereVariableIsBasis(i);
		if (condIndex != INVALID_INDEX)
			res[condIndex] = i;
	}
	return res;
}

LinearTask::ConditionIndex LinearTask::condIndexWhereVariableIsBasis(const VariableIndex variableIndex) const
{
	ConditionIndex res = INVALID_INDEX;
	for (ConditionIndex row = 0; row < conditionCount(); row++)
	{
		if (!eq(m_conds[row].coefs[variableIndex], 0)) {
			/*if (!eq(m_conds[row].coefs[variableIndex], 1))
				return INVALID_INDEX;*/

			if (res != INVALID_INDEX) {
				return INVALID_INDEX; // встречается в двух строках - переменная не базисная
			}
			res = row;
		}
	}
	return res;
}

const CoefVector &LinearTask::targetFunctionCoefs() const
{
	return m_targetFunctionCoefs;
}

void LinearTask::setTargetFunctionCoef(const VariableIndex variableIndex, const Coef &value)
{
	m_targetFunctionCoefs[variableIndex] = value;
}

void LinearTask::setTargetFunctionCoefs(const CoefVector &value)
{
	if (isRightLen(value.size()))
		m_targetFunctionCoefs = value;
}

bool LinearTask::strivesForMin() const
{
	return m_strivesForMin;
}

void LinearTask::setStrivesForMin(bool value)
{
	m_strivesForMin = value;
}

void LinearTask::reverseStrives()
{
	setStrivesForMin(!m_strivesForMin);
	for (Coef &coef : m_targetFunctionCoefs) {
		coef *= -1;
	}
}

const std::vector<LinearCondition> &LinearTask::conds() const
{
	return m_conds;
}

void LinearTask::setConds(const std::vector<LinearCondition> &value)
{
	for (const LinearCondition &cond : value)
		addCond(cond);
}

void LinearTask::addCond(const LinearCondition &value)
{
	if (isRightLen(value.coefs.size()) && value.cond != LinearCondition::Unknown)
		m_conds.emplace_back(value);
}

void LinearTask::addCond(LinearCondition &&value)
{
	if (isRightLen(value.coefs.size()) && value.cond != LinearCondition::Unknown)
		m_conds.emplace_back(std::move(value));
}

LinearTask::ConditionIndex LinearTask::conditionCount() const
{
	return m_conds.size();
}

LinearTask::VariableIndex LinearTask::variablesCount() const
{
	return m_variablesNames.size();
}

bool LinearTask::isRightLen(int len) const
{
	return variablesCount() == len;
}
