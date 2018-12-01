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
#include <fstream>

std::ofstream os;
const char* out_path = "C:/Users/lenovo/Source/Repos/___RulsTSL/x64/Release/result.lr1";
auto ____ = []()
{
	os.open(out_path, std::ios::out | std::ios::trunc);
	if (!os.is_open())
	{
		std::cout << "Can't open file: " << out_path << std::endl;
		exit(0);
	}
	return 0;
}();
#define out os
#define OUTPUT_DFA
#include "../src/lr1.hpp"

void rsy_lexer_test()
{
	Mini_C::lexer::Lexer _lexer;
	try { _lexer.tokenize("./test/rsy1.txt"); }
	catch (const Mini_C::MiniC_Base_Exception& e) { e.printException(); std::cout << std::endl; }
	catch (const std::exception& e) { std::cout << e.what() << std::endl << std::endl; }
	catch (...) { std::cout << "WTF: Unexpected Exception" << std::endl << std::endl; }
	_lexer.print(out);
	out << "\n-----------------------------------------\n" << std::endl;

	std::optional<std::pair<Mini_C::lexer::Token, std::string>> result =
		Mini_C::LR1::analyze(_lexer);
	if (result.has_value())
	{
		auto const&[token, str] = result.value();
		Mini_C::TEST::outputToken(token, out);
		out << str << std::endl;
	}
	else std::cout << "ok" << std::endl;
}


namespace Mini_C::parser
{
	void eval();
	void apply();
}

void rsy_parser_test()
{
	using namespace Mini_C::parser;




}


void test()
{
	rsy_lexer_test();
	// rsy_parser_test();
}


int main()
{
	::test();
	system("pause");
	return 0;
}
#endif // RSY_TEST
