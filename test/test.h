#ifndef test_hpp
#define test_hpp
#include <string>
#include "../src/lexer.h"
#include "../test/attempt/rsy/eval_and_apply.h"

namespace Mini_C::TEST
{

	/**
	 * way to test lexical analyzer:
	 * 		use TEST_LEXER::test_lexer(<your test string in char *>, <line number>);
	 * 		and result will be automatically output in the console.
	 * 		watch the console carefully to see if there is anything eccentric.
	 */
	namespace TEST_LEXER
	{
		void test_lexer(const char* s, const std::size_t line_num);
	}

	/*
	 * print numeric value in form of formal format.
	 */
	void num_print(const lexer::numeric_t& _num, std::ostream&);

	/*
	 * output token_t
	 */
	void output_token_t(const lexer::token_t& token, std::ostream&);

	/*
	 * output the token-list when used in debug mode.
	 */
	void outputLexVector(const std::vector<Mini_C::lexer::token_info> &result, std::ostream&);


	void outputToken(const lexer::Token& token, std::ostream&);
	void outputTokenVector(const std::vector<lexer::Token>& tokens, std::ostream&);


	/*
	 * return quoted type-string
	 */
	 //std::string outputType(const parser::value_t& _type);


}
#endif /* test_hpp */
