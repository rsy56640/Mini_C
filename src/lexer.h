#pragma once
#ifndef _LEXER_H
#define _LEXER_H
#include <variant>
#include <utility>
#include <string>
#include <unordered_map>
#include <vector>

namespace Mini_C::lexer
{

	/*
	 * represent names for: variable and function
	 */
	using identifier = std::string;


	/*
	 * represent char, i16, i32, f32, f64
	 */
	enum class numeric_type { CHAR, I16, I32, F32, F64 };
	using numeric_t = std::tuple<double, numeric_type>;


	/*
	 * represent string literals: "xxx".
	 */
	using string_literal_t = std::tuple<const std::string>;


	/*
	 * enum class `type` represents: 1. all of key-words
	 *                               2. operators
	 *                               3. others, like `,;()[]{}`
	 */
	enum class type { /* implementation defined */ };
	static const std::unordered_map<std::string, type> keywords = {};


	/*
	 * represent all types of token
	 */
	using token_t = std::variant<type, identifier, numeric_t, string_literal_t>;


	/*
	 * Parameter:
	 *     c         :  char array to be processed.
	 *     size      :  actual amounts of char to be processed.
	 *     line_num  :  the current line number, used in exception information.
	 *
	 * Return value:
	 *     return a vector with tokens, then
	 *
	 * Exception:
	 *     throw
	 *
	 *
	 */
	namespace tokenize_util
	{
		bool isKeyWord(const std::string&);
		bool isKeyWord(char);
	}
	std::vector<token_t> tokenize(const char* c, const std::size_t size, const std::size_t line_num);


}// end namespace Mini_C::lexer

#endif // !_LEXER_H