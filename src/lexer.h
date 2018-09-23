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
	 * represent char, i16, i32, u16, u32, f32, f64
	 * TODO: type promotion when different numeric type in arithmetic operation.
	 *       So maybe some tactic(such as bit operation) can be used here,
	 *       and the value should be designed carefully.
	 *
	 * Promise: The order of `numeric_type` is same in `type`.
	 */
	enum class numeric_type { CHAR, I16, I32, U16, U32, F32, F64 };
	using numeric_t = std::tuple<double, numeric_type>;


	/*
	 * represent string literals: "xxx".
	 */
	using string_literal_t = std::tuple<const std::string>;


	/*
	 * enum class `type` represents: 1. all of key-words
	 *                               2. operators
	 *                               3. others, like `,.;()[]{}`
	 */
	enum class type
	{
		ADD, SUB, MUL, DIV,
		SELF_INC, SELF_DEC,

		CHAR, I16, I32,
		U16, U32, F32, F64,

		COMMA, PERIOD, SEMICOLON,                    // ",", ".", ";"
		LEFT_PARENTHESIS, RIGHT_PARENTHESIS,         // parenthesis      : "(", ")"
		LEFT_SQUARE_BRACKETS, RIGHT_SQUARE_BRACKETS, // square brackets  : "[", "]"
		LEFT_CURLY_BRACKETS, RIGHT_CURLY_BRACKETS,   // curly brackets   : "{", "}"

	};

	type num_t2type(numeric_type num_t);
	/*
	inline numeric_type type2num_t(type _t) {
		return static_cast<numeric_type>(static_cast<std::size_t>(numeric_type::CHAR)
			+ static_cast<std::size_t>(_t) - static_cast<std::size_t>(type::CHAR));
	}*/

	static const std::unordered_map<std::string, type> keywords =
	{
		{ "+", type::ADD }, { "-", type::SUB }, { "*", type::MUL }, { "/", type::DIV },
		{ "++", type::SELF_INC }, { "--", type::SELF_DEC },


		{ "char", type::CHAR }, { "i16", type::I16 }, { "i32", type::I32 },
		{ "u16", type::U16 }, { "u32", type::U32 }, { "f32", type::F32 }, { "f64", type::F64 },

		{ ",", type::COMMA }, { ".", type::PERIOD }, { ";", type::SEMICOLON },
		{ "(", type::LEFT_PARENTHESIS },      { ")", type::RIGHT_PARENTHESIS },
		{ "[", type::LEFT_SQUARE_BRACKETS },  { "]", type::RIGHT_SQUARE_BRACKETS },
		{ "{", type::LEFT_CURLY_BRACKETS },   { "}", type::RIGHT_CURLY_BRACKETS },

	};

	// for display
	std::string type2str(type _type);
	static const std::unordered_map<type, std::string> keyword2str =
	{
		{ type::ADD, "+" }, { type::SUB, "-" }, { type::MUL, "*",  },{ type::DIV, "/",  },
		{ type::SELF_INC, "++" }, { type::SELF_DEC, "--" },


		{ type::CHAR, "char" }, { type::I16 , "i16" }, { type::I32, "i32" },
		{ type::U16, "u16" }, { type::U32, "u32" }, { type::F32, "f32" },{ type::F64, "f64" },

		{ type::COMMA, "," }, { type::PERIOD, "." }, {  type::SEMICOLON, ";" },
		{ type::LEFT_PARENTHESIS, "("  },      { type::RIGHT_PARENTHESIS, ")" },
		{ type::LEFT_SQUARE_BRACKETS, "["  },  { type::RIGHT_SQUARE_BRACKETS, "]" },
		{ type::LEFT_CURLY_BRACKETS, "{"  },   { type::RIGHT_CURLY_BRACKETS, "}" },

	};



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
			std::string _msg;
			std::size_t _position;
			Token_Ex(const std::string& msg, std::size_t position)
				:_msg(msg), _position(position) {}
			Token_Ex(std::string&& msg, std::size_t position)
				:_msg(std::move(msg)), _position(position) {}
		};
	}
	std::variant<std::vector<token_t>, analyzers::Token_Ex> tokenize(const char* s, const std::size_t size);


}// end namespace Mini_C::lexer

#endif // !_LEXER_H