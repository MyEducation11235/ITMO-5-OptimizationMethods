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
	bool reversed = !task.strivesForMin();

	if (reversed) {
		task.reverseStrives();
	}
	task.equalizedConditions();

	writeLinearTask(outputFile, task, std::to_string(stage++) + ". Приведеная к каноническому виду:");
	writeBasis(outputFile, task);

	SimplexMethodCalculator calculator;
	LinearTask supportiveTask = calculator.setLinearTask(task);

	const auto fullCalculate = [&outputFile, &calculator](const size_t maxIterCount = 100) -> bool {
		writeCalculator(outputFile, calculator);
		size_t i = 0;
		for (; (i < maxIterCount) && calculator.oneCalcStep(); i++)
		{
			writeCalculator(outputFile, calculator);
		}

		return i < maxIterCount;
	};

	if (calculator.supportive()) {
		writeLinearTask(outputFile, supportiveTask, std::to_string(stage++) + ". Формирование вспомогательной задачи:");
		writeBasis(outputFile, supportiveTask);

		writeString(outputFile, std::to_string(stage++) + ". Решение вспомогательной задачи:\n");
		if (!fullCalculate()) {
			writeString(outputFile, "Критерий не достигает конечного значения.\n");
			goto calculationEnd;
		}

		if (!calculator.continueLikeMainTask()) {
			writeString(outputFile, "Не удалось перейти к основному решению.\n");
			goto calculationEnd;
		}
	}
	writeString(outputFile, std::to_string(stage++) + ". Решение основной задачи:\n");
	if (!fullCalculate()) {
		writeString(outputFile, "Критерий не достигает конечного значения.\n");
		goto calculationEnd;
	}

	writeCalculatorResult(outputFile, calculator, startVariablesCount, reversed);

calculationEnd:
	std::cout << "Нажмите любую клавишу, чтобы закрыть это окно…";
	_getch();
}