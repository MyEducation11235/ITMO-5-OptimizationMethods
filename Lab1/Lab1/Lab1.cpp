#include "InputOutput.h"
#include <conio.h>

int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	std::system("chcp 1251");

	std::string inputFile;// = "main.txt";

	if (inputFile.empty()) {
		std::cout << "Ведите название исодного файла: ";
		std::cin >> inputFile;
		if (inputFile.find(".txt") == -1)
			inputFile += ".txt";
	}
	const std::string outputFile = inputFile;

	int stage = 1;

	LinearTask task = readLinearTask(inputFile);
	writeLinearTask(outputFile, task, std::to_string(stage++) + ". Считанные условия:", false);

	const LinearTask::VariableIndex startVariablesCount = task.variablesCount();

	if (!task.strivesForMin())
		task.reverseStrives();
	task.equalizedConditions();

	writeLinearTask(outputFile, task, std::to_string(stage++) + ". Приведеная к каноническому виду:");
	writeBasis(outputFile, task);

	LinearTask::Basis basis = task.currentBasis();
	// требуется ли вспомогательная задача
	if (basis.size() < task.conditionCount()) {
		LinearTask supportiveTask = task;
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

		writeLinearTask(outputFile, supportiveTask, std::to_string(stage++) + ". Формирование вспомогательной задачи:");
		writeBasis(outputFile, supportiveTask);

		SimplexMethodCalculator calculator(supportiveTask, true);

		writeString(outputFile, std::to_string(stage++) + ". Решение вспомогательной задачи:\n");
		writeCalculator(outputFile, calculator);
		while (calculator.oneCalcStep()) {
			writeCalculator(outputFile, calculator);
		}

		calculator.continueLikeMainTask(task);

		writeString(outputFile, std::to_string(stage++) + ". Переход к основной задаче:\n");
		writeCalculator(outputFile, calculator);
		while (calculator.oneCalcStep()) {
			writeCalculator(outputFile, calculator);
		}

		writeCalculatorResult(outputFile, calculator, startVariablesCount);
	}
	else {
		SimplexMethodCalculator calculator(task, false);

		writeString(outputFile, std::to_string(stage++) + ". Решение основной задачи:\n");
		writeCalculator(outputFile, calculator);
		while (calculator.oneCalcStep()) {
			writeCalculator(outputFile, calculator);
		}

		writeCalculatorResult(outputFile, calculator, startVariablesCount);
	}
	//writeLinearTask(outputFile, task, std::to_string(stage++) + ". Ответ:");

	std::cout << "Нажмите любую клавишу, чтобы закрыть это окно…";
	_getch();
}