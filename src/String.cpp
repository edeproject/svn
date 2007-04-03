/*
 * $Id$
 *
 * A simple string class.
 * Part of edelib.
 * Copyright (c) 2005-2007 EDE Authors.
 *
 * This program is licenced under terms of the 
 * GNU General Public Licence version 2 or newer.
 * See COPYING for details.
 */

#include <edelib/econfig.h>
#include <edelib/String.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h> // vsnprintf

#define PRINTF_BUFF 1024
#define STERM '\0'

EDELIB_NAMESPACE {

String::StringData String::null_data = {0, 0, ""};
const String::size_type  String::npos = ~(String::size_type)0;

String::String() : sdata(&null_data)
{ }

String::String(const char* str) : sdata(&null_data)
{ 
	assign(str); 
}

String::String(const String& str) : sdata(&null_data)
{ 
	if(str.length())
		assign(str.c_str()); 
}

String::~String()
{
	dispose();
}

void String::init(size_type len, size_type cap)
{
	assert(len <= cap);
	sdata = new StringData;
	sdata->chars = new char[cap + 1];
	sdata->chars[0] = STERM;
	sdata->length = len;
	sdata->capacity = cap;
}

void String::dispose(void)
{
	if(sdata != &null_data)
	{
		delete [] sdata->chars;
		delete sdata;
		sdata = &null_data;
	}
}

void String::reserve(size_type cap)
{
	if(cap > capacity())
	{
		String tmp;
		tmp.init(length(), cap);
		memcpy(tmp.sdata->chars, data(), length());
		swap(tmp);
	}
}

void String::swap(String& from)
{
	StringData* d = sdata;
	sdata = from.sdata;
	from.sdata = d;
}

String& String::assign(const char* str, size_type len)
{
	/*
	 * Hanle cases:
	 *    foo.reserve(20);
	 *    foo.assign("foo");
	 * since std::string will have:
	 *    foo.length() == 3;
	 *    foo.capacity() == 20;
	 */
	size_type cap = capacity();
	dispose();

	if(len < cap)
		init(len, cap);
	else
		init(len, len);
	memcpy(sdata->chars, str, len);
	sdata->chars[len] = STERM;

	return *this;
}

String& String::assign(const char* str)
{
	assign(str, strlen(str));
	return *this;
}

String& String::assign(const String& str)
{
	assign(str.c_str(), str.length());
	return *this;
}

String& String::append(const char* str, size_type len)
{
	if(len + length() <= capacity())
	{
		memcpy(sdata->chars + length(), str, len);
		sdata->length += len;
		sdata->chars[sdata->length] = STERM;
	}
	else
	{
		reserve((capacity() + len) * 2);
		memcpy(sdata->chars + length(), str, len);
		sdata->length += len;
		sdata->chars[sdata->length] = STERM;
	}

	return *this;
}

String& String::append(const char* str)
{
	append(str, strlen(str));
	return *this;
}

String& String::append(const String& str)
{
	append(str.c_str(), str.length());
	return *this;
}

void String::clear(void)
{
	dispose();
}

void String::printf(const char* fmt, ...)
{
	assert(fmt != NULL);

	char buff[PRINTF_BUFF];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buff, PRINTF_BUFF, fmt, ap);
	va_end(ap);

	assign((char*)buff);
}

String& String::operator=(const char* str)
{
	assign(str);
	return *this;
}

String& String::operator=(const String& str)
{
	if(&str != this)
		assign(str);
	return *this;
}

String& String::operator+=(const char* str)
{
	append(str);
	return *this;
}

String& String::operator+=(const String& str)
{
	append(str);
	return *this;
}

char& String::operator[](size_type index)
{
	assert(index < length());
	return sdata->chars[index];
}

char String::operator[](size_type index) const
{
	assert(index < length());
	return sdata->chars[index];
}

String String::substr(size_type index, size_type num)
{
	assert(index < num);

	String tmp;
	if(num == npos)
		tmp.assign(data() + index, length() - index);
	else
		tmp.assign(data() + index, num);
	return tmp;
}

String::size_type String::find(const char* str, size_type offset)
{
	if(offset >= length())
		return npos;

	char* p = strstr(data() + offset, str);
	if(!p)
		return npos;
	else
		return (p - data());
}

String::size_type String::find(const char* str)
{
	return find(str, 0);
}

String::size_type String::find(char ch, size_type offset)
{
	if(offset >= length())
		return npos;

	size_type i = 0;
	for(const char* p = data() + offset; *p != STERM && i < length(); p++, i++)
	{
		if(*p == ch)
			return (i + offset);
	}
	return npos;
}

String operator+(const String& s1, const String& s2)
{
	String tmp;

	String::size_type len = s1.length();
	len += s1.length();
	/*
	 * Do not allocate anything if sum of lenghts of 
	 * s1 and s2 are 0.
	 */
	if(len)
	{
		tmp.reserve(len);
		tmp += s1;
		tmp += s2;
	}
	return tmp;
}

String operator+(const char* s1, const String& s2)
{
	String tmp;

	String::size_type len = strlen(s1);
	len += s2.length();
	// see above for allocation
	if(len)
	{
		tmp.reserve(len);
		tmp += s1;
		tmp += s2;
	}
	return tmp;
}

String operator+(const String& s1, const char* s2)
{
	return operator+(s2, s1);
}

}