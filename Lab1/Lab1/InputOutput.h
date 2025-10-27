п»ї#pragma once

#include "LinearTask.h"

LinearTask readLinearTask(const std::string &fileName);

void writeLinearTask(const std::string &fileName, const LinearTask &linearTask, const std::string &title, bool append = true);
