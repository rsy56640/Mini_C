//
//  lexer.cpp
//  Mini_C
//
//  Created by 潇湘夜雨 on 2018/9/21.
//  Copyright © 2018年 ssyram. All rights reserved.
//

#include "lexer.h"
#include <vector>
#include <functional>
#include <unordered_set>

// As lexical analyzer, I must assume that except for the appearance of some unknown character which is definitely wrong input, the input is all right.
// the mission of it is to divide them into the right sequence, give each of them the type that as fidelity as possible and the corresponding right value, if it has.

using namespace Mini_C::lexer;
using std::unordered_set;
using std::size_t;
using std::function;
using std::vector;
using std::string;
using std::variant;
using std::make_tuple;
using std::unordered_map;
using keyword_it = unordered_map<string, type>::const_iterator;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

namespace Mini_C::lexer {
    std::string type2str(type _type)
    {
        auto it = keyword2str.find(_type);
        if (it == keyword2str.end())return "No such type";
        return it->second;
    }
    type num_t2type(numeric_type num_t) {
        return static_cast<type>(static_cast<std::size_t>(type::CHAR)
                                 + static_cast<std::size_t>(num_t) - static_cast<std::size_t>(numeric_type::CHAR));
    }
}

/**
 * analyzers
 */
namespace Mini_C::lexer::analyzers {
    //if returns true, means that it has tackled the word.
    //thus it doesn't need to worry about if the pos == size at the begginning of analyzers.
    using analyzer = function<bool(const char *, size_t&, const size_t, vector<token_t> &)>;

    namespace supporters {
        const static unordered_map<char, char> escapingMap = {
                { 't', '\t' }, { 'n', '\n' }, { 'r', '\r' }, { 'a', '\a' }, { 'b', '\b' }, { 'f', '\f' }
        };
        const static unordered_set<char> combindableOperatorSet = {
                '|', '&', '+', '^', '*', '/', '!', '.', ':', '=', '<', '>', '%'
        };
        const static unordered_set<char> singleOperatorSet = {
                '(', ')', '{', '}', '[', ']', ';', ',', '?'
        };
        const static unordered_set<char> secondCombinableOperatorSet = {
                '+', '=', '|', '&', '.', ':', '<', '>'
        };
        const static unordered_set<char> dividerCharSet = {
                ' ', '\n', '\t'
        };
        const static unordered_set<char> minusCharSet = {
                '-', '=', '>'
        };

        inline bool inMinusCharSet(char c) {
            return minusCharSet.find(c) != minusCharSet.end();
        }
        inline bool isDivider(char c) {
            return dividerCharSet.find(c) != dividerCharSet.end();
        }
        inline bool isWordBeginning(char c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        }
        inline bool isNum(char c) {
            return c >= '0' && c <= '9';
        }
        inline bool isNumBegin(char c) {
            return isNum(c) || c == '.';
        }
        inline bool canInWord(char c) {
            return isWordBeginning(c) || isNum(c);
        }
        inline bool isCombindableOperatorChar(char c) {
            return combindableOperatorSet.find(c) != combindableOperatorSet.end();
        }
        inline bool isSingleSymbolChar(char c) {
            return singleOperatorSet.find(c) != singleOperatorSet.end();
        }
        inline bool isSecondCombinableOpearatorChar(char c) {
            return secondCombinableOperatorSet.find(c) != secondCombinableOperatorSet.end();
        }
        inline bool isOctNum(char c) {
            return c >= '0' && c <= '7';
        }

