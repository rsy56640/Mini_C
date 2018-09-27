#ifndef _UTIL_H
#define _UTIL_H
#include "../src/lexer.h"

namespace Mini_C::util
{
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	/*
	 * print numeric value in form of formal format.
	 */
	void num_print(const Mini_C::lexer::numeric_t& _num) noexcept;

	/*
	 * output the token-list when used in debug mode.
	 */
	void outputLexVector(const std::vector<lexer::token_info> &result) noexcept;

}

#endif // !_UTIL_H
