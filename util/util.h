#ifndef _UTIL_H
#define _UTIL_H

namespace Mini_C::util
{
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;






}

#endif // !_UTIL_H
