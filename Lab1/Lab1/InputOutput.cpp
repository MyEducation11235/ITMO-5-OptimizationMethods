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

std::string toStrign(const Coef coef) {

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

std::string toStrign(const CoefVector &coefs) {
	std::string resultStr;
	for (const Coef coef : coefs) {
		resultStr += toStrign(coef);
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

std::string toStrign(const LinearCondition::Condition cond) {
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
	}
	resultStr.back() = stringEnd;

	resultStr += toStrign(linearTask.targetFunctionCoefs());
	resultStr += (linearTask.strivesForMin() ? s_min : s_max);
	resultStr.push_back(stringEnd);

	for (const LinearCondition &cond : linearTask.conds()) {
		resultStr += toStrign(cond.coefs);
		resultStr += toStrign(cond.cond);
		resultStr.push_back(sep);
		resultStr += toStrign(cond.target);
		resultStr.push_back(stringEnd);
	}
	resultStr.push_back(stringEnd);

	fout << resultStr;
	fout.close();
}