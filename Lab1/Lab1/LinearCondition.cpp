#include "LinearCondition.h"

bool LinearCondition::isFulfilled(const CoefVector &currentCoefs) const
{
	Coef calculated = 0;
	for (size_t i = 0; i < currentCoefs.size(); i++)
	{
		calculated += coefs[i] * currentCoefs[i];
	}

	switch (cond)
	{
	case LinearCondition::eq:
		return calculated == target;
	case LinearCondition::ne:
		return calculated != target;
	case LinearCondition::gt:
		return calculated > target;
	case LinearCondition::ge:
		return calculated >= target;
	case LinearCondition::lt:
		return calculated < target;
	case LinearCondition::le:
		return calculated <= target;
	case LinearCondition::Unknown:
	default:
		return false;
	}
}
