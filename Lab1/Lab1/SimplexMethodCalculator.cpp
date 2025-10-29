#include "SimplexMethodCalculator.h"

SimplexMethodCalculator::SimplexMethodCalculator(const LinearTask &linearTask, const bool supportive)
{
	m_variablesNames = linearTask.variablesNames();
	m_basis = m_startBasis = linearTask.currentBasis();
	m_targetFunctionCoefs = linearTask.targetFunctionCoefs();
	m_supportive = supportive;
	for (const auto &cond : linearTask.conds()) {
		m_rows.push_back(Row{});
		for (const auto coef : cond.coefs) {
			m_rows.back().coefs.push_back(coef);
		}
		m_rows.back().coefs.push_back(cond.target);
	}
	m_rows.push_back(Row{});
	if (supportive) {
		m_rows.back().coefs.resize(linearTask.variablesCount() + 1, 0);
		for (LinearTask::ConditionIndex j = 0; j < linearTask.conditionCount(); j++)
		{
			m_rows.back() -= m_rows[j];
		}
	}
	else {
		m_rows.back().coefs = linearTask.targetFunctionCoefs();
		m_rows.back().coefs.push_back(0);
	}
}

bool SimplexMethodCalculator::oneCalcStep()
{
	bool finished = true;
	for (LinearTask::VariableIndex i = 0; i < F().size() - 1; i++)
	{
		if (lt(F().coefs[i], 0)) {
			finished = false;
			break;
		}
	}
	if (finished)
		return false;

	// step
	LinearTask::VariableIndex minVariantIndex = 0;
	for (LinearTask::VariableIndex i = 1; i < F().size() - 1; i++)
	{
		if (lt(F().coefs[i], F().coefs[minVariantIndex]))
			minVariantIndex = i;
	}

	LinearTask::ConditionIndex minConditionIndex = 0;
	for (LinearTask::ConditionIndex i = 1; i < m_rows.size() - 1; i++)
	{
		const Coef n = m_rows[i].coefs[minVariantIndex];
		const Coef o = m_rows[minConditionIndex].coefs[minVariantIndex];
		const Coef bn = m_rows[i].b();
		const Coef bo = m_rows[minConditionIndex].b();

		if (le(o, 0) || (lt(0, n) && lt(bn / n, bo / o)))
			minConditionIndex = i;
	}

	const Coef cellValue = m_rows[minConditionIndex].coefs[minVariantIndex];

	m_rows[minConditionIndex] /= cellValue;
	for (LinearTask::ConditionIndex i = 0; i < m_rows.size(); i++) {
		if (i == minConditionIndex)
			continue;

		m_rows[i] -= (m_rows[minConditionIndex] * m_rows[i].coefs[minVariantIndex]);
	}

	m_basis[minConditionIndex] = minVariantIndex;

	return true;
}

bool SimplexMethodCalculator::continueLikeMainTask(const LinearTask &task)
{
	if (!m_supportive)
		return false;

	m_supportive = false;


	for (LinearTask::VariableIndex i = F().size() - 2; i >= task.variablesCount(); --i)
		removeVariableIndex(i);


	std::set<LinearTask::ConditionIndex> ignor;

	for (LinearTask::ConditionIndex rowIndex = 0; rowIndex < m_rows.size() - 1; rowIndex++) {
		ignor.insert(m_basis[rowIndex]);
	}

	for (LinearTask::VariableIndex i = 0; i < F().size(); i++)
	{
		if (ignor.contains(i))
			continue;

		Coef res = 0;
		if (i < task.targetFunctionCoefs().size())
			res = task.targetFunctionCoefs()[i];
		for (LinearTask::ConditionIndex rowIndex = 0; rowIndex < m_rows.size() - 1; rowIndex++) {
			if (m_basis[rowIndex] < task.targetFunctionCoefs().size())
				res -= task.targetFunctionCoefs()[m_basis[rowIndex]] * m_rows[rowIndex].coefs[i];
		}

		F().coefs[i] = res;
	}

	return true;
}

SimplexMethodCalculator::Row &SimplexMethodCalculator::F()
{
	return m_rows.back();
}

void SimplexMethodCalculator::removeVariableIndex(const LinearTask::VariableIndex variablesIndex)
{
	m_targetFunctionCoefs.erase(m_targetFunctionCoefs.begin() + variablesIndex);
	m_variablesNames.erase(m_variablesNames.begin() + variablesIndex);
	for (auto& row : m_rows)
		row.coefs.erase(row.coefs.begin() + variablesIndex);
}

SimplexMethodCalculator::Row &SimplexMethodCalculator::Row::operator-=(const Row &other)
{
	for (size_t i = 0; i < other.size(); i++)
	{
		this->coefs[i] -= other.coefs[i];
	}
	return *this;
}

SimplexMethodCalculator::Row &SimplexMethodCalculator::Row::operator+=(const Row &other)
{
	for (size_t i = 0; i < other.size(); i++)
	{
		this->coefs[i] += other.coefs[i];
	}
	return *this;
}

SimplexMethodCalculator::Row &SimplexMethodCalculator::Row::operator/=(const Coef val)
{
	for (size_t i = 0; i < size(); i++)
	{
		this->coefs[i] /= val;
	}
	return *this;
}

SimplexMethodCalculator::Row SimplexMethodCalculator::Row::operator*(const Coef val) const
{
	Row res;
	for (size_t i = 0; i < size(); i++)
	{
		res.coefs.push_back(coefs[i] * val);
	}
	return res;
}

Coef &SimplexMethodCalculator::Row::b()
{
	return coefs.back();
}

size_t SimplexMethodCalculator::Row::size() const
{
	return coefs.size();
}
