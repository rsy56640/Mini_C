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
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using namespace Mini_C::lexer;


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

void outputLexVector(std::variant<std::vector<token_t>, analyzers::Token_Ex> &r);

#endif /* test_hpp */
