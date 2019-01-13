#include <iomanip>
#include <fstream>
#include <list>
#include <deque>
#include <unordered_map>
#include <string_view>
#include <cctype>
#include <tuple>
#include <functional>

#include "miniC_exception.h"


namespace Mini_C::preprocess
{

	namespace
	{

		/*
		 * Param
		 *     line :
		 *     pos  : from pos to detect identifier // pos <= sizes
		 * Return
		 *     identifier, pos after identifier
		 *     if pos == -1, means no identifier !!!
		 * No exception thrown
		 */
		[[nodiscard]] std::pair<std::string, int>
			get_identifier(std::string_view line, int pos, bool only_id = true) noexcept
		{
			const std::size_t size = line.size();
			while (pos < size && line[pos] == ' ') pos++;
			if (only_id)
			{
				if (pos == size || (!std::isalpha(line[pos]) && line[pos] != '_'))
					return std::pair<std::string, int>({}, -1);
			}
			else {
				if (pos == size || (!std::isalpha(line[pos]) && line[pos] != '_' && !std::isdigit(line[pos])))
					return std::pair<std::string, int>({}, -1);
			}
			int start = pos++;
			for (; pos < size; pos++)
				if (!std::isalpha(line[pos])
					&& !std::isdigit(line[pos])
					&& line[pos] != '_')
					break;
			return std::pair<std::string, int>(line.substr(start, pos - start), pos);
		}


		using arg_t = std::string;
		using param_t = std::string;

		class Macros
		{

		public:
			/*
			* null     : #define x
			* object   : #define xx ...
			* function : #define foo(a, b) ...
			*/
			enum class macro_t { null, object, function };

		private:
			class DefineBlock
			{
			public:
				DefineBlock(std::string macro, macro_t macro_type, std::size_t line_num)
					:_macro(std::move(macro)), _macro_type(macro_type), _line_num(line_num) {}

				// throw MiniC_Universal_Exception
				void push_arg(arg_t arg, std::size_t pos)
				{
					for (auto const& _arg : _args)
						if (_arg == arg)
							throw MiniC_Universal_Exception(
								std::string("\"") + arg + "\" has already appeared in the macro",
								_line_num, pos);
					_args.push_back(std::move(arg));
				}

				void push_replace(std::string replace) { _replace.push_back(std::move(replace)); }

				std::list<std::string> get_replace() const { return _replace; }
				std::deque<arg_t> get_args() const { return _args; }

				// for debug
				void print(std::ostream& out) const
				{
					out << "macro \"" << _macro << "\" in line " << _line_num << "\n";
					if (!_args.empty())
					{
						out << "\targs:\n";
						for (auto const& arg : _args)
							out << "\t\t" << arg << std::endl;
					}
					if (!_replace.empty())
					{
						out << "\treplace:\n";
						for (auto const& replace : _replace)
							out << "\t\t" << replace << std::endl;
					}
					out << "------------------------------------------------\n";
				}

				const macro_t _macro_type;
				const std::string _macro;
				const std::size_t _line_num;

			private:
				std::deque<arg_t> _args;
				std::list<std::string> _replace;
			}; // end class DefineBlock;

			static std::unordered_map<std::string, DefineBlock> macros;

		public:

			// throw MiniC_Universal_Exception for macro collision
			static void define_macro(const std::string& macro, macro_t macro_type, std::size_t line)
			{
				if (macros.count(macro))
					throw MiniC_Universal_Exception(
						std::string("The macro \"") + macro + "\" has already been defined",
						line, 0);
				macros.insert(
					std::pair<const std::string, DefineBlock>
					(macro, DefineBlock{ macro, macro_type, line }));
			}

			// throw MiniC_Universal_Exception for undefining non macro
			static void undef_macro(const std::string& macro, std::size_t line, std::size_t pos)
			{
				auto it = macros.find(macro);
				if (it == macros.end())
					throw MiniC_Universal_Exception(
						std::string("The macro \"") + macro + "\" has not been defined yet",
						line, pos);
				macros.erase(it);
			}

