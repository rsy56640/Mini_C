#pragma once
#ifndef _PATTERN_HPP
#define _PATTERN_HPP
#include <variant>
#include <type_traits>
#include "util.h"

namespace Mini_C::util {

	template<typename ...Ts>
	using tagged_union = std::variant<Ts...>;

	namespace _impl {

		template<typename R, typename ...Ts, typename U, typename ...Fs> R match_impl(U &&u, Fs ...arms)
		{
			static_assert(std::conjunction_v<std::is_invocable<overloaded, Ts>...>,
				"Missing matching cases");
			static_assert(
				std::conjunction_v<std::is_convertible<std::invoke_result_t<overloaded, Ts>, R>...>,
				"Return type not compatible");
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