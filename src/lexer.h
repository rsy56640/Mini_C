#pragma once
#ifndef _LEXER_H
#define _LEXER_H
#include <variant>
#include <utility>
#include <string>
#include <unordered_map>
#include <vector>
#include "miniC_exception.h"

namespace Mini_C::lexer
{

	/*
	 * represent names for: variable, function and struct
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
	enum class type
	{
		ADD, SUB, MUL, DIV, SELF_INC, SELF_DEC,
	};
	static const std::unordered_map<std::string, type> keywords = {};


	/*
	 * represent all types of token
	 */
	using token_t = std::variant<type, identifier, numeric_t, string_literal_t>;


	/*
	 * Parameter:
	 *     s         :  char array to be processed.
	 *     size      :  actual amounts of char to be processed.
	 *
	 * Return value:
	 *     return a vector with tokens, then
	 *
	 * Exception:
	 *     throw Token_Ex, which implies 'error message' and 'position'.
	 *
	 */
	namespace analyzers
	{
		struct Token_Ex
		{
			const std::string _msg;
			const std::size_t _position;
			Token_Ex(const std::string& msg, std::size_t position)
				:_msg(msg), _position(position) {}
			Token_Ex(std::string&& msg, std::size_t position)
				:_msg(std::move(msg)), _position(position) {}
		};
	}
	std::vector<token_t> tokenize(const char* s, const std::size_t size);


}// end namespace Mini_C::lexer

#endif // !_LEXER_H