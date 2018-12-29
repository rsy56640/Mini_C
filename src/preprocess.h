#pragma once
#ifndef _PREPROCESS_H
#define _PREPROCESS_H
#include <string>
#include <iostream>

namespace Mini_C::preprocess
{

	[[nodiscard]] const std::string preprocess(const std::string& file_name, std::ostream&);

} // end namespace Mini_C::preprocess

#endif // !_PREPROCESS_H