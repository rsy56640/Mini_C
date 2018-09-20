#include "lexer.h"

namespace Mini_C::lexer
{

	namespace tokenize_util
	{
		bool isKeyWord(const std::string& str) { return keywords.find(str) != keywords.end(); }
		bool isKeyWord(char c) { return keywords.find(std::string(1, c)) != keywords.end(); }
	}



}// end namespace Mini_C::lexer