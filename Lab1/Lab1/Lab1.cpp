п»ї#include "InputOutput.h"
#include <conio.h>

int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	std::system("chcp 1251");

	const std::string inputFile = "1.txt";
	const std::string outputFile = "test.txt";//inputFile;

	const LinearTask task = readLinearTask(inputFile);
	writeLinearTask(outputFile, task, "РЎС‡РёС‚Р°РЅРЅС‹Рµ СѓСЃР»РѕРІРёСЏ:", false);

	std::cout << "РќР°Р¶РјРёС‚Рµ Р»СЋР±СѓСЋ РєР»Р°РІРёС€Сѓ, С‡С‚РѕР±С‹ Р·Р°РєСЂС‹С‚СЊ СЌС‚Рѕ РѕРєРЅРѕвЂ¦";
	_getch();
}