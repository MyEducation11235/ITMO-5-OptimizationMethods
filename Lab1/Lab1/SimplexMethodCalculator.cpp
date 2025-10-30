#include "SimplexMethodCalculator.h"

SimplexMethodCalculator::SimplexMethodCalculator()
{
}

LinearTask SimplexMethodCalculator::setLinearTask(const LinearTask &linearTask)
{
	LinearTask::Basis basis = linearTask.currentBasis();
	// требуется ли вспомогательная задача
	m_supportive = basis.size() < linearTask.conditionCount();
	m_mainTargetFunctionCoefs = linearTask.targetFunctionCoefs();

	LinearTask supportiveTask = linearTask;
	if (m_supportive) {
		supportiveTask.setStrivesForMin(true);
		supportiveTask.setTargetFunctionCoefs(CoefVector(supportiveTask.variablesCount(), 0));

		for (LinearTask::ConditionIndex i = 0; i < supportiveTask.conditionCount(); i++)
		{
			const auto found = basis.find(i);
			// нужно добавить переменную в эту строку
			if (found == basis.end()) {
				supportiveTask.addVariableInCond(i);
				supportiveTask.setTargetFunctionCoef(supportiveTask.variablesCount() - 1, 1);
			}
		}
	}

	m_variablesNames = supportiveTask.variablesNames();
	m_basis = supportiveTask.currentBasis();
	for (const auto &cond : supportiveTask.conds()) {
		m_rows.push_back(Row{});
		for (const auto coef : cond.coefs) {
			m_rows.back().coefs.push_back(coef);
		}
		m_rows.back().coefs.push_back(cond.target);
	}
	m_rows.push_back(Row{});
	if (m_supportive) {
		m_rows.back().coefs.resize(supportiveTask.variablesCount() + 1, 0);
		for (LinearTask::ConditionIndex j = 0; j < supportiveTask.conditionCount(); j++)
		{
			m_rows.back() -= m_rows[j];
		}
	}
	else {
		m_rows.back().coefs = m_mainTargetFunctionCoefs;
		m_rows.back().coefs.push_back(0); // b
	}

	return supportiveTask;
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

bool SimplexMethodCalculator::continueLikeMainTask()
{
	if (!m_supportive)
		return false;
	m_supportive = false;

	std::set<LinearTask::ConditionIndex> ignor;
	for (LinearTask::ConditionIndex rowIndex = 0; rowIndex < m_rows.size() - 1; rowIndex++) {
		ignor.insert(m_basis[rowIndex]);
	}

	for (LinearTask::VariableIndex i = F().size() - 2; i >= m_mainTargetFunctionCoefs.size(); --i)
		removeVariableIndex(i);
	for (const auto el : ignor) {
		if (el >= m_variablesNames.size())
			return false;
	}

	for (LinearTask::VariableIndex i = 0; i < F().size(); i++)
	{
		if (ignor.contains(i))
			continue;

		Coef res = 0;
		if (i < m_mainTargetFunctionCoefs.size())
			res = m_mainTargetFunctionCoefs[i];
		for (LinearTask::ConditionIndex rowIndex = 0; rowIndex < m_rows.size() - 1; rowIndex++) {
			if (m_basis[rowIndex] < m_mainTargetFunctionCoefs.size())
				res -= m_mainTargetFunctionCoefs[m_basis[rowIndex]] * m_rows[rowIndex].coefs[i];
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
	m_variablesNames.erase(m_variablesNames.begin() + variablesIndex);
	for (auto& row : m_rows)
		row.coefs.erase(row.coefs.begin() + variablesIndex);
}

bool SimplexMethodCalculator::supportive() const
{
	return m_supportive;
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
