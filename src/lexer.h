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
	 * represent bool, char, i16, i32, u16, u32, f32, f64
	 * TODO: type promotion when different numeric type in arithmetic operation.
	 *       So maybe some tactic(such as bit operation) can be used here,
	 *       and the value should be designed carefully.
	 *
	 * Promise: The order of `numeric_type` is same in `type`.
	 */
	enum class numeric_type { BOOLEAN, CHAR, I16, I32, U16, U32, F32, F64 };
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
		/* arithmetic operator */
		ADD, SUB, MUL, DIV, MOD,                    // + - * / %   // Note: * also can be pointer!!
		LEFT_SHIFT, RIGHT_SHIFT,                    // << >>
		NOT, AND, OR, XOR,                          // ~ & | ^     // Note: & also can be take address.


		SELF_INC, SELF_DEC,                         // ++ --

		/* logic operator */
		LOGIC_NOT, LOGIC_AND, LOGIC_OR,             // ! && ||

		/* compare operator */
		EQ, NEQ,                                    // == !=
		LESS, GREATER, LEQ, GEQ,                    // < > <= >=

		/* assign operator */
		ASSIGN,                                     // =
		ADD_EQ, SUB_EQ, MUL_EQ, DIV_EQ, MOD_EQ,     // += -= *= /= %=
		L_SHIFT_EQ, R_SHIFT_EQ,                     // <<= >>=
		AND_EQ, OR_EQ, XOR_EQ,                      // &= |=

		/* numeric type */
		BOOLEAN, CHAR, I16, I32,
		U16, U32, F32, F64,

		/* special keyword */
		TRUE, FALSE,
		STRUCT, STR, FN, VOID,
		STATIC, CONST,
		ENUM, UNION,
		NEW, DELETE,
		USING,
		MAIN,

		/* control statement */
		IF, ELSE,
		FOR, WHILE, DO,
		CONTINUE, BREAK,
		SWITCH, CASE, DEFAULT,
		RETURN,

		/* other operator */
		COMMA, PERIOD, SEMICOLON,                    // ",", ".", ";"
		QUESTION, COLON,                             // "?", ":"      // ternary operator
		MEMBER_ACCESS, CLASS_SCOPE,                  // "->", "::"    // "->": 1. member access; 2. return type specification
		SIZEOF,                                      // sizeof
		LEFT_PARENTHESIS, RIGHT_PARENTHESIS,         // parenthesis      : "(", ")"
		LEFT_SQUARE_BRACKETS, RIGHT_SQUARE_BRACKETS, // square brackets  : "[", "]"
		LEFT_CURLY_BRACKETS, RIGHT_CURLY_BRACKETS,   // curly brackets   : "{", "}"

	};

	type num_t2type(numeric_type num_t) noexcept;
	static const std::unordered_map<std::string, type> keywords =
	{
		/* arithmetic operator */
		{ "+", type::ADD }, { "-", type::SUB }, { "*", type::MUL }, { "/", type::DIV }, { "%", type::MOD },
		{ "<<", type::LEFT_SHIFT }, { ">>", type::RIGHT_SHIFT },
		{ "~", type::NOT }, { "&", type::AND }, { "|", type::OR }, { "^", type::XOR },


		{ "++", type::SELF_INC }, { "--", type::SELF_DEC },

		/* logic operator */
		{ "!", type::LOGIC_NOT }, { "&&", type::LOGIC_AND }, { "||", type::LOGIC_OR },

		/* compare operator */
		{ "==", type::EQ }, { "!=", type::NEQ },
		{ "<", type::LESS }, { ">", type::GREATER }, { "<=", type::LEQ }, { ">=", type::GEQ },

		/* assign operator */
		{ "=", type::ASSIGN },
		{ "+=", type::ADD_EQ }, { "-=", type::SUB_EQ }, { "*=", type::MUL_EQ }, { "/=", type::DIV_EQ }, { "%=", type::MOD_EQ },
		{ "<<=", type::L_SHIFT_EQ }, { ">>=", type::R_SHIFT_EQ },
		{ "&=", type::AND_EQ }, { "|=", type::OR_EQ }, { "^=", type::XOR_EQ },

		/* numeric type */
		{ "bool", type::BOOLEAN }, { "char", type::CHAR }, { "i16", type::I16 }, { "i32", type::I32 },
		{ "u16", type::U16 }, { "u32", type::U32 }, { "f32", type::F32 }, { "f64", type::F64 },

		/* special keyword */
		{ "true", type::TRUE }, { "false", type::FALSE },
		{ "struct", type::STRUCT }, { "str", type::STR }, { "fn", type::FN }, { "void", type::VOID },
		{ "static", type::STATIC }, { "const", type::CONST },
		{ "enum", type::ENUM }, { "union", type::UNION },
		{ "new", type::NEW }, { "delete", type::DELETE },
		{ "using", type::USING },
		{ "main", type::MAIN },

		/* control statement */
		{ "if", type::IF }, { "else", type::ELSE },
		{ "for", type::FOR }, { "while", type::WHILE }, { "do", type::DO },
		{ "continue", type::CONTINUE }, { "break", type::BREAK },
		{ "switch", type::SWITCH }, { "case", type::CASE }, { "default", type::DEFAULT },
		{ "return", type::RETURN },

		/* other operator */
		{ ",", type::COMMA }, { ".", type::PERIOD }, { ";", type::SEMICOLON },
		{ "?", type::QUESTION }, { ":", type::COLON },
		{ "->", type::MEMBER_ACCESS }, { "::", type::CLASS_SCOPE },
		{ "sizeof", type::SIZEOF },
		{ "(", type::LEFT_PARENTHESIS },      { ")", type::RIGHT_PARENTHESIS },
		{ "[", type::LEFT_SQUARE_BRACKETS },  { "]", type::RIGHT_SQUARE_BRACKETS },
		{ "{", type::LEFT_CURLY_BRACKETS },   { "}", type::RIGHT_CURLY_BRACKETS },

	};

	// for display
	std::string type2str(type _type) noexcept;
	static const std::unordered_map<type, std::string> keyword2str =
	{
		/* arithmetic operator */
		{ type::ADD, "+" }, { type::SUB, "-" }, { type::MUL, "*",  }, { type::DIV, "/",  }, { type::MOD, "%" },
		{ type::LEFT_SHIFT, "<<" }, { type::RIGHT_SHIFT, ">>" },
		{ type::NOT, "~" }, { type::AND, "&" }, { type::OR, "|" }, { type::XOR, "^" },

		{ type::SELF_INC, "++" }, { type::SELF_DEC, "--" },

		/* logic operator */
		{ type::LOGIC_NOT, "!" }, { type::LOGIC_AND, "&&" }, { type::LOGIC_OR, "||" },

		/* compare operator */
		{ type::EQ , "==" }, { type::NEQ, "!=" },
		{ type::LESS , "<" }, { type::GREATER, ">" }, { type::LEQ , "<=" }, { type::GEQ, ">=" },

		/* assign operator */
		{ type::ASSIGN, "=" },
		{ type::ADD_EQ, "+=" }, { type::SUB_EQ, "-=" }, { type::MUL_EQ, "*=" }, { type::DIV_EQ, "/=" }, { type::MOD_EQ, "%=" },
		{ type::L_SHIFT_EQ, "<<=" }, { type::R_SHIFT_EQ, ">>=" },
		{ type::AND_EQ, "&=" }, { type::OR_EQ, "|=" }, { type::XOR_EQ, "^=" },

		/* numeric type */
		{ type::BOOLEAN, "bool" }, { type::CHAR, "char" }, { type::I16 , "i16" }, { type::I32, "i32" },
		{ type::U16, "u16" }, { type::U32, "u32" }, { type::F32, "f32" }, { type::F64, "f64" },

		/* special keyword */
		{ type::TRUE, "true" }, { type::FALSE, "false" },
		{ type::STRUCT, "struct" }, { type::STR, "str" }, { type::FN, "fn" }, { type::VOID, "void" },
		{ type::STATIC, "static" }, { type::CONST, "const" },
		{ type::ENUM, "enum" }, { type::UNION, "union" },
		{ type::NEW, "new" }, { type::DELETE, "delete" },
		{ type::USING, "using" },
		{ type::MAIN, "main" },

		/* control statement */
		{ type::IF, "if" }, { type::ELSE, "else" },
		{ type::FOR, "for"}, { type::WHILE, "while" }, { type::DO, "do" },
		{ type::CONTINUE, "continue" }, { type::BREAK, "break" },
		{ type::SWITCH, "switch" }, { type::CASE, "case" }, { type::DEFAULT, "default" },
		{ type::RETURN, "return" },

		/* other operator */
		{ type::COMMA, "," }, { type::PERIOD, "." }, {  type::SEMICOLON, ";" },
		{ type::QUESTION, "?" }, { type::COLON, ":" },
		{ type::MEMBER_ACCESS, "->" }, { type::CLASS_SCOPE, "::" },
		{ type::SIZEOF, "sizeof" },
		{ type::LEFT_PARENTHESIS, "(" },      { type::RIGHT_PARENTHESIS, ")" },
		{ type::LEFT_SQUARE_BRACKETS, "[" },  { type::RIGHT_SQUARE_BRACKETS, "]" },
		{ type::LEFT_CURLY_BRACKETS, "{" },   { type::RIGHT_CURLY_BRACKETS, "}" },

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
	 *     return a vector with tokens, otherwise the error message.
	 *
	 * Exception:
	 *     No exception thrown, all exception should be diagnoed innerly,
	 *     and return error message if possible.
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
	using pos_t = std::size_t;
	using token_info = std::tuple<token_t, pos_t>;
	std::variant<std::vector<token_info>, analyzers::Token_Ex> tokenize(const char* s, const std::size_t size) noexcept;


}// end namespace Mini_C::lexer

#endif // !_LEXER_H
