#pragma once
#ifndef _EVAL_AND_APPLY_H
#define _EVAL_AND_APPLY_H
#include "../../../src/lexer.h"


namespace Mini_C::parser
{

	using num_t = std::tuple<double, lexer::numeric_type>;
	struct pointer_t {};
	struct str_t {};
	struct class_t {};
	struct union_t {};

	enum class specifier_t { const_t, static_t };
	using value_t = std::pair<specifier_t,
		std::variant<num_t, pointer_t, str_t, class_t, union_t>>;

	// constraint on variables
	class Env
	{
		using Env_Ex = std::string;
		using Env_ptr = std::shared_ptr<Env>;

		std::unordered_map<lexer::identifier, value_t> _env;
		const Env_ptr _prev_env_p;

		template<typename> friend class std::_Ref_count_obj;
		Env(const Env_ptr& prev = nullptr) :_prev_env_p(prev) {}

		static const Env_ptr get_base_env();

	public:

		Env(const Env&) = delete;
		Env& operator=(const Env&) = delete;
		Env(Env&&) = delete;
		Env& operator=(Env&&) = delete;

		/*
		 * If no such `_name` exists, throw Env_Ex.
		 */
		value_t find(const lexer::identifier& _name);

		/*
		 * If such `_name` exists, throw Env_Ex.
		 */
		void insert(const lexer::identifier& _name, const value_t& _value);

		/*
		 * If no such `_name` exists,
		 * otherwise if Type-Check fails,throw Env_Ex.
		 */
		void update(const lexer::identifier& _name, const value_t& _value);

		Env_ptr make_env(const Env_ptr& cur_env_ptr,
			const std::vector<std::pair<lexer::identifier, value_t>>& param_list);

	};


	class Func_Set
	{

	};

	class Type_Set
	{

	};


	void eval();


	void apply();


} // end namespace Mini_C::parser

#endif // _EVAL_AND_APPLY_H
