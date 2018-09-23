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

void outputLexVector(std::variant<std::vector<token_t>, analyzers::Token_Ex> &r) {
	std::visit(overloaded{
		[](const Mini_C::lexer::analyzers::Token_Ex& e) {
		throw Mini_C::MiniC_Universal_Exception{
			std::move(const_cast<analyzers::Token_Ex&>(e)._msg),
			0u, e._position }; },
			[](const std::vector<token_t>& tokens) {
			for (auto const& token : tokens)
				std::visit(overloaded{
				[](const Mini_C::lexer::type& _type) { std::cout << "type: " << type2str(_type) << std::endl; },
				[](const Mini_C::lexer::identifier& _identifier) { std::cout << "identifier: " << _identifier << std::endl; },
				[](const Mini_C::lexer::numeric_t& _num) { std::cout << "numeric: " << std::quoted(type2str(num_t2type(std::get<numeric_type>(_num)))) << " " << std::get<double>(_num) << std::endl; },
				[](const Mini_C::lexer::string_literal_t& _str) { std::cout << "string literal: " << std::quoted(std::get<const std::string>(_str)) << std::endl; },
				[](auto) { std::cout << "WTF: tokenizer" << std::endl; },
					}, token); },
		}, r);
}
