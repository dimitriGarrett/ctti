#pragma once

#include "string_literal.h"

namespace ctti
{
	namespace string_utils
	{
		template <typename char_type>
		constexpr basic_string_literal_size cstring_length(const char_type* str)
		{
			return str[0] == '\0' ? 0 : cstring_length(str + 1) + 1;
		}
	}

	template <typename char_type>
	struct basic_cstring
	{
		using size_type = basic_string_literal<char_type, 0>::size_type;

		const char_type* m_Data;
		size_type size = 0;
		
		constexpr basic_cstring() = default;

		constexpr bool operator== (const basic_cstring& rhs) const
		{
			return std::string_view(m_Data, size) == std::string_view(rhs.m_Data, rhs.size);
		}

		constexpr basic_cstring(const char_type* str)
			: m_Data(str), size(string_utils::cstring_length(str))
		{
		}

		constexpr basic_cstring(const char_type* str, size_type start, size_type Size = string_utils::cstring_length<char_type>(str))
			: m_Data{ str + start }, size(Size)
		{
		}

		/*template <size_type... N>
		constexpr basic_cstring(const char_type* str, size_type start, std::index_sequence<N...>)
			: m_Data{ (str[start + N], ...) }
		{
		}*/

		template <size_type N>
		basic_cstring(const basic_string_literal<char_type, N>& str)
			: basic_cstring(str.m_Data)
		{
		}

		constexpr size_type find(char_type st) const
		{
			for (size_type i = 0; i < size; ++i)
				if (m_Data[i] == st) return i;
			return basic_string_literal<char_type, 0>::npos;
		}

		constexpr const char_type* begin() const
		{
			return &m_Data[0];
		}

		constexpr const char_type* end() const
		{
			return &m_Data[size];
		}

		operator const char_type* () const
		{ 
			return std::string_view(m_Data, size).data();
		}
	};

	using cstring = basic_cstring<char>;

	template <typename char_type>
	constexpr bool operator== (const char_type* str, const basic_cstring<char_type>& rhs)
	{
		if (rhs.size != string_utils::cstring_length(str))
			return false;
		for (typename basic_cstring<char_type>::size_type i = 0; i < rhs.size; ++i)
		{
			if (str[i] != rhs.m_Data[i]) return false;
		}

		return true;
	}

	template <typename char_type>
	constexpr bool operator== (const basic_cstring<char_type>& rhs, const char_type* str)
	{
		return str == rhs;
	}
}