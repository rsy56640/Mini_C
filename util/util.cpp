#include "util.h"
#include <iostream>
#include <iomanip>

namespace Mini_C::util
{

	void outputLexVector(const std::vector<lexer::token_info> &tokens) noexcept
	{
		for (auto const& token : tokens)
			std::visit(overloaded{
					[](const Mini_C::lexer::type& _type) { std::cout << "type: " << std::quoted(Mini_C::lexer::type2str(_type)) << std::endl; },
					[](const Mini_C::lexer::identifier& _identifier) { std::cout << "identifier: " << _identifier << std::endl; },
					[](const Mini_C::lexer::numeric_t& _num) { std::cout << "numeric: " << std::quoted(Mini_C::lexer::type2str(num_t2type(std::get<Mini_C::lexer::numeric_type>(_num)))) << " "; num_print(_num); std::cout << std::endl; },
					[](const Mini_C::lexer::string_literal_t& _str) { std::cout << "string literal: " << std::quoted(std::get<const std::string>(_str)) << std::endl; },
					[](auto) { std::cout << "WTF: tokenizer" << std::endl; },
				}, std::get<lexer::token_t>(token));
	}

	void num_print(const Mini_C::lexer::numeric_t& _num) noexcept
	{
		const static std::unordered_map<char, std::string> escapingMap = {
			{ '\t', "'\\t'" }, { '\n', "'\\n'" }, { '\r', "'\\r'" },
			{ '\a', "'\\a'" }, { '\b', "'\\b'" }, { '\f', "'\\f'" },
		};

		Mini_C::lexer::numeric_type num_t = std::get<Mini_C::lexer::numeric_type>(_num);

		if (num_t == Mini_C::lexer::numeric_type::U32)
			std::cout << static_cast<std::size_t>(std::get<double>(_num));

		else if (num_t == Mini_C::lexer::numeric_type::F32 || num_t == Mini_C::lexer::numeric_type::F64)
			printf("%lf", std::get<double>(_num));

		else if (num_t == Mini_C::lexer::numeric_type::CHAR)
		{
			const char c = static_cast<char>(std::get<double>(_num));
			if (auto it = escapingMap.find(c); it != escapingMap.end())
				std::cout << it->second;
			else std::cout << std::string("'") + c + "'";
		}

		else if (num_t == Mini_C::lexer::numeric_type::BOOLEAN)
			std::cout << (static_cast<std::size_t>(std::get<double>(_num)) ? "true" : "false");

		else std::cout << static_cast<int>(std::get<double>(_num));
	}



}
