#include <edelib/String.h>
#include <stdlib.h>
#include "UnitTest.h"

using namespace edelib;

UT_FUNC(StringBasicTest, "Test basic string functions")
{
	String dstr;
	UT_VERIFY( dstr.length() == 0 );

	dstr.reserve(20);
	UT_VERIFY( dstr.capacity() == 20 );
	UT_VERIFY( dstr.length() == 0 );

	dstr = "Test string";
	UT_VERIFY( dstr.length() == 11 );

	dstr = "Test";
	UT_VERIFY( dstr.length() == 4 );

	dstr.clear();
	UT_VERIFY( dstr.length() == 0 );

	dstr.assign("test");
	UT_VERIFY( dstr.length() == 4 );
	dstr.append(" sample");
	UT_VERIFY( dstr.length() == 11 );
	dstr.append(" sample1");
	dstr.append(" sample2");
	dstr.append("sample3");
	UT_VERIFY( dstr.length() == 34 );
	UT_VERIFY( dstr == "test sample sample1 sample2sample3" );

	dstr.clear();
	dstr.append("Sample string", 4);
	UT_VERIFY( dstr == "Samp" );
	UT_VERIFY( dstr[0] == 'S' );
	UT_VERIFY( dstr[1] == 'a' );
	UT_VERIFY( dstr[2] == 'm' );
	UT_VERIFY( dstr[3] == 'p' );
}

UT_FUNC(StringOperators, "Test string operators")
{
	String test("sample");
	UT_VERIFY( test != "Sample" );
	UT_VERIFY( test == "sample" );
	UT_VERIFY( test <= "sample" );
	UT_VERIFY( test >= "sample" );
	UT_VERIFY( test > "sa" );
	UT_VERIFY( test < "samplebla" );

	UT_VERIFY( "Sample" != test );
	UT_VERIFY( "sample" == test );
	UT_VERIFY( "sample" >= test );
	UT_VERIFY( "sample" <= test );
	UT_VERIFY( "samplebla" > test );
	UT_VERIFY( "sam" < test );

	String test2 = "dumb";
	UT_VERIFY( test != test2 );

	String test3 = "dumb";
	UT_VERIFY( test3 == test2 );
	UT_VERIFY( test3 == test3 );
	UT_VERIFY( test2 == test2 );
	UT_VERIFY( test2 >= test3 );
	UT_VERIFY( test2 <= test3 );
	UT_VERIFY( test >= test2 );
	UT_VERIFY( test > test2 );
	UT_VERIFY( test >= test3 );
	UT_VERIFY( test > test3 );

	UT_VERIFY( test3[0] == 'd' );
	UT_VERIFY( test2[2] == 'm' );

	test2[0] = 'D';
	UT_VERIFY( test2 == "Dumb" );
	test2[2] = 'M';
	UT_VERIFY( test2 == "DuMb" );

	String test4 = "";
	test4 += "bla";
	UT_VERIFY( test4 == "bla" );
	test4 += "";
	UT_VERIFY( test4 == "bla" );
	test4 += " ";
	UT_VERIFY( test4 == "bla " );
	test4 += "xxx";
	UT_VERIFY( test4 == "bla xxx" );
	test4.clear();
	UT_VERIFY( test4 == "" );
	test4.assign("moo");
	UT_VERIFY( test4 == "moo" );
	test4 += "xxx";
	UT_VERIFY( test4 == "mooxxx" );
	test4.assign("foo");
	UT_VERIFY( test4 == "foo" );
}

UT_FUNC(StringFind, "Test string find")
{
	String s("Sample string");
	UT_VERIFY( s.find("ample") == 1 );
	UT_VERIFY( s.find("XXX") == String::npos );
	UT_VERIFY( s.find('e', 0) == 5 );
}

UT_FUNC(StringPrintf, "Test string printf")
{
	String s;
	s.printf("Sample %s", "demo");
	UT_VERIFY( s == "Sample demo" );
	s.printf("Sample number %i", 5);
	UT_VERIFY( s == "Sample number 5" );

	s.clear();
	UT_VERIFY( s == "" );

	s.reserve(20);
	s.printf("Sample number %i", 5);
	UT_VERIFY( s == "Sample number 5" );
	UT_VERIFY( s.length() == 15 );
	UT_VERIFY( s.capacity() == 20 );

	s.printf("");
	UT_VERIFY( s.length() == 0 );
	UT_VERIFY( s.capacity() == 20 );
	s.clear();
	UT_VERIFY( s.capacity() == 0 );
}

#include <string>
UT_FUNC(StringComparison, "Test std::string comparison")
{
	String s;
	std::string s1;
	UT_VERIFY( s.length() == s1.length() );
	UT_VERIFY( s.capacity() == s1.capacity() );

	s = "foo";
	s1 = "foo";
	UT_VERIFY( s == s1.c_str() );

	UT_VERIFY( s.find("oo") == s1.find("oo") );
	UT_VERIFY( s.find("foo") == s1.find("foo") );
	UT_VERIFY( s.find("o") == s1.find("o") );
	UT_VERIFY( s.find('o', 0) == s1.find('o', 0) );
}
