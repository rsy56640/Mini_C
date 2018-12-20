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
		char access(int pos) {
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
	 * [const_specifier]
	 * [storage duration]
	 * [if still hold the resource]
	 *
	 */
	struct ValueSemantic
	{
		bool _is_const; // const
		bool _is_owned; // on stack or on heap.
		bool _heap_allocated; // new or deleted. 
	};


	/*
	 * int *p  = &a;      // a { 0 }
	 * int **q = &p + 2;  // a { 0, 2 }
	 * **q = &p;		  // a { 0, 0 }
	 */
	struct PointerLevel
	{
	private:
		std::vector<std::ptrdiff_t> _level; // store each level ptrdiff_t
	public:
		PointerLevel() = default;
		PointerLevel(const PointerLevel& other) :_level(other._level) {}
		void AddressOf(std::size_t n) { _level.resize(_level.size() + n, 0); }
		void Dereference(std::size_t n) {}
		void Offset(std::ptrdiff_t offset) { _level.back() += offset; }
	};


	/*
	 * Expression value and Type
	 */
	enum class valueType { number, str, tid, mutex, once_flag, user_class, func, };
	struct ValueType
	{
	private:
		ValueSemantic _valueSemantic;
		valueType _valueType;
		PointerLevel _pointerLevel;
	public:


	};


	/*
	 * struct, union
	 * non-static member: var, type_alias, func_var
	 */
	 // now only support struct
	struct UserClass
	{



	};



	/*
	 * func<Ret(Args...)>
	 *
	 */
	struct FuncType
	{
	private:

	public:


	};



	class Parser
	{




	};


} // end namepsace

#endif // !_PARSER_H
