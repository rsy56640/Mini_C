#include <variant>
#include "test.h"
#include "../util/util.h"

namespace TEST
{

	using namespace Mini_C::lexer;
	using namespace Mini_C::util;

	namespace TEST_LEXER
	{
		void test_lexer(const char* s, const std::size_t line_num)
		{
			auto result = Mini_C::lexer::tokenize(s, strlen(s));
			std::visit(overloaded{
					[line_num](const Mini_C::lexer::analyzers::Token_Ex& e) {
						throw Mini_C::MiniC_Universal_Exception{
							std::move(const_cast<Mini_C::lexer::analyzers::Token_Ex&>(e)._msg),
							line_num, e._position }; },
					[line_num](const std::vector<Mini_C::lexer::token_t>& tokens) { outputLexVector(tokens); },
				}, result);
		}
	}// end namespace TEST_LEXER

}// end namespace TEST
