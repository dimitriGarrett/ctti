#pragma once

#include "cstring.h"

namespace ctti
{
	namespace detail
	{
		// currently not tested on any other compiler than msvc
		template <typename... T>
		consteval const char* full_name_char_impl()
		{
#if defined(__clang__) || defined(__GNUC__)
			return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
			return __FUNCSIG__;
#else
#ifdef __func__
			return __func__;
#else
#error "Unknown Compiler, cannot use the ctti!"
#endif
#endif
		}

		// currently not tested on any other compiler than msvc
		template <typename char_type, typename... T>
		consteval const char_type* full_name_char()
		{
			return static_cast<const char_type*>(full_name_char_impl<T...>());
		}

		template <typename char_type, typename... T>
		consteval basic_string_literal<char_type, string_utils::string_length(full_name_char<char_type, T...>())> full_name()
		{
			return basic_string_literal<char_type, string_utils::string_length(full_name_char<char_type, T...>())>(full_name_char<char_type, T...>(), std::make_index_sequence<string_utils::string_length(full_name_char<char_type, T...>())>{});
		}

		template <typename char_type, typename... T>
		constexpr basic_cstring<char_type> full_name_string()
		{
			return cstring(full_name_char<char_type, T...>());
		}

		template <typename char_type, basic_string_literal_size start, basic_string_literal_size stop, typename... T>
		consteval basic_string_literal<char_type, stop - start> type_name()
		{
			return basic_string_literal<char_type, 
				string_utils::string_length(full_name_char<char_type, T...>())>
				(full_name_char<char_type, T...>(), std::make_index_sequence<string_utils::string_length(full_name_char<char_type, T...>())>{}).substr<start, stop>();
			//return make_string_literal<string_length(full_name_char<T...>())>(full_name_char<T...>()).substr<start, stop>();
		}

		/*template <string::size_type start, string::size_type stop, typename... T>
		consteval fixed_string<char, stop - start> type_name()
		{
			using str_type = fixed_string<char, string_len(full_name<T...>().str)>;

			constexpr str_type temp = str_type(detail::full_name<T...>());
			return temp.substr<start, stop>();
		}*/
	}

	template <typename char_type, typename... T>
	consteval decltype(auto) basic_type_name()
	{
		constexpr basic_string_literal<char_type, string_utils::string_length(detail::full_name_char<char_type, T...>())> temp = 
			basic_string_literal<char_type, string_utils::string_length(detail::full_name_char<char_type, T...>())>
			(detail::full_name_char<char_type, T...>(), std::make_index_sequence<string_utils::string_length(detail::full_name_char<char_type, T...>())>{});
		return detail::type_name<char_type, temp.find('<') + 1, temp.find('>'), T...>();
	}

	template <typename... T>
	consteval decltype(auto) type_name()
	{
		return basic_type_name<char, T...>();
	}
}