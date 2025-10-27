#include "InputOutput.h"
#include <conio.h>

int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	std::system("chcp 1251");

	const std::string inputFile = "1.txt";
	const std::string outputFile = "test.txt";//inputFile;

	const LinearTask task = readLinearTask(inputFile);
	writeLinearTask(outputFile, task, "Считанные условия:", false);

	std::cout << "Нажмите любую клавишу, чтобы закрыть это окно…";
	_getch();
}