        const static unordered_set<type> unminusable_type_corresponding_string = {
                type::SELF_INC, type::SELF_DEC, type::RIGHT_PARENTHESIS, type::RIGHT_SQUARE_BRACKETS, type::RIGHT_CURLY_BRACKETS
        };
        bool rIsInUnminusableSituation(vector<token_t> &r) {
            if (!r.size())
                return false;
            auto&& rb = r.back();
            return std::visit([&](auto&& r)->bool {
                using t = std::decay_t<decltype(r)>;
                if constexpr (std::is_same_v<t, identifier>)
                    return true;
                else if constexpr (std::is_same_v<t, numeric_t>)
                    return true;
                else if constexpr (std::is_same_v<t, type>)
                    if (unminusable_type_corresponding_string.find(std::get<type>(rb)) != unminusable_type_corresponding_string.end())
                        return true;
                return false;
            }, rb);
        }
        inline bool isHex(char c) {
            return isNum(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        }
        inline int turnHex(char c) {
            if (isNum(c))
                return c - '0';
            else if (c >= 'a' && c <= 'f')
                return c - 'a' + 10;
            else
                return c - 'A' + 10;
        }

        // unnecessity
        // follow numbers
        const static unordered_set<char> followNumberCharSet = { '+', '-', '*', '/', ')', ']', '}', '|', '&', '^', ',', ':', ';', '%', '=' };
        inline bool canFollowNumber(char c) {
            return followNumberCharSet.find(c) != followNumberCharSet.end();
        }

        // for calculator
        const static unordered_set<char> calculatorNormalOperatorCharSet = { '+', '*', '/', '|', '&', '^', '~', '(', ')' };
        inline bool isInCalculatorNormalOperatorCharSet(char c) {
            return calculatorNormalOperatorCharSet.find(c) != calculatorNormalOperatorCharSet.end();
        }
        // end for calculator
    }

#define write_analyzer(funcname) bool funcname(const char *s, size_t& pos, const size_t size, vector<token_t> &r)

    write_analyzer(word_analyzer) {
        if (!supporters::isWordBeginning(s[pos]))
            return false;
        size_t length = 1;
        for (; supporters::canInWord(s[pos + length]); length++);


        string ns(s + pos, length);
        keyword_it it = keywords.find(ns);
        if (it != keywords.end())
            if (it->second == type::TRUE)
                r.push_back(make_tuple(1.0, numeric_type::BOOLEAN));
            else if (it->second == type::FALSE)
                r.push_back(make_tuple(0.0, numeric_type::BOOLEAN));
            else
                r.push_back(it->second);
        else
            r.push_back(std::move(ns));

        pos += length;

        return true;
    }

    // target:
    //      to get the longest string of operators
    // organization:
    //      1. get the longest string of operators
    //      2. loop to get the longest one that conforms to the rules
    write_analyzer(combindable_operator_analyzer) {
        if (!supporters::isCombindableOperatorChar(s[pos]))
            return false;

        size_t begin = pos;
        // for that the end of string is '\0' which is not combindable operator char, so there is no need to check the boundary

        // here uses a somehow ambiguous analyze process
        // for that it does not need a rigid DFA for every single combinable openning character
        while (supporters::isSecondCombinableOpearatorChar(s[pos++]));
        // loop to change
        keyword_it it;
        for (; (it = keywords.find(string(s + begin, pos - begin))) == keywords.end(); --pos);
        r.push_back(it->second);

        return true;
    }

    write_analyzer(single_symbol_analyzer) {
        if (!supporters::isSingleSymbolChar(s[pos]))
            return false;

        r.push_back(keywords.find(string(1, s[pos++]))->second); // bug when input is: "(3+4.2)-abc+32"
        return true;
    }

    // returns the char value in int
    int escapeTackle(const char *s, size_t& pos, const size_t size, vector<token_t> &r) {

        int value = 0;

        auto octNumTackle = [&]() {
            value = s[pos++];
            // here pos is in first number
            for (size_t begin = pos; supporters::isOctNum(s[pos]) && pos < begin + 2; ++pos)
                value = value * 8 + (s[pos] - '0');
        };

        auto hexNumTackle = [&]() {
            // here pos is in 'x'
            if (!supporters::isHex(s[++pos])) { // point to and get char next to 'x'
                value = 'x';
                return;
            }
            value = supporters::turnHex(s[pos++]);
            for (size_t begin = pos; supporters::isHex(s[pos]) && pos < begin + 1; pos++)
                value = value * 16 + supporters::turnHex(s[pos]);
        };

        auto charTackle = [&]() {
            char c = s[pos++]; // no need to point to next in the following
            unordered_map<char, char>::const_iterator p = supporters::escapingMap.find(c);
            if (p != supporters::escapingMap.end())
                value = p->second;
            else // like \', \", \\ that's just the same with '\c' which means 'c' itself
                value = c;
        };

        ++pos;
        char c = s[pos];
        if (supporters::isOctNum(c))
            octNumTackle();
        else if (c == 'x')
            hexNumTackle();
        else
            charTackle();


        return value;
    }

