#pragma once

#include "string_utils.h"

namespace ctti
{
	namespace detail
	{
		template <typename... Ts>
		consteval const char* full_name()
		{
#ifdef _MSC_VER
			return __FUNCSIG__;
#else
			return __PRETTY_FUNCTION__;
#endif
		}
	}

#ifdef _MSC_VER
	inline constexpr char START = '<';
	inline constexpr char END = '>';
#elif defined(__GNUC__)
	inline constexpr char START = '{';
	inline constexpr char END = '}';
#endif

	template <typename char_type = char, typename... Ts>
	constexpr decltype(auto) type_name_string()
	{
		constexpr const char* f = detail::full_name<Ts...>();

		constexpr size_t start = string_utils::find(START, f) + 1;
		constexpr size_t end = string_utils::find(END, f);

		return basic_fixed_string<char, end - start>(f, start, std::make_index_sequence<end - start>{});
	}

	template <typename char_type, typename... Ts>
	constexpr hash_t basic_type_name()
	{
		return string_utils::hash(type_name_string<char_type, Ts...>().data);
	}

	template <typename... Ts>
	constexpr hash_t type_name()
	{
		return basic_type_name<char, Ts...>();
	}
}