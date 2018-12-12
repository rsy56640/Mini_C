#pragma once
#ifndef _PARSER_H
#define _PARSER_H
#include "lexer.h"
#include "miniC_exception.h"
#include <mutex>
#include <thread>
#include <sstream>

namespace Mini_C::parser
{

	struct Number {
		lexer::numeric_t _number;
		Number(const lexer::numeric_t& number) :_number(number) {}
		// TODO function
	};
	struct String {
		std::string _string;
		const std::size_t _size;
		String(const lexer::string_literal_t& string)
			:_string(std::get<0>(string)), _size(_string.size()) {}
		std::size_t size() const { return _size; }
		char& access(int pos) {
			if (pos < 0 || pos >= _size)
				throw MiniC_Runtime_Exception{
				std::string("\"") + _string + "\" access out of range!!!" };
			return _string[pos];
		}
	};
	struct TID {
		const std::thread::id _tid;
		int get_id() const
		{
			std::ostringstream os;
			os << _tid;
			return std::stoi(os.str());
		}
	};
	struct Mutex {
		std::mutex _mutex;
		void lock() { _mutex.lock(); }
		void unlock() { _mutex.unlock(); }
	};
	struct ONCE_FLAG {
		std::once_flag _once_flag;
		bool call_once() {
			bool _used = false;
			std::call_once(_once_flag, [](bool& used) { used = true; }, _used);
			return _used;
		}
	};
	struct UserClass;
	struct FuncType;

	/*
	 * Expression value and Type
	 */
	enum class valueType { number, str, tid, mutex, once_flag, user_class, func, };
	struct ValueType
	{
	private:
		bool _is_owned; // on stack or on heap.
		bool _heap_allocated; // new or deleted. 

		valueType _valueType;
		std::size_t _pointer_level;
	public:


	};


	/*
	 * struct, union
	 * non-static member: var, type_alias, func_var
	 */
	struct UserClass
	{


	};



	/*
	 * func<Ret(Args...)>
	 *
	 */
	class FuncType
	{
	private:

	public:


	};



	class Parser
	{




	};


} // end namepsace

#endif // !_PARSER_H
