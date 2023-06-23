#pragma once

#include <type_traits>

namespace ctti
{
	using basic_string_literal_size = decltype(sizeof(nullptr));
	namespace string_utils
	{
		template <typename char_type>
		consteval basic_string_literal_size string_length(const char_type* str)
		{
			return str[0] == '\0' ? 0 : string_length(str + 1) + 1;
		}
	}

	template <typename char_type, basic_string_literal_size N>
	struct basic_string_literal
	{
		using size_type = basic_string_literal_size;
		using value_type = char_type;

		static constexpr size_type npos = static_cast<size_type>(-1);

		consteval basic_string_literal() = default;

		template <size_type... seq>
		consteval basic_string_literal(const char* str, size_type start, std::index_sequence<seq...>)
			: m_Data{ str[start + seq]... }
		{
		}

		template <size_type... seq>
		consteval basic_string_literal(const char* str, std::index_sequence<seq...> s)
			: basic_string_literal(str, 0, s)
		{
		}

		consteval char_type operator[] (size_type index) const
		{
			return m_Data[index];
		}

		constexpr const char_type* begin() const
		{
			return &m_Data[0];
		}

		constexpr const char_type* end() const
		{
			return &m_Data[N];
		}

		consteval bool empty() const
		{
			return m_Data == nullptr;
		}

		consteval size_type size() const
		{
			return N;
		}

		consteval size_type length() const
		{
			return size();
		}

		consteval size_type contains(char_type test) const
		{
			for (size_type i = 0; i < N; ++i)
				if (m_Data[i] == test) return i;
			return npos;
		}

		template <size_type start = 0, size_type end = N>
		consteval basic_string_literal<char_type, end - start> substr() const
		{
			return substr<start, end - start>(std::make_index_sequence<end - start>{});
		}

		consteval size_type find(const char_type ch) const
		{
			for (size_type i = 0; i < N; ++i)
				if (m_Data[i] == ch) return i;
			return npos;
		}

		consteval size_type find_reverse(const char_type ch) const
		{
			for (size_type i = N - 1; i > 0; --i)
				if (m_Data[i] == ch) return i;
			return npos;
		}

		consteval bool operator== (const basic_string_literal<char_type, N>& rhs) const
		{
			for (size_type i = 0; i < N; ++i)
				if (m_Data[i] != rhs.m_Data[i]) return false;
			return true;
		}

		consteval bool operator== (const char_type* rhs) const
		{
			if (string_utils::string_length(rhs) != N)
				return false;
			for (size_type i = 0; i < N; ++i)
				if (m_Data[i] != rhs[i]) return false;
			return true;
		}

		consteval bool operator!= (const basic_string_literal<char_type, N>& rhs) const
		{
			return !(*this == rhs);
		}

		consteval bool operator!= (const char_type* rhs) const
		{
			return !(*this == rhs);
		}

		consteval operator const char_type* () const { return m_Data; }

	private:
		template <typename c>
		friend struct basic_cstring;

		const char_type m_Data[N + 1];

		template <size_type start, size_type size, size_type... seq>
		consteval basic_string_literal<char_type, size> substr(std::index_sequence<seq...> s) const
		{
			static_assert(sizeof...(seq) > 0, "Can't have zero sized basic_string_literal!");

			return basic_string_literal<char_type, size>(m_Data, start, s);
		}
	};
}