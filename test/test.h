#ifndef test_hpp
#define test_hpp

namespace TEST
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



}
#endif /* test_hpp */