    write_analyzer(char_analyzer) {
        if (s[pos] != '\'')
            return false;

        if (s[++pos] == '\\') {
            r.push_back(make_tuple((double)escapeTackle(s, pos, size, r), numeric_type::CHAR));
            return true;
        }

        if (pos + 1 >= size || s[pos + 1] != '\'')
            throw Token_Ex("there should be only one character in \'\'", pos);
        r.push_back(std::make_tuple((double)s[pos], numeric_type::CHAR));
        pos += 2;

        return true;
    }

    write_analyzer(string_analyzer) {
        if (s[pos] != '"')
            return false;

        string ns;

        for (++pos; pos < size && s[pos] != '"'; ++pos)
            if (s[pos] == '\\')
                ns += (char)escapeTackle(s, pos, size, r);
            else
                ns += s[pos];

        if (pos == size)
            throw Token_Ex("expected a corresponding '\"'", pos);

        r.push_back(make_tuple(ns));


        return true;
    }

    // only get the number, but can be specified to accept the condition that tells whether it's minus
    bool inner_number_analyzer(const char *s, size_t &pos, const size_t size, vector<token_t> &r, const bool isMinus = false) {

        auto generateNumberException = [&]() {
            throw Token_Ex("not a valid number", pos);
        };

        auto hexAnalyzer = [&]() {
            pos += 2;
            if (isMinus || !supporters::isHex(s[pos]))
                generateNumberException();

            double value = supporters::turnHex(s[pos]);
            while (true)
                if (supporters::isHex(s[++pos]))
                    value = value * 16 + supporters::turnHex(s[pos]);
                else if (s[pos] == '_')
                    continue;
                else
                    break;

            // unnecessity
            // check if it's followed by '.' to generate things.
            //            while (supporters::isDivider(s[pos])) ++pos;
            //            char c = s[pos];
            //            if (c == '.')
            //                generateNumberException();

            r.push_back(make_tuple(value, numeric_type::U32));
        };

        auto defaultAnalyzer = [&]() {
            double value = 0;
            numeric_type type = numeric_type::I32;

            auto decimalAnalyzer = [&]() {
                // here pos is where '.'/'e'/'E' appears
                int decimal = 0;
                type = numeric_type::F32;

                auto f64Analyzer = [&]() {
                    // here pos is where e/E appears
                    ++pos;
                    bool rminus = false;
                    if (s[pos] == '-') {
                        rminus = true;
                        ++pos;
                    }
                    // for '\0' is definitely not num, no need to check the boundary
                    if (!supporters::isNum(s[pos]))
                        generateNumberException();

                    // previous(wrong) implemetation
//                    if (s[pos] != '-' && !supporters::isNum(s[pos]))
//                        generateNumberException();
//
//                    int right = 0;
//                    bool rminus = false;
//                    if (s[pos] == '-') {
//                        rminus = true;
//                        ++pos;
//                    }

                    int right = (s[pos] - '0');
                    type = numeric_type::F64;

                    while (true)
                        if (supporters::isNum(s[++pos]))
                            right = right * 10 + (s[pos] - '0');
                        else if (s[pos] == '_')
                            continue;
                        else
                            break;

                    r.push_back(make_tuple((isMinus ? -value : value) * pow(10, rminus ? -right : right), type));
                };

                //default is f32 analyzer
                for (; true; ++pos) {
                    char c = s[pos];
                    if (supporters::isNum(c))
                        value += (c - '0') * pow(10, decimal--);
                    else if (c == '_')
                        continue;
                    else if (c == '.')
                        if (!decimal)
                            --decimal;
                        else
                            generateNumberException();
                    else if (c == 'e' || c == 'E') {
                        f64Analyzer();
                        return;
                    }
                    else
                        break;
                }

                r.push_back(make_tuple((isMinus ? -value : value), type));
            };


            //default is i32 analyzer.
            for (; true; ++pos) {
                char c = s[pos];
                if (supporters::isNum(c))
                    value = value * 10 + (c - '0');
                else if (c == '_')
                    continue;
                else if (c == '.' || c == 'e' || c == 'E') {
                    decimalAnalyzer();
                    return;
                }
                else if (c == 'u' || c == 'U') {
                    if (isMinus)
                        generateNumberException();
                    type = numeric_type::U32;
                    break;
                }
                else
                    break;
            }

            r.push_back(make_tuple((isMinus ? -value : value), type));
        };





        if (s[pos] == '0' && pos + 1 < size && s[pos + 1] == 'x')
            hexAnalyzer();
        else
            defaultAnalyzer();

        // unneccesity
        // see if the following char is allowed
        while (supporters::isDivider(s[pos])) ++pos;
        if (pos < size && !supporters::canFollowNumber(s[pos]))
            throw Token_Ex("not valid following content", pos);

        return true;
    }

