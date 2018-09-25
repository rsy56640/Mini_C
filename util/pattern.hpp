#pragma once
#ifndef _PATTERN_HPP
#define _PATTERN_HPP
#include <variant>
#include <type_traits>
template<typename ...Ts>
using tagged_union = std::variant<Ts...>;

namespace Mini_C::uitl {
	namespace _impl {

		template<typename R, typename ...Ts, typename U, typename ...Fs> R match_impl(U &&u, Fs ...arms)
		{
			struct overloaded : public Fs... {using Fs::operator()...; };
			using CheckArgs = std::conjunction<std::is_invocable<overloaded, Ts>...>;
			static_assert(CheckArgs::value, "Missing matching cases");
			using CheckRet = std::conjunction<
				std::is_convertible<std::invoke_result_t<overloaded, Ts>, R>...>;
			static_assert(CheckRet::value, "Return type not compatible");
			overloaded dispatcher{ arms... };
			return std::visit([&](auto &&var) -> R {
				return dispatcher(std::forward<decltype(var)>(var));
			}, std::forward<U>(u));
		}

	} // namespace _impl

	template<typename R = void, typename ...Ts, typename ...Fs>
	R match(const tagged_union<Ts...> &u, Fs ...arms) {
		return _impl::match_impl<R, const Ts &...>(u, arms...);
	}

	template<typename R = void, typename ...Ts, typename ...Fs>
	R match(tagged_union<Ts...> &&u, Fs ...arms) {
		return _impl::match_impl<R, Ts &&...>(std::move(u), arms...);
	}
}

#endif // !_PATTERN_HPP