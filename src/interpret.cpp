#include "interpret.h"

#if 0
namespace Mini_C::interpret
{

	using namespace lexer;

	const typename Env::Env_ptr Env::get_base_env()
	{
		static Env::Env_ptr _base_env = std::make_shared<Env>();
		return _base_env;
	}

	/*
	 * If no such `_name` exists, throw Env_Ex.
	 */
	value_t Env::find(const lexer::identifier& _name)
	{
		auto it = _env.find(_name);
		if (it != _env.end()) return it->second;
		throw std::string("The variable \"") + _name + "\" has not been defined.";
	}

	/*
	 * If such `_name` exists, throw Env_Ex.
	 */
	void Env::insert(const lexer::identifier& _name, const value_t& _value)
	{
		if (_env.find(_name) == _env.end())
			_env.insert(std::make_pair(_name, _value));
		else throw std::string("The variable \"") + _name + "\" has been defined twice.";
	}

	/*
	 * If no such `_name` exists,
	 * otherwise if Type-Check fails,throw Env_Ex.
	 */
	void Env::update(const lexer::identifier& _name, const value_t& _value)
	{
		auto it = _env.find(_name);

		// check wheather exists
		if (it == _env.end())
			throw std::string("The variable \"") + _name + "\" has not been defined.";

		// type check
		else if (_value.second.index() != it->second.second.index())
			throw std::string("The type of value assigned to the variable \"") + _name + "\" has not been matched: "
			+ TEST::outputType(it->second) + " to " + TEST::outputType(_value);

		// type detailed check
		// 
		// TODO
		//
		//



		// const check
		else if (it->second.first == specifier_t::const_t)
			throw std::string("const value cannot be modified after definition.");

		it->second.second = _value.second;
	}

	typename Env::Env_ptr Env::make_env(const Env_ptr& cur_env_ptr,
		const std::vector<std::pair<identifier, value_t>>& param_list)
	{
		Env_ptr new_env = std::make_shared<Env>(cur_env_ptr);
		for (auto const&[id, v] : param_list)
			insert(id, v);
		return new_env;
	}




} // end namespace Mini_C::interpret
#endif