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

/**
 * analyzers
 */
namespace Mini_C::lexer::analyzers {
    //if returns true, means that it has tackled the word.
    //thus it doesn't need to worry about if the pos == size at the begginning of analyzers.
    using analyzer = function<bool(const char *, size_t&, const size_t, vector<token_t> &)>;
    
    namespace supporters {
        const static unordered_set<char> combindableOperatorSet = {
            '|', '&', '^', '*', '/', '!', ':', '=', '<', '>'
        };
        const static unordered_set<char> singleOperatorSet = {
            '(', ')', '{', '}', '[', ']', ';', '.', ',', '?'
        };
        inline bool isWordBeginning(char c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        }
        inline bool isNum(char c) {
            return c >= '0' && c <= '9';
        }
        inline bool canInWord(char c) {
            return isWordBeginning(c) && isNum(c);
        }
        inline bool isCombindableOperatorChar(char c) {
            return combindableOperatorSet.find(c) != combindableOperatorSet.end();
        }
        inline bool isSingleSymbolChar(char c) {
            return singleOperatorSet.find(c) != singleOperatorSet.end();
        }
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
            r.push_back(it->second);
        else
            r.push_back(std::move(ns));
        
        pos += length;
        
        return true;
    }
    
    write_analyzer(combindable_operator_analyzer) {
        if (!supporters::isCombindableOperatorChar(s[pos]))
            return false;
        size_t begin = pos++;
        while (supporters::isCombindableOperatorChar(s[pos++]));
        
        string ns(s + begin, pos - begin);
        keyword_it it = keywords.find(ns);
        if (it != keywords.end())
            r.push_back(it->second);
        else
            throw Token_Ex("not a valid operator", begin);
        
        return true;
    }
    
    write_analyzer(single_symbol_analyzer) {
        if (!supporters::isSingleSymbolChar(s[pos]))
            return false;
        
        ++pos;
        r.push_back(keywords.find(string(1, s[pos]))->second);
        return true;
    }
    
    write_analyzer(number_analyzer) {
        if (!supporters::isNum(s[pos]))
            return false;
    }
    
    write_analyzer(minus_analyzer) {
        if (s[pos] != '-')
            return false;
        
        
        return true;
    }
#undef write_analyzer
}
const int analyzerNum = 6;
analyzers::analyzer analyzer[6];

vector<token_t> tokenize(const char *s, const size_t size) {
    vector<token_t> r;
    bool ok;
    for (size_t pos = 0; pos < size; ) {
        ok = false;
        if (s[pos] == ' ') {
            ++pos;
            continue;
        }
        for (size_t i = 0; i < analyzerNum; i++) {
            //if it returns true, means it've done, and there's no need to tackle this round again
            ok = analyzer[i](s, pos, size, r);
            if (ok)
                break;
        }
        if (!ok)
            throw analyzers::Token_Ex("not a recognizable character.", pos);
    }
    
    return r;
}
