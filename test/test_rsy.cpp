#ifdef RSY_TEST
#include <iostream>
#include <variant>
#include <vector>
#include <string>
#include "../src/lexer.h"
#include "../src/miniC_exception.h"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

void test()
{
	const char* s = "(3+4.2)-abc+32u";
	auto result = Mini_C::lexer::tokenize(s, strlen(s));
	std::visit(overloaded{
		[](Mini_C::lexer::analyzers::Token_Ex e) {
		Mini_C::MiniC_Universal_Exception{
			std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
			0u, e._position }.printException(); },
		[](std::vector<Mini_C::lexer::token_t> tokens) {
			for (auto const& token : tokens)
				std::visit(overloaded{
				[](Mini_C::lexer::type _type) {std::cout << "type: " << std::endl; },
				[](Mini_C::lexer::identifier _identifier) {std::cout << "identifier: " << _identifier << std::endl; },
				[](Mini_C::lexer::numeric_t _num) {std::cout << "numeric: " << std::get<double>(_num) << std::endl; },
				[](Mini_C::lexer::string_literal_t _str) {std::cout << "string literal: " << std::get<const std::string>(_str) << std::endl; },
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
