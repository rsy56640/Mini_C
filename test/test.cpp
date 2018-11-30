#include <variant>
#include "test.h"
#include "../util/util.h"

namespace Mini_C::TEST
{

	using namespace Mini_C::lexer;
	using namespace Mini_C::util;

	namespace TEST_LEXER
	{
		void test_lexer(const char* s, const std::size_t line_num)
		{
			auto result = Mini_C::lexer::tokenize(s, strlen(s));
			std::visit(overloaded{
					[line_num](const Mini_C::lexer::analyzers::Token_Ex& e) {
						throw Mini_C::MiniC_Universal_Exception{
							std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
							line_num, e._position }; },
					[](const std::vector<Mini_C::lexer::token_info>& tokens) { outputLexVector(tokens, std::cout); },
				}, result);
		}
	}// end namespace TEST_LEXER


	void num_print(const Mini_C::lexer::numeric_t& _num, std::ostream& out)
	{
		const static std::unordered_map<char, std::string> escapingMap = {
			{ '\t', "'\\t'" },{ '\n', "'\\n'" },{ '\r', "'\\r'" },
			{ '\a', "'\\a'" },{ '\b', "'\\b'" },{ '\f', "'\\f'" },
		};

		Mini_C::lexer::numeric_type num_t = std::get<const Mini_C::lexer::numeric_type>(_num);

		if (num_t == Mini_C::lexer::numeric_type::U32)
			out << static_cast<std::size_t>(std::get<0>(_num));

		else if (num_t == Mini_C::lexer::numeric_type::F32 || num_t == Mini_C::lexer::numeric_type::F64)
			out << static_cast<double>(std::get<0>(_num));

		else if (num_t == Mini_C::lexer::numeric_type::CHAR)
		{
			const char c = static_cast<char>(std::get<0>(_num));
			if (auto it = escapingMap.find(c); it != escapingMap.end())
				out << it->second;
			else out << std::string("'") + c + "'";
		}

		else if (num_t == Mini_C::lexer::numeric_type::BOOLEAN)
			out << (static_cast<std::size_t>(std::get<0>(_num)) ? "true" : "false");

		else out << static_cast<int>(std::get<0>(_num));
	}


	void output_token_t(const lexer::token_t& token, std::ostream& out)
	{
		std::visit(overloaded{
				[&out](const Mini_C::lexer::type& _type) { out << "type: " << "\t\t\t" << std::quoted(Mini_C::lexer::type2str(_type)) << std::endl; },
				[&out](const Mini_C::lexer::identifier& _identifier) { out << "identifier: " << "\t\t" << _identifier << std::endl; },
				[&out](const Mini_C::lexer::numeric_t& _num) { out << "numeric: " << "\t\t" << std::quoted(Mini_C::lexer::type2str(num_t2type(std::get<const Mini_C::lexer::numeric_type>(_num)))) << " "; num_print(_num, out); out << std::endl; },
				[&out](const Mini_C::lexer::string_literal_t& _str) { out << "string literal: " << "\t" << std::quoted(std::get<const std::string>(_str)) << std::endl; },
				[&out](auto) { out << "WTF: tokenizer" << std::endl; },
			}, token);
	}

	void outputLexVector(const std::vector<lexer::token_info> &tokens, std::ostream& out)
	{
		for (auto const& token : tokens)
			output_token_t(std::get<lexer::token_t>(token), out);
	}


	void outputToken(const lexer::Token& token, std::ostream& out)
	{
		out << "In line: " << token._line << ", pos: " << token._pos << ", ";
		output_token_t(token._token, out);
	}

	void outputTokenVector(const std::vector<lexer::Token>& tokens, std::ostream& out)
	{
		for (auto const& token : tokens)
			outputToken(token, out);
	}

	/*
	std::string outputType(const parser::value_t& _type)
	{
		return std::string();
	}*/

}// end namespace TEST
