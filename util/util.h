#ifndef _UTIL_H
#define _UTIL_H
#include <string>
#include "../src/lexer.h"
#include "../test/attempt/rsy/eval_and_apply.h"

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


	/*
	 * return quoted type-string
	 */
	std::string outputType(const parser::value_t& _type);




}

#endif // !_UTIL_H
