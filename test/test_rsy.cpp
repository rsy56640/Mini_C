#ifdef RSY_TEST
#include <iostream>
#include <variant>
#include <vector>
#include <string>
#include <type_traits>
#include "../src/lexer.h"
#include "../src/miniC_exception.h"
#include <fstream>
#include <iomanip>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

void test()
{
	const char* filename = "./test/rsy1.txt";
	std::ifstream inputFile{ filename, std::ios::in };
	if (!inputFile.is_open()) {
		std::cout << "failed to open: " << std::quoted(filename) << std::endl;
	}
	else {
		try {


		}
		catch (Mini_C::MiniC_Base_Exception& e)
		{
			e.printException();
		}
	}
}


void test_lexer()
{
	const char* s = "(3+4.2)-abc+32";
	auto result = Mini_C::lexer::tokenize(s, strlen(s));
	std::visit(overloaded{
		[](const Mini_C::lexer::analyzers::Token_Ex& e) {
		Mini_C::MiniC_Universal_Exception{
			std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
			0u, e._position }.printException(); },
			[](const std::vector<Mini_C::lexer::token_t>& tokens) {
			for (auto const& token : tokens)
				std::visit(overloaded{
				[](const Mini_C::lexer::type& _type) { std::cout << "type: " << std::endl; },
				[](const Mini_C::lexer::identifier& _identifier) { std::cout << "identifier: " << _identifier << std::endl; },
				[](const Mini_C::lexer::numeric_t& _num) { std::cout << "numeric: " << std::get<double>(_num) << std::endl; },
				[](const Mini_C::lexer::string_literal_t& _str) { std::cout << "string literal: " << std::quoted(std::get<const std::string>(_str)) << std::endl; },
				[](auto) { std::cout << "WTF" << std::endl; },
					}, token); },
		}, result);
}

int main()
{
	test();
	std::getchar();
	return 0;
}
#endif // RSY_TEST
