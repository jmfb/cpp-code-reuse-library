#if !defined(CRL_STRINGUTILITY_H)
#define CRL_STRINGUTILITY_H

#include <string>
#include <cctype>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <CRL/TypeUtility.h>
#include <CRL/CScope.h>

namespace STRING
{
	inline std::string replace(const std::string& src, const std::string& find, const std::string& repl)
	{
		std::string copy = src;
		if (!find.empty())
		{
			for (std::string::size_type pos = copy.find(find);
				pos != std::string::npos;
				pos = copy.find(find, pos + repl.size()))
			{
				copy.replace(pos, find.size(), repl);
			}
		}
		return(copy);
	}

	inline std::string ltrim(const std::string& str)
	{
		std::string::size_type iter = 0;
		for (; (iter < str.size()) && ::isspace(str.at(iter)); ++iter);
		return(str.substr(iter));
	}

	inline std::string rtrim(const std::string& str)
	{
		std::string::size_type iter = str.size();
		for (; (iter > 0) && ::isspace(str.at(iter - 1)); --iter);
		return(str.substr(0, iter));
	}

	inline std::string trim(const std::string& str)
	{
		return(rtrim(ltrim(str)));
	}

	inline std::string upper(const std::string& str)
	{
		std::string retval = str;
		std::transform(retval.begin(), retval.end(), retval.begin(), ::toupper);
		return(retval);
	}

	inline std::string lower(const std::string& str)
	{
		std::string retval = str;
		std::transform(retval.begin(), retval.end(), retval.begin(), ::tolower);
		return(retval);
	}

	inline void split(
		const std::string& str,
		const std::string& match,
		std::string& before,
		std::string& after)
	{
		std::string::size_type pos = str.find(match);
		before = str.substr(0, pos);
		after.erase();
		if (pos != std::string::npos && (pos + match.size()) < str.size())
			after = str.substr(pos + match.size());
	}

	template <typename T>
	inline void split(
		const std::string& str,
		const std::string& match,
		T oiter)
	{
		for (std::string::size_type pos = 0; pos < str.size(); )
		{
			std::string::size_type end = str.find(match, pos);
			*oiter++ = str.substr(pos, end == std::string::npos ? end : end - pos);
			pos = end == std::string::npos ? end : end + match.size();
		}
	}

	template <typename TContainer>
	inline std::string join(const TContainer& container, const std::string& separator)
	{
		std::ostringstream out;
		for (auto start = container.begin(), iter = container.begin(), end = container.end(); iter != end; ++iter)
		{
			if (iter != start)
				out << separator;
			out << *iter;
		}
		return out.str();
	}

	inline bool isident(char c)
	{
		return(::isalpha(c) || c == '_');
	}
	inline bool isidnum(char c)
	{
		return(::isalnum(c) || c == '_');
	}

	inline bool read_until(std::istream& in, std::string& str, bool& hit, char sep = '\n')
	{
		char c = 0;
		str.erase();
		hit = false;
		while (in.get(c))
		{
			if (c == sep)
			{
				hit = true;
				break;
			}
			str += c;
		}
		return(hit || !str.empty());
	}

	template <typename T>
	inline std::string hex(T val)
	{
		std::ostringstream out;
		out << std::setw(sizeof(T) * 2)
			<< std::setfill('0')
			<< std::hex
			<< std::setiosflags(std::ios::right | std::ios::uppercase)
			<< static_cast<unsigned long long>(TYPE::unsigned_cast(val));
		return(out.str());
	}

	typedef PTR::CScope<PTR::CPtrPolicy<char, true> > CStringPtr;

	template <template <typename> class T>
	class case_insensitive : std::binary_function<char, char, bool>
	{
	public:
		bool operator()(char lhs, char rhs) const
		{
			return(T<char>()(::tolower(lhs), ::tolower(rhs)));
		}
	};

	typedef case_insensitive<std::less> iless_char;
	typedef case_insensitive<std::equal_to> iequal_char;

	class iless : std::binary_function<std::string, std::string, bool>
	{
	public:
		bool operator()(const std::string& lhs, const std::string& rhs) const
		{
			return(std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), iless_char()));
		}
	};

	class iequal : std::binary_function<std::string, std::string, bool>
	{
	public:
		bool operator()(const std::string& lhs, const std::string& rhs) const
		{
			return(lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), iequal_char()));
		}
	};

	inline std::wstring to_wstring(const std::string& value)
	{
		std::wostringstream out;
		for (char ch: value)
			out << static_cast<wchar_t>(ch);
		return out.str();
	}

	template <typename T>
	inline std::string to_string(T value)
	{
		std::ostringstream out;
		out << value;
		return out.str();
	}

	template <typename T>
	inline T from_string(const std::string& value)
	{
		T result;
		std::istringstream in(value);
		in >> result;
		return result;
	}

	inline bool EndsWith(const std::string& value, const std::string& suffix)
	{
		return value.rfind(suffix) == value.size() - suffix.size();
	}
}

#endif //CRL_STRINGUTILITY_H

