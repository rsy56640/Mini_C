#ifdef RSY_TEST
#include <iostream>
#include <variant>
#include <vector>
#include <string>
#include <exception>
#include <type_traits>
#include "../src/lexer.h"
#include "../src/miniC_exception.h"
#include "test.h"
#include <fstream>
#include <iomanip>

void rsy_test()
{
	using namespace TEST;
	constexpr std::size_t MAXSIZE = 128;
	char buffer[MAXSIZE];
	const char* filename = "./test/rsy1.txt";
	std::ifstream inputFile{ filename, std::ios::in };
	if (!inputFile.is_open())
	{
		std::cout << "failed to open: " << std::quoted(filename) << std::endl;
		return;
	}
	try {
		std::size_t line_num = 0;
		while (!inputFile.eof())
		{
			inputFile.getline(buffer, MAXSIZE - 1);
			line_num++;
			std::cout << "-------------------------------------------------" << std::endl;
			std::cout << "line " << line_num << ":" << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
			TEST_LEXER::test_lexer(buffer, line_num);
			std::cout << std::endl << std::endl;
		}
		inputFile.close();
	}
	catch (const Mini_C::MiniC_Base_Exception& e) { e.printException(); }
	catch (const std::exception& e) { std::cout << e.what() << std::endl; }
	catch (...) { std::cout << "WTF: Unexpected Exception" << std::endl; }
}


int main()
{
	rsy_test();
	std::getchar();
	return 0;
}
#endif // RSY_TEST
