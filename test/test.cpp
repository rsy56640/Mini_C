//
//  test.cpp
//  Mini_C
//
//  Created by 潇湘夜雨 on 2018/9/23.
//  Copyright © 2018年 ssyram. All rights reserved.
//

#include "../test/test.h"
#include <iomanip>
using std::variant;
using namespace Mini_C::lexer;
namespace TEST
{

	namespace TEST_LEXER
	{
		void outputLexVector(const std::variant<std::vector<token_t>, analyzers::Token_Ex> &result, const std::size_t line_num)
		{
			std::visit(overloaded{
				[line_num](const Mini_C::lexer::analyzers::Token_Ex& e) {
				throw Mini_C::MiniC_Universal_Exception{
					std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
					line_num, e._position }; },
				[](const std::vector<Mini_C::lexer::token_t>& tokens) {
					for (auto const& token : tokens)
						std::visit(overloaded{
							[](const Mini_C::lexer::type& _type) { std::cout << "type: " << std::quoted(Mini_C::lexer::type2str(_type)) << std::endl; },
							[](const Mini_C::lexer::identifier& _identifier) { std::cout << "identifier: " << _identifier << std::endl; },
							[](const Mini_C::lexer::numeric_t& _num) { std::cout << "numeric: " << std::quoted(Mini_C::lexer::type2str(num_t2type(std::get<Mini_C::lexer::numeric_type>(_num)))) << " "; num_print(_num); std::cout << std::endl; },
							[](const Mini_C::lexer::string_literal_t& _str) { std::cout << "string literal: " << std::quoted(std::get<const std::string>(_str)) << std::endl; },
							[](auto) { std::cout << "WTF: tokenizer" << std::endl; },
							}, token); },
				}, result);
		}

		void num_print(const Mini_C::lexer::numeric_t& _num) noexcept
		{
			Mini_C::lexer::numeric_type num_t = std::get<Mini_C::lexer::numeric_type>(_num);
			if (num_t == Mini_C::lexer::numeric_type::U32)
				std::cout << static_cast<std::size_t>(std::get<double>(_num));
			else if (num_t == Mini_C::lexer::numeric_type::F32 || num_t == Mini_C::lexer::numeric_type::F64)
				printf("%lf", std::get<double>(_num));
			else if (num_t == Mini_C::lexer::numeric_type::BOOLEAN) {
				if (static_cast<std::size_t>(std::get<double>(_num)))
					std::cout << "true";
				else std::cout << "false";
			}
			else std::cout << static_cast<int>(std::get<double>(_num));
		}

		void test_lexer(const char* s, const std::size_t line_num)
		{
			auto result = Mini_C::lexer::tokenize(s, strlen(s));
			outputLexVector(result, line_num);
		}
	}


}// end namespace TEST