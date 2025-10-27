#include "LinearTask.h"

LinearTask::LinearTask(std::vector<std::string> &&variablesNames) : m_variablesNames(std::move(variablesNames))
{
}

const std::vector<std::string> &LinearTask::variablesNames() const
{
	return m_variablesNames;
}

const CoefVector &LinearTask::targetFunctionCoefs() const
{
	return m_targetFunctionCoefs;
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

bool LinearTask::isRightLen(int len) const
{
	return m_variablesNames.size() == len;
}
