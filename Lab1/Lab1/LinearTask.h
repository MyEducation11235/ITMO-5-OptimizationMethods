п»ї#pragma once

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
	const std::vector<std::string> m_variablesNames;	// РёРјРµРЅР° РїРµСЂРµРјРµРЅРЅС‹С…
	CoefVector m_targetFunctionCoefs;					// С†РµР»РµРІР°СЏ С„СѓРЅРєС†РёР№
	bool m_strivesForMin;								// РјРёРЅРёРјРёР·РёСЂРѕРІР°С‚СЊ?
	std::vector<LinearCondition> m_conds;				// Р»РёРЅРµР№РЅС‹Рµ РѕРіСЂР°РЅРёС‡РµРЅРёСЏ
};

