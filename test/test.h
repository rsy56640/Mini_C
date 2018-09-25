//
//  test.hpp
//  Mini_C
//
//  Created by 潇湘夜雨 on 2018/9/23.
//  Copyright © 2018年 ssyram. All rights reserved.
//

#ifndef test_hpp
#define test_hpp

#include "../src/lexer.h"
#include "../util/pattern.hpp"
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using namespace Mini_C::lexer;

namespace TEST
{

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

	/**
	 * way to test lexical analyzer:
	 * 		use TEST_LEXER::test_lexer(<your test string in char *>, <line number>);
	 * 		and result will be automatically output in the console.
	 * 		watch the console carefully to see if there is anything eccentric.
	 */
	namespace TEST_LEXER
	{
		void test_lexer(const char* s, const std::size_t line_num);
		void num_print(const Mini_C::lexer::numeric_t& _num) noexcept;
		void outputLexVector(const tokenize_result &result,
			const std::size_t line_num);
	}

}
#endif /* test_hpp */
