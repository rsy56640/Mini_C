#ifndef _UTIL_H
#define _UTIL_H
#include "../src/lexer.h"

namespace Mini_C::uitl
{
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	void num_print(const Mini_C::lexer::numeric_t& _num) noexcept;

	void outputLexVector(const std::vector<lexer::token_t> &result, const std::size_t line_num) noexcept;

}

#endif // !_UTIL_H
