#include "LinearCondition.h"

bool LinearCondition::isFulfilled(const CoefVector &currentCoefs) const
{
	if (coefs.size() != currentCoefs.size())
		return false;

	Coef calculated = 0;
	for (size_t i = 0; i < currentCoefs.size(); i++)
	{
		calculated += coefs[i] * currentCoefs[i];
	}

	switch (cond)
	{
	case LinearCondition::eq:
		return ::eq(calculated, target);
	case LinearCondition::ne:
		return ::ne(calculated, target);
	case LinearCondition::gt:
		return ::gt(calculated, target);
	case LinearCondition::ge:
		return ::ge(calculated, target);
	case LinearCondition::lt:
		return ::lt(calculated, target);
	case LinearCondition::le:
		return ::le(calculated, target);
	case LinearCondition::Unknown:
	default:
		return false;
	}
}
