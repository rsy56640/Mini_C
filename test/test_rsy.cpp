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
#include "../src/rule.h"
#include <fstream>
#include <iomanip>

void rsy_lexer_test()
{
	Mini_C::lexer::Lexer _lexer;
	try { _lexer.tokenize("./test/rsy1.txt"); }
	catch (const Mini_C::MiniC_Base_Exception& e) { e.printException(); std::cout << std::endl; }
	catch (const std::exception& e) { std::cout << e.what() << std::endl << std::endl; }
	catch (...) { std::cout << "WTF: Unexpected Exception" << std::endl << std::endl; }
	_lexer.print();

	std::optional<std::pair<Mini_C::lexer::Token, std::string>> result =
		Mini_C::LR1::analyze(_lexer);
	if (result.has_value()) {
		auto const&[token, str] = result.value();
		Mini_C::TEST::outputToken(token);
		std::cout << str << std::endl;
	}
	else std::cout << "ok" << std::endl;
	/*
	while (!_lexer.empty())
	{
		Mini_C::lexer::Token t = _lexer.consumeToken();
		std::cout << "In line: " << t._line << ", pos: " << t._pos
			<< type2str(getType(t)) << std::endl;;
	}
	*/
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
	std::getchar();
	return 0;
}
#endif // RSY_TEST
