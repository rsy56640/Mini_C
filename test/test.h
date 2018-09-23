//
//  test.hpp
//  Mini_C
//
//  Created by 潇湘夜雨 on 2018/9/23.
//  Copyright © 2018年 ssyram. All rights reserved.
//

#ifndef test_hpp
#define test_hpp

#include "lexer.h"
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
using namespace Mini_C::lexer;

const static std::unordered_map<Mini_C::lexer::type, std::string> kw_ouput_map = {
    { (Mini_C::lexer::type)17, ")" }, { (Mini_C::lexer::type)16, "(" }, { (Mini_C::lexer::type)11, "f32" }, { (Mini_C::lexer::type)13, "," }, { (Mini_C::lexer::type)9, "u16" }, { (Mini_C::lexer::type)20, "{" }, { (Mini_C::lexer::type)14, "." }, { (Mini_C::lexer::type)7, "i16" }, { (Mini_C::lexer::type)18, "[" }, { (Mini_C::lexer::type)6, "char" }, { (Mini_C::lexer::type)10, "u32" }, { (Mini_C::lexer::type)5, "--" }, { (Mini_C::lexer::type)12, "f64" }, { (Mini_C::lexer::type)8, "i32" }, { (Mini_C::lexer::type)21, "}" }, { (Mini_C::lexer::type)2, "*" }, { (Mini_C::lexer::type)1, "-" }, { (Mini_C::lexer::type)4, "++" }, { (Mini_C::lexer::type)19, "]" }, { (Mini_C::lexer::type)15, ";" }, { (Mini_C::lexer::type)3, "/" }, { (Mini_C::lexer::type)0, "+" },
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

void outputLexVector(std::variant<std::vector<token_t>, analyzers::Token_Ex> &r);

#endif /* test_hpp */
