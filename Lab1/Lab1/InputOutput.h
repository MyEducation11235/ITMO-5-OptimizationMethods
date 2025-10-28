#pragma once

#include "LinearTask.h"
#include "SimplexMethodCalculator.h"

LinearTask readLinearTask(const std::string &fileName);

void writeLinearTask(const std::string &fileName, const LinearTask &linearTask, const std::string &title, bool append = true);

void writeBasis(const std::string &fileName, const LinearTask &linearTask, bool append = true);

void writeCalculator(const std::string &fileName, const SimplexMethodCalculator &calculator, bool append = true);
void writeCalculatorResult(const std::string &fileName, const SimplexMethodCalculator &calculator, const LinearTask::VariableIndex startVariablesCount, bool append = true);

void writeString(const std::string &fileName, const std::string &str, bool append = true);