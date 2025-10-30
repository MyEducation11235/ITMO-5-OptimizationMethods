#include <fstream>
#include <sstream>
#include <iomanip> //for std::setprecision
#include "InputOutput.h"

const std::string inputDir = "input\\";
const std::string outputDir = "output\\";

const char s_gt = '>';
const char s_lt = '<';
const char s_eq = '=';
const std::string s_ne = "!=";
const char sep = '\t';
const char stringEnd = '\n';

const std::string s_min = "min";
const std::string s_max = "max";

std::vector<std::string> split(const std::string &s, char delim = sep) {
	std::vector<std::string> res;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		res.emplace_back(std::move(item));

	return res;
}

Coef toCoef(const std::string &s) {
	return std::stof(s);
}

std::string toString(const Coef coef) {

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << coef;
	return stream.str();
	//return ,std::to_string(std::round(coef));
}

CoefVector toCoefVector(const std::vector<std::string> &sv) {
	CoefVector res;
	res.reserve(sv.size());
	for (const std::string &s : sv)
		res.emplace_back(toCoef(s));

	return res;
}

std::string toString(const CoefVector &coefs) {
	std::string resultStr;
	for (const Coef coef : coefs) {
		resultStr += toString(coef);
		resultStr.push_back(sep);
	}
	return resultStr;
}

LinearCondition::Condition toCondition(const std::string &s) {
	char l = s.front();
	const auto grltCheck = [](char c) -> LinearCondition::Condition {
		switch (c) {
		case s_gt:
			return LinearCondition::gt;
		case s_lt:
			return LinearCondition::lt;
		default:
			return LinearCondition::eq;
		}
	};

	if (s.size() == 1) {
		return grltCheck(l);
	}

	LinearCondition::Condition res = grltCheck(l);
	if (res == LinearCondition::eq)
		return LinearCondition::Condition(~res);

	return LinearCondition::Condition(res | LinearCondition::eq);
}

std::string toString(const LinearCondition::Condition cond) {
	if (cond == LinearCondition::ne)
		return s_ne;

	std::string resultStr;
	if (cond & LinearCondition::gt) {
		resultStr.push_back(s_gt);
	}
	else if (cond & LinearCondition::lt) {
		resultStr.push_back(s_lt);
	}

	if (cond & LinearCondition::eq)
		resultStr.push_back(s_eq);

	return resultStr;
}

//
// PUBLIC
//

LinearTask readLinearTask(const std::string &fileName) {
	std::ifstream fin(inputDir + fileName);

	if (!fin.is_open()) {
		fin.close();
		std::cout << "Не удаётся открыть файл на чтение:" << outputDir + fileName << std::endl;
		return LinearTask({});
	}

	std::string readedLine;

	std::getline(fin, readedLine);
	LinearTask linearTask(split(readedLine));

	std::getline(fin, readedLine);
	std::vector<std::string> splited = split(readedLine);
	linearTask.setStrivesForMin(splited.back() == s_min);
	splited.pop_back();
	linearTask.setTargetFunctionCoefs(toCoefVector(splited));
	while (std::getline(fin, readedLine)) {
		splited = split(readedLine);

		LinearCondition cond;
		cond.target = toCoef(splited.back());
		splited.pop_back();
		cond.cond = toCondition(splited.back());
		splited.pop_back();
		cond.coefs = std::move(toCoefVector(splited));

		linearTask.addCond(std::move(cond));
	}

	fin.close();
	return linearTask;
}

void writeLinearTask(const std::string &fileName, const LinearTask &linearTask, const std::string &title, bool append)
{
	std::ofstream fout(outputDir + fileName, append ? std::ios::app : std::ios::out);

	if (!fout.is_open()) {
		fout.close();
		std::cout << "Не удаётся открыть файл для записи:" << outputDir + fileName << std::endl;
		return;
	}

	std::string resultStr(title);
	resultStr.push_back(stringEnd);

	for (const std::string &name : linearTask.variablesNames()) {
		resultStr += name;
		resultStr.push_back(sep);
		resultStr.push_back(sep);
	}
	resultStr.back() = stringEnd;

	resultStr += toString(linearTask.targetFunctionCoefs());
	resultStr += (linearTask.strivesForMin() ? s_min : s_max);
	resultStr.push_back(stringEnd);

	for (const LinearCondition &cond : linearTask.conds()) {
		resultStr += toString(cond.coefs);
		resultStr += toString(cond.cond);
		resultStr.push_back(sep);
		resultStr += toString(cond.target);
		resultStr.push_back(stringEnd);
	}
	resultStr.push_back(stringEnd);

	fout << resultStr;
	fout.close();
}

