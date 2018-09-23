//
//  test.cpp
//  Mini_C
//
//  Created by 潇湘夜雨 on 2018/9/23.
//  Copyright © 2018年 ssyram. All rights reserved.
//

#include "test.h"
using std::variant;
using namespace Mini_C::lexer;

const static std::unordered_map<numeric_type, std::string> numeric_id= {
    {numeric_type::CHAR, "char"}, {numeric_type::I16, "i16"},
    {numeric_type::I32, "i32"}, {numeric_type::U16, "u16"},
    {numeric_type::U32, "u32"}, {numeric_type::F32, "f32"},
    {numeric_type::F64, "f64"},
};

void outputLexVector(std::variant<std::vector<token_t>, analyzers::Token_Ex> &r) {
    std::visit(overloaded {
        [](const Mini_C::lexer::analyzers::Token_Ex& e) {
            Mini_C::MiniC_Universal_Exception{
                std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
                0u, e._position }.printException(); },
        [](const std::vector<Mini_C::lexer::token_t>& tokens) {
            for (auto const& token : tokens)
                std::visit(overloaded{
                    [](const Mini_C::lexer::type& _type) { std::cout << "<type, " << kw_ouput_map.find(_type)->second << ">" << std::endl; },
                    [](const Mini_C::lexer::identifier& _identifier) { std::cout << "<identifier, " << _identifier << ">" << std::endl; },
                    [](const Mini_C::lexer::numeric_t& _num) { std::cout << "<" << numeric_id.find(std::get<Mini_C::lexer::numeric_type>(_num))->second << ", " << std::get<double>(_num) << ">" << std::endl; },
                    [](const Mini_C::lexer::string_literal_t& _str) { std::cout << "string literal: " << std::get<const std::string>(_str) << std::endl; },
                    [](auto) { std::cout << "WTF" << std::endl; },
                }, token);
        },
    }, r);
}