    write_analyzer(number_analyzer) {
        if (!supporters::isNumBegin(s[pos]))
            return false;

        return inner_number_analyzer(s, pos, size, r);
    }

    write_analyzer(minus_analyzer) {
        if (s[pos] != '-')
            return false;

        size_t begin = pos;
        // to the next meaningful char
        while (++pos < size && supporters::isDivider(s[pos]));
        // see if it's minus
        if (supporters::isNumBegin(s[pos]) && !supporters::rIsInUnminusableSituation(r))
            return inner_number_analyzer(s, pos, size, r, true);
        else
            // there exists divider(s) or it's the end, or it could not be
            // a reasonable char
        if (begin != pos - 1 || pos == size || supporters::inMinusCharSet(s[pos]))
            r.push_back(keywords.find("-")->second);
        else { //pos == begin + 1
            r.push_back(keywords.find(string(s + begin, 2))->second);
            ++pos; // for when leaving the function, pos should always be in the position of the next char to be identified
        }

        return true;
    }


    // for calculator
    write_analyzer(calculator_analyzer) {
        if (number_analyzer(s, pos, size, r))
            return true;

        char c = s[pos++];
        if (c == '-') {
            // to the next meaningful char
            while (supporters::isDivider(s[pos])) ++pos;
            // see if it's minus
            if (supporters::isNumBegin(s[pos]) && !supporters::rIsInUnminusableSituation(r))
                return inner_number_analyzer(s, pos, size, r, true);
            else
                r.push_back(keywords.find("-")->second);
        }
        else if (c == '<' || c == '>')
            if (c == s[pos])
                r.push_back(keywords.find(string(2, s[pos++]))->second); // here must have a pos++ to let the pointer point to the next position, otherwise, the second reasonable '<'/'>' must be judge as exception
            else
                throw Token_Ex("not a valid operator.", pos - 1);
        else if (supporters::isInCalculatorNormalOperatorCharSet(c))
            r.push_back(keywords.find(string(1, c))->second);
        else
            throw Token_Ex("not a valid character.", pos - 1);

        return true;
    }
    // end for calculator
#undef write_analyzer
}
const int analyzerNum = 7; //7 in normal, 1 in calculator
analyzers::analyzer analyzer[] = { //analyzers::calculator_analyzer in calculator
    analyzers::word_analyzer,
    analyzers::number_analyzer,
    analyzers::minus_analyzer,
    analyzers::single_symbol_analyzer,
    analyzers::combindable_operator_analyzer,
    analyzers::char_analyzer,
    analyzers::string_analyzer,
//        analyzers::calculator_analyzer,
};
namespace Mini_C::lexer
{
    std::variant<std::vector<token_t>, analyzers::Token_Ex> tokenize(const char *s, const size_t size) {
        vector<token_t> r;
        bool ok;
        for (size_t pos = 0; pos < size; ) {
            ok = false;
            if (analyzers::supporters::isDivider(s[pos])) {
                ++pos;
                continue;
            }
            for (size_t i = 0; i < analyzerNum; i++) {
                //if it returns true, means it've done, and there's no need to tackle this round again
                try {
                    ok = analyzer[i](s, pos, size, r);
                    if (ok)
                        break;
                }
                catch (analyzers::Token_Ex& e) {
                    return e;
                }
            }
            if (!ok)
                return analyzers::Token_Ex("not a recognizable character.", pos);
        }

        return r;
    }
}