void writeBasis(const std::string &fileName, const LinearTask &linearTask, bool append)
{
	std::ofstream fout(outputDir + fileName, append ? std::ios::app : std::ios::out);

	if (!fout.is_open()) {
		fout.close();
		std::cout << "Не удаётся открыть файл для записи:" << outputDir + fileName << std::endl;
		return;
	}

	std::string resultStr("Базис: ");

	LinearTask::Basis basis = linearTask.currentBasis();

	for (const auto &el : basis) {
		resultStr += linearTask.variablesNames()[el.second];
		resultStr += ", ";
	}
	resultStr.pop_back();
	resultStr.back() = stringEnd;
	resultStr.push_back(stringEnd);

	fout << resultStr;
	fout.close();
}

void writeCalculator(const std::string &fileName, const SimplexMethodCalculator &calculator, bool append)
{
	std::ofstream fout(outputDir + fileName, append ? std::ios::app : std::ios::out);

	if (!fout.is_open()) {
		fout.close();
		std::cout << "Не удаётся открыть файл для записи:" << outputDir + fileName << std::endl;
		return;
	}

	std::string resultStr;
	resultStr.push_back(sep);

	for (const auto &name : calculator.m_variablesNames) {
		resultStr += name;
		resultStr.push_back(sep);
		resultStr.push_back(sep);
	}
	resultStr.push_back('b');
	resultStr.push_back(stringEnd);

	for (LinearTask::ConditionIndex i = 0; i <= calculator.m_basis.size(); i++)
	{
		if (i == calculator.m_basis.size()) {
			resultStr.push_back(calculator.m_supportive ? 'S' : 'F');
		}
		else {
			LinearTask::VariableIndex vari = calculator.m_basis.at(i);
			resultStr += calculator.m_variablesNames[vari];
		}
		resultStr.push_back(sep);
		for (const Coef &coef : calculator.m_rows[i].coefs) {
			resultStr += toString(coef);
			resultStr.push_back(sep);
		}
		resultStr.back() = stringEnd;
	}
	resultStr.push_back(stringEnd);

	fout << resultStr;
	fout.close();
}

void writeCalculatorResult(const std::string &fileName, const SimplexMethodCalculator &calculator, const LinearTask::VariableIndex startVariablesCount, const bool reversed, bool append)
{
	std::ofstream fout(outputDir + fileName, append ? std::ios::app : std::ios::out);

	if (!fout.is_open()) {
		fout.close();
		std::cout << "Не удаётся открыть файл для записи:" << outputDir + fileName << std::endl;
		return;
	}

	std::string resultStr;
	resultStr += "Ответ: ";

	resultStr.push_back('(');
	for (LinearTask::VariableIndex i = 0; i < startVariablesCount; i++)
	{
		resultStr += calculator.m_variablesNames[i];
		resultStr.push_back(',');
		resultStr.push_back(' ');
	}
	resultStr.pop_back();
	resultStr.pop_back();
	resultStr += ") = (";
	CoefVector xVector;
	xVector.resize(startVariablesCount, 0);
	for (const auto &el : calculator.m_basis) {
		if (el.second < startVariablesCount) {
			xVector[el.second] = calculator.m_rows[el.first].coefs.back();
		}
	}


	for (const auto coef : xVector) {
		resultStr += toString(coef);
		resultStr.push_back(',');
		resultStr.push_back(' ');
	}
	resultStr.pop_back();
	resultStr.pop_back();
	resultStr.push_back(')');
	resultStr.push_back(sep);
	resultStr += "F = ";

	Coef resultF = 0;
	for (LinearTask::VariableIndex i = 0; i < startVariablesCount; i++)
	{
		resultF += xVector[i] * calculator.m_mainTargetFunctionCoefs[i];
	}
	//resultStr += toString(abs(calculator.m_rows.back().coefs.back()));
	if (reversed)
		resultF *= -1;
	resultStr += toString(resultF);

	resultStr.push_back(stringEnd);

	fout << resultStr;
	fout.close();
}

void writeString(const std::string &fileName, const std::string &str, bool append)
{
	std::ofstream fout(outputDir + fileName, append ? std::ios::app : std::ios::out);

	if (!fout.is_open()) {
		fout.close();
		std::cout << "Не удаётся открыть файл для записи:" << outputDir + fileName << std::endl;
		return;
	}

	fout << str;
	fout.close();
}