			// throw MiniC_Universal_Exception for arg collision
			static void push_arg(const std::string& macro, std::string arg, std::size_t pos)
			{
				auto pBlock = macros.find(macro);
				pBlock->second.push_arg(std::move(arg), pos);
			}

			static void push_replace(const std::string& macro, std::string replace)
			{
				macros.find(macro)->second.push_replace(std::move(replace));
			}

			// make replacement for one line
			// throw MiniC_Universal_Exception if macro format does not meet
			// return (line_str, line_num, change_line)
			static std::list<std::tuple<std::string, std::size_t, bool>>
				replace(std::string_view line, const std::size_t line_num, bool change_line)
			{
				const std::size_t size = line.size();
				std::size_t pos = 0;
				std::list<std::tuple<std::string, std::size_t, bool>> ret;

				// throw MiniC_Universal_Exception if macro format does not meet 
				std::function<void(const std::string&, std::size_t)> replace_macro
					= [&replace_macro, &ret, size, line, line_num]
					(const std::string& macro, std::size_t pos)->void
				{
					pos += macro.size();
					auto const pb = macros.find(macro);
					const DefineBlock& block = pb->second;

					if (block._macro_type == macro_t::null)
						throw MiniC_Universal_Exception(
							"Expected something for macro \"" + block._macro + "\"",
							line_num, pos);

					if (block._macro_type == macro_t::object)
					{
						const std::list<std::string> replaces = block.get_replace();
						for (auto it = replaces.begin(); it != replaces.end(); ++it)
							ret.push_back(std::make_tuple(*it, line_num, true));
						std::get<2>(ret.back()) = false;
					} // end replace "macro_t::object"

					else if (block._macro_type == macro_t::function)
					{
						// "foo()" or "foo(a, b, c)"
						std::deque<param_t> params;
						std::pair<std::string, int> p;

						if (line[pos] != '(')
							throw MiniC_Universal_Exception(
								"Expected \"(\" after macro \"" + block._macro + "\"",
								line_num, pos);
						pos++;

						const std::deque<std::string> args = block.get_args();
						const std::size_t args_size = args.size();

						// detect "foo()"
						if (pos == size)
						{
							if (args_size == 0)
								throw MiniC_Universal_Exception{
								"Expected \")\" for macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos) };
							else throw MiniC_Universal_Exception{
								"Expected " + std::to_string(args_size)
								+ " parameters for macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos) };
						}

						int _case = 0;
						if (line[pos] == ')')
						{
							_case = 2;
							pos++;
						}
						if (_case == 0)
						{
							p = get_identifier(line, pos, false);
							if (p.second == -1)
								throw MiniC_Universal_Exception{
								"Incorrect format in macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos) };
							pos = p.second;
							params.push_back(p.first);
						}


						// case   -> id -> case 0
						// detect: *(, id) ")" or 
						// case 0 -> ,  -> case 1
						// case 1 -> id -> case 0
						// case 0 -> )  -> case 2 -> ok
						//          ,  id   )
						// case0:   1, -1,  2 
						// case1:  -1,  0, -1
						std::size_t prev_pos = 0;
						auto throw_ex_case_0 = [line, args_size, &params, macro, line_num, &prev_pos]()->void
						{
							const int size_diff = static_cast<int>(args_size)
								- static_cast<int>(params.size());

							if (size_diff > 0)
								throw MiniC_Universal_Exception{
										"Expected \",\" and "
										+ std::to_string(size_diff)
										+ " more parameters in function-macro \""
										+ macro + "\"",
										line_num, static_cast<std::size_t>(prev_pos + params.back().size()) };
							else if (size_diff == 0)
								throw MiniC_Universal_Exception{
										"Expected \")\" after function-macro \""
										+ macro + "\"",
										line_num, static_cast<std::size_t>(prev_pos + params.back().size()) };
							else // size_diff < 0
								throw MiniC_Universal_Exception{
										"Expected "
										+ std::to_string(args_size)
										+ " parameters but given "
										+ std::to_string(params.size())
										+ " in function-macro \""
										+ macro + "\"",
										line_num, static_cast<std::size_t>(prev_pos + params.back().size()) };
						};
						auto throw_ex_case_1 = [line, args_size, &params, macro, line_num, &prev_pos]()->void
						{
							const int size_diff = static_cast<int>(args_size)
								- static_cast<int>(params.size());

							if (size_diff > 0)
								throw MiniC_Universal_Exception{
										"Expected "
										+ std::to_string(size_diff)
										+ " more parameters in function-macro \""
										+ macro + "\" after \",\"",
										line_num, static_cast<std::size_t>(prev_pos + 1) };
							else if (size_diff == 0)
								throw MiniC_Universal_Exception{
										"Unexpected \",\" in function-macro \""
										+ macro + "\" but expect \")\"",
										line_num, static_cast<std::size_t>(prev_pos) };
							else // size_diff < 0
								throw MiniC_Universal_Exception{
										"Unexpected \",\" and "
										+ std::to_string(args_size)
										+ " parameters but given "
										+ std::to_string(params.size())
										+ " in function-macro \""
										+ macro + "\"",
										line_num, static_cast<std::size_t>(prev_pos) };
						};
						while (_case != 2)
						{
							while (pos < size && line[pos] == ' ') pos++;
							if (pos == size)
							{
								if (_case == 0)
									throw_ex_case_0();
								else // _case == 1
									throw_ex_case_1();
							}

							if (_case == 0)
							{
								// case 0 -> )  -> case 2 -> ok
								if (line[pos] == ')' && _case == 0)
								{
									pos++;
									break;
								}

								// case 0 -> ,  -> case 1
								if (line[pos] == ',' && _case == 0)
								{
									prev_pos = pos;
									_case = 1;
									continue;
								}

								throw_ex_case_0();
							}

							// case 1 -> id -> case 0
							p = get_identifier(line, pos + 1, false);
							if (p.second == -1)
								throw_ex_case_1();
							prev_pos = pos + 1;
							pos = p.second;
							params.push_back(p.first);
							_case = 0;

						} // end while-loop for detecting function macro


						if (params.size() != args_size)
							throw MiniC_Universal_Exception(
								"Expected " + std::to_string(args_size)
								+ " parmeters for macro \"" + block._macro + "\" but given "
								+ std::to_string(params.size()),
								line_num, pos);

						/*
						 * replace macro lines
						 */

						std::unordered_map<arg_t, param_t> replace_map;
						for (int i = 0; i < args_size; i++)
							replace_map.insert(std::make_pair(args[i], params[i]));

						const std::list<std::string> replaces = block.get_replace();

						auto replace_macro_line =
							[&replace_map](std::string_view macro_line)->std::string
						{
							std::string replace;
							std::pair<std::string, int> p;
							const int size = macro_line.size();
							int start = 0, pos = 0;
							while (pos < size)
							{
								while (pos < size && macro_line[pos] == ' ') pos++;
								p = get_identifier(macro_line, pos);

								if (p.second == -1)
								{
									pos++;
									continue;
								}

								auto it = replace_map.find(p.first);
								if (it == replace_map.end())
								{
									pos += p.first.size();
									continue;
								}

								replace.append(std::string(
									macro_line.substr(start, pos - start)));
								replace.append(it->second);
								start = pos = p.second;
							}
							replace.append(std::string(macro_line.substr(start, pos - start)));
							return replace;
						}; // end lambda replace_macro_line();

						for (auto it = replaces.begin(); it != replaces.end(); ++it)
							ret.push_back(std::make_tuple(
								replace_macro_line(*it), line_num, true));
						std::get<2>(ret.back()) = false;

					} // end replace "macro_t::function"

				}; // end std::function replace_macro();

				std::pair<std::string, int> p;
				std::size_t start = 0;
				while (pos < size)
				{
					while (pos < size && line[pos] == ' ') pos++;
					p = get_identifier(line, pos);
					if (p.second == -1)
					{
						pos++;
						continue;
					}
					if (macros.find(p.first) == macros.end()) pos = p.second;
					else // replace "foo(2, 3)"
					{
						ret.push_back(std::make_tuple(
							std::string(line.substr(start, pos - start)), line_num, false));
						replace_macro(p.first, pos); // throw MiniC_Universal_Exception
													 // perpare the "pos" and "start"
						{
							auto const pb = macros.find(p.first);
							const DefineBlock& block = pb->second;
							if (block._macro_type == macro_t::function)
							{
								// jump over the ")" in "foo(2, 3)"
								while (line[pos] != ')') pos++;
								start = ++pos;
							}
							else // "macro_t::function"
								start = pos = p.second;
						}
					}
				}
				ret.push_back(std::make_tuple(
					std::string(line.substr(start, pos - start)), line_num, change_line));
				return ret;
			} // end function replace();

			// for debug
			static void print(std::ostream& out)
			{
				for (auto const&[s, b] : macros)
					b.print(out);
			}

		}; // end class Macros;


	} // end anonymous namespace
	std::unordered_map<std::string, Macros::DefineBlock> Macros::macros = {};


	/*
	 * return a new file which makes the replacement of macros
	 */
	[[nodiscard]] const std::string preprocess(const std::string& file_name, std::ostream& out)
	{
		constexpr std::size_t MAXSIZE = 256;
		char buffer[MAXSIZE];
		std::ifstream inputFile{ file_name, std::ios::in };
		if (!inputFile.is_open()) {
			std::cout << "failed to open: " << std::quoted(file_name) << std::endl;
			exit(0);
		}

		// (line_str, line_num, change_line)
		std::list<std::tuple<std::string, std::size_t, bool>> lines;
		std::size_t line_num = 0;
		while (!inputFile.eof())
		{
			line_num++;
			inputFile.getline(buffer, MAXSIZE - 1);
			lines.push_back(std::make_tuple(std::string(buffer), line_num, true));
		}
		std::get<2>(lines.back()) = false;
		inputFile.close();


		line_num = 1;
		for (auto it = lines.begin(); it != lines.end(); )
		{
			std::string_view line = std::get<0>(*it);
			line_num = std::get<1>(*it);
			std::size_t size = line.size();

			// `#define` must be at the start of one line
			if (line._Starts_with("#define "))
			{
				std::pair<std::string, int> p;
				p = get_identifier(line, 8);
				int pos = p.second;
				if (pos == -1)
					throw MiniC_Universal_Exception{ "Expected identifier after \"#define\"", line_num, 8 };
				const std::string macro{ p.first };

				while (pos < size && line[pos] == ' ') pos++;

				try {

					// "#define xxx"
					if (pos == size)
					{
						Macros::define_macro(macro, Macros::macro_t::null, line_num);
						it = lines.erase(it); // erase this line
						continue;
					}

					// "#define foo(...)"
					if (line[pos] == '(')
					{
						Macros::define_macro(macro, Macros::macro_t::function, line_num);

						pos++;
						// detect "foo()"
						if (pos == size)
							throw MiniC_Universal_Exception{
							"Expected something for macro \"" + macro + "\"",
							line_num, static_cast<std::size_t>(pos) };

						int _case = 0;
						if (line[pos] == ')')
						{
							_case = 2;
							pos++;
						}
						if (_case == 0)
						{
							p = get_identifier(line, pos);
							if (p.second == -1)
								throw MiniC_Universal_Exception{
								"Incorrect format in macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos) };
							pos = p.second;
							Macros::push_arg(macro, p.first, pos - p.first.size());
						}
						// case   -> (  -> case 0
						// detect: *(, id) ")"
						// case 0 -> ,  -> case 1
						// case 1 -> id -> case 0
						// case 0 -> )  -> case 2 -> ok
						//          ,  id   )
						// case0:   1, -1,  2 
						// case1:  -1,  0, -1
						while (_case != 2)
						{
							while (pos < size && line[pos] == ' ') pos++;
							if (pos == size)
								throw MiniC_Universal_Exception{
								"Incorrect format in macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos) };

							if (_case == 0)
							{
								// case 0 -> )  -> case 2 -> ok
								if (line[pos] == ')' && _case == 0)
								{
									pos++;
									break;
								}

								// case 0 -> ,  -> case 1
								if (line[pos] == ',' && _case == 0)
								{
									_case = 1;
									continue;
								}

								throw MiniC_Universal_Exception{
									"Incorrect format in macro \"" + macro + "\"",
									line_num, static_cast<std::size_t>(pos) };
							}

							// case 1 -> id -> case 0
							p = get_identifier(line, pos + 1);
							if (p.second == -1)
								throw MiniC_Universal_Exception{
								"Incorrect format in macro \"" + macro + "\"",
								line_num, static_cast<std::size_t>(pos + 1) };
							pos = p.second;
							Macros::push_arg(macro, p.first, pos - p.first.size());
							_case = 0;

						} // end while-loop for function macro

					}

					// "#define xxx ..."
					else
					{
						Macros::define_macro(macro, Macros::macro_t::object, line_num);
					}

					// push replace line for "macro_t::object" and "macro_t::function"
					// `pos` denotes the start // `pos` <= `size`
					bool next_line = false;
					int _pos = pos;
					while (pos < size && line[pos] != '\\') pos++;
					if (pos != _pos)
						Macros::push_replace(macro, std::string(line, _pos, pos - _pos));
					if (pos != size) next_line = true;
					it = lines.erase(it); // erase this line
					while (next_line)
					{
						next_line = false;
						if (it == lines.end())
							throw MiniC_Universal_Exception{
							"Expected more lines after macro",
							line_num, 0 };
						line = std::get<0>(*it);
						line_num = std::get<1>(*it);
						size = line.size();
						pos = 0;
						while (pos < size && line[pos] != '\\') pos++;
						Macros::push_replace(macro, std::string(line, 0, pos));
						if (pos != size) next_line = true;
						it = lines.erase(it); // erase this line
					}
				}
				catch (MiniC_Universal_Exception&) { throw; }
			} // end "#define"


			// `#undef`
			else if (line._Starts_with("#undef "))
			{
				std::pair<std::string, int> p;
				p = get_identifier(line, 7);
				int pos = p.second;
				if (pos == -1)
					throw MiniC_Universal_Exception{ "Expected identifier after \"#undef\"", line_num, 7 };
				const std::string macro{ p.first };
				Macros::undef_macro(macro, line_num, pos - macro.size()); // might throw `MiniC_Universal_Exception`
				it = lines.erase(it); // erase this line
			} // end "#undef"


			// may replace
			else
			{
				std::list<std::tuple<std::string, std::size_t, bool>> new_lines =
					Macros::replace(line, std::get<1>(*it), std::get<2>(*it));
				if (new_lines.size() == 1 && std::get<0>(new_lines.front()) == line)
				{
					++it;
					continue;
				}
				it = lines.erase(it);
				it = lines.insert(it, new_lines.begin(), new_lines.end());
			} // end macro replace

		} // end preprocess

		Macros::print(out);

		// assume the program is "xxx.txt"
		const std::string out_file = std::string{ file_name }.replace(
			file_name.size() - 4, 4, "_.txt");
		std::ofstream outputFile{ out_file, std::ios::out | std::ios::trunc };
		if (!outputFile.is_open()) {
			std::cout << "failed to open: " << std::quoted(out_file) << std::endl;
			exit(0);
		}

		for (auto const& line : lines)
		{
			outputFile << std::get<0>(line);
			if (std::get<2>(line)) outputFile << "\n";
		}

		outputFile.close();

		return out_file;

	} // end function preprocess();


} // end namespace Mini_C::preprocess