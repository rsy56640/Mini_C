#pragma once
#include <string>
#include <iostream>

namespace Mini_C
{
	class MiniC_Base_Exception
	{
	public:
		void printException() const { this->printEx(); }
	private:
		virtual void printEx() const = 0;
	};


	/*
	 * Universal Exception class.
	 *     display specific error message, line num, and position.
	 */
	class MiniC_Universal_Exception : public MiniC_Base_Exception
	{
		const std::string _msg;
		const std::size_t _line, _position;
		virtual void printEx() const { std::cout << (*this) << std::endl; }
	public:
		MiniC_Universal_Exception(const std::string& msg, std::size_t line, std::size_t position)
			:_msg(msg), _line(line), _position(position) {}
		MiniC_Universal_Exception(std::string&& msg, std::size_t line, std::size_t position)
			:_msg(std::move(msg)), _line(line), _position(position) {}
		friend std::ostream& operator<<(std::ostream& os, const MiniC_Universal_Exception& e)
		{
			os << e._msg << " in the line: " << e._line << ", at position: " << e._position;
			return os;
		}
	};

	class MiniC_Runtime_Exception
	{
		const std::string _msg;
		virtual void printEx() const { std::cout << (*this) << std::endl; }
	public:
		MiniC_Runtime_Exception(const std::string& msg) :_msg(msg) {}
		MiniC_Runtime_Exception(std::string&& msg) :_msg(std::move(msg)) {}
		friend std::ostream& operator<<(std::ostream& os, const MiniC_Runtime_Exception& e)
		{
			os << "Runtime Error: " << e._msg;
			return os;
		}
	};

}