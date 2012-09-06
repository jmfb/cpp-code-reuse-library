////////////////////////////////////////////////////////////////////////////////
// Filename:    main.cpp
// Description: This is the test file for all Code Reuse Library (CRL) files.
// Created:     2005-03-19 17:34:06
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500

#include <CRL/MathUtility.h>
#include <CRL/StringUtility.h>
#include <CRL/TimeUtility.h>
#include <CRL/PtrUtility.h>
#include <CRL/TypeUtility.h>
#include <CRL/FileUtility.h>
#include <CRL/InternetUtility.h>
#include <CRL/ErrorUtility.h>

class CTest : public PTR::CSingleton<CTest>
{
public:
	void Test()
	{
		std::cout << "CTest::Test" << std::endl;
	}

private:
	CTest()
	{
		std::cout << "CTest c-tor" << std::endl;
	}
	~CTest()
	{
		std::cout << "CTest d-tor" << std::endl;
	}

	friend class PTR::CSingleton<CTest>;

private:
	CTest(const CTest& rhs);
	CTest& operator=(const CTest& rhs);
};

int main()
{
	int i = 0;
	bool b = false;
	std::string s;
	TIME::CDate date;
	TIME::CTime time;
	TIME::CDateTime dt;

	i = MATH::Min(0, 1);
	i = MATH::Max(0, 1);
	b = MATH::Between(0, 2, 1);
	i = MATH::Bound(0, 2, -1);
	i = MATH::Instead(1, 1, 2);

	s = STRING::replace("hello", "h", "H");
	s = STRING::trim(" hello ");
	s = STRING::ltrim(" hello ");
	s = STRING::rtrim(" hello ");
	s = STRING::upper("hello");
	s = STRING::lower("HELLO");
	b = STRING::isident('_');
	b = STRING::isidnum('_');

	s = TIME::Format(TIME::GetDateTime(), "{d},{dd},{M},{MM},{MMM},{MMM},{yy},{yyyy},{h},{hh},{H},{HH},{m},{mm},{s},{ss},{n},{nnn},{t},{tt}");

	s = FSYS::GetFileName("c:\\save\\code\\CRL\\main.cpp");
	s = FSYS::GetFilePath("c:\\save\\code\\CRL\\main.cpp");
	s = FSYS::GetFolderName("c:\\save\\code\\CRL\\main.cpp");
	s = FSYS::GetFileTitle("c:\\save\\code\\CRL\\main.cpp");
	s = FSYS::GetFileExt("c:\\save\\code\\CRL\\main.cpp");

	s = FSYS::GetFileName("");
	s = FSYS::GetFilePath("");
	s = FSYS::GetFolderName("");
	s = FSYS::GetFileTitle("");
	s = FSYS::GetFileExt("");

	s = FSYS::GetFileName("\\");
	s = FSYS::GetFilePath("\\");
	s = FSYS::GetFolderName("\\\\");
	s = FSYS::GetFileTitle("\\.");
	s = FSYS::GetFileExt("\\.");

	std::string from = "this\nand\nthat";
	std::istringstream in(from);
	std::string line;
	while (STRING::read_until(in, line, b));

	{
		PTR::CScope<PTR::CPtrPolicy<int> > p(new int(6));
		*p.Get() = 5;

		PTR::CScope<PTR::CPtrPolicy<int, true> > pa(new int[6]);
		pa.Get()[0] = 1;
		pa.Get()[5] = 6;
		pa.Release();
	}

	CTest::Get().Test();
	b = CTest::Exists();

	TYPE::integer_type<char>::unsigned_t c = 'c';
	c = 0;
	TYPE::integer_type<unsigned long long>::signed_t ll = -40000000000ll;
	ll = 2;

	std::cout << STRING::hex(0x45ffabcd) << std::endl;
	std::cout << STRING::hex(0x45ff) << std::endl;
	std::cout << STRING::hex(static_cast<short>(0x45ff)) << std::endl;

	return(0);
}

