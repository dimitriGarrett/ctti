#pragma once

#include <utility>

namespace ctti
{
	using size_t = decltype(sizeof(nullptr));

	namespace string_utils
	{
		inline constexpr size_t npos = static_cast<size_t>(-1);

		template <typename char_type = char>
		constexpr size_t string_length(const char_type* str)
		{
			return str[0] == '\0' ? 0 : string_length(str + 1) + 1;
		}

		template <typename char_type = char>
		consteval size_t find(char c, const char_type* str)
		{
			for (size_t i = 0; i < string_utils::string_length(str); ++i)
				if (c == str[i])
					return i;

			return npos;
		}

		template <typename char_type = char>
		consteval size_t distance(char c, char c2, const char_type* str)
		{
			return find<char_type>(c2, str) - find<char_type>(c, str);
		}

#if INTPTR_MAX == INT32_MAX
	constexpr size_t FNVOffset = 2166136261;
	constexpr size_t FNVPrime = 16777619;
#elif INTPTR_MAX == INT64_MAX
	constexpr size_t FNVOffset = 0xcbf29ce484222325;
	constexpr size_t FNVPrime = 1099511628211;
#else
	#error "Unkown architecture"
#endif

		template <typename char_type = char>
		constexpr size_t hash(const char_type* str)
		{
			size_t result = FNVOffset;

			for (size_t i = 0; i < string_length(str); ++i)
			{
				result ^= str[i];
				result *= FNVPrime;
			}

			return result;
		}
	}

	struct hash_t
	{
		size_t val = -1;

		constexpr hash_t() = default;

		constexpr hash_t(const size_t& s)
			: val(s)
		{
		}

		template <typename char_type = char>
		constexpr hash_t(const char_type* rhs)
			: val(string_utils::hash(rhs))
		{
		}

		constexpr bool operator== (const hash_t& rhs) const
		{
			return val == rhs.val;
		}

		constexpr bool operator== (const size_t& rhs) const
		{
			return val == rhs;
		}

		template <typename char_type = char>
		constexpr bool operator== (const char_type* rhs) const
		{
			return val == string_utils::hash(rhs);
		}
	};

	template <typename char_type, size_t N>
	struct basic_fixed_string
	{
		char_type data[N + 1] = { 0 };

		template <size_t... seq>
		consteval basic_fixed_string(const char_type* str, size_t start, std::index_sequence<seq...>)
			: data{ str[start + seq]... }
		{
		}

		template <size_t... seq>
		consteval basic_fixed_string(const char_type* str, std::index_sequence<seq...> s)
			: basic_fixed_string(str, 0, s)
		{
		}

		consteval bool operator==(const char_type* str) const
		{
			if (N != string_utils::string_length(str))
				return false;

			for (size_t i = 0; i < N; ++i)
				if (data[i] != str[i])
					return false;
			return true;
		}
	};
}

namespace std
{
	template <>
	struct std::hash<ctti::hash_t>
	{
		ctti::size_t operator() (const ctti::hash_t& h) const
		{
			return h.val;
		}
	};
}