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
	if (calculator.supportive()) {
		writeLinearTask(outputFile, supportiveTask, std::to_string(stage++) + ". Формирование вспомогательной задачи:");
		writeBasis(outputFile, supportiveTask);

		writeString(outputFile, std::to_string(stage++) + ". Решение вспомогательной задачи:\n");
		writeCalculator(outputFile, calculator);
		while (calculator.oneCalcStep()) {
			writeCalculator(outputFile, calculator);
		}

		calculator.continueLikeMainTask();
	}
	writeString(outputFile, std::to_string(stage++) + ". Решение основной задачи:\n");
	writeCalculator(outputFile, calculator);
	while (calculator.oneCalcStep()) {
		writeCalculator(outputFile, calculator);
	}

	writeCalculatorResult(outputFile, calculator, startVariablesCount, reversed);

	std::cout << "Нажмите любую клавишу, чтобы закрыть это окно…";
	_getch();
}