/*************************************************************************
* Muon
* C++ Core Library
*------------------------------------------------------------------------
* Copyright (c) 2015 Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#include <cstdlib>
#include <cstring>

#include "Muon/Memory/IStream.hpp"
#include "Muon/System/Log.hpp"
#include "Muon/System/Assert.hpp"
#include "Muon/Type/String.hpp"

namespace muon
{
	String::String()
		: _str(NULL)
		, _charcount(0)
		, _memsize(0)
	{
	}

	String::String(const char* str)
		: String()
	{
		if (str && *str != 0)
		{
			_memsize = ::strlen(str);
			_charcount = _memsize;
			_str = (char*)::malloc(sizeof(char) * (_memsize + 1));
			MUON_ASSERT_BREAK(_str != NULL, "String(const char*) failed!");
			::strcpy(_str, str);
			*(_str + _memsize) = 0;
		}
	}

	String::String(const String& str)
		: String()
	{
		if (str._charcount > 0)
		{
			_charcount = str._charcount;
			_memsize = _charcount;
			_str = (char*)::malloc(sizeof(char) * (_charcount + 1));
			MUON_ASSERT_BREAK(_str != NULL, "String(const String&) failed!");
			::strcpy(_str, str._str);
			*(_str + _charcount) = 0;
		}
	}

	String::String(u32 size, char c)
		: String()
	{
		if (size != 0)
		{
			_memsize = (size + 1) * sizeof(char);
			_str = (char*)::malloc(_memsize);
			MUON_ASSERT_BREAK(_str != NULL, "String(u32, char) failed!");
			if (c != 0)
			{
				_charcount = size;
				::memcpy(_str, &c, _memsize);
				*(_str + _charcount) = 0;
			}
			else
			{
				(*_str) = 0;
			}
		}
	}

	String::~String()
	{
		if (_str)
		{
			free(_str);
		}
	}

	char String::operator[](u32 i) const
	{
		return *(_str + i);
	}

	char& String::operator[](u32 i)
	{
		return *(_str + i);
	}


	bool String::operator==(const char* other) const
	{
		if (_str == NULL || other == NULL)
		{
			return false;
		}
		return (strcmp(_str, other) == 0);
	}

	bool String::operator!=(const char* other) const
	{
		return !(*this == other);
	}

	bool String::operator==(const String& other) const
	{
		return (*this == other.cStr());
	}

	bool String::operator!=(const String& other) const
	{
		return !(*this == other);
	}

	String& String::operator+=(const char* other)
	{
		if (other)
		{
			muon::u32 size = ::strlen(other);
			//Realloc only if needed
			if (_charcount + size >= _memsize)
			{
				_str = (char*)::realloc(_str, sizeof(char) * (_charcount + size + 1));
				_memsize = _charcount + size;
			}
			::strncpy(_str + _charcount, other, size);
			_charcount += size;
			*(_str + _charcount) = 0;
		}
		return *this;
	}

	String& String::operator+=(const String& other)
	{
		return *this += other.cStr();
	}

	String& String::operator+=(const char c)
	{
		char carray[2] = { c, '\0' };
		return *this += carray;
	}

	String& String::operator=(const char* str)
	{
		if (str)
		{
			clear();
			_charcount = ::strlen(str);
			_memsize = _charcount;
			_str = (char*)::malloc(sizeof(char) * (_charcount + 1));
			MUON_ASSERT_BREAK(_str != NULL, "operator=() failed!");
			::strcpy(_str, str);
			*(_str + _charcount) = 0;
		}
		return *this;
	}

	String& String::operator=(const String& other)
	{
		return operator=(other.cStr());
	}

	void String::clear()
	{
		if (_str)
		{
			::free(_str);
			_str = NULL;
			_memsize = 0;
			_charcount = 0;
		}
	}

	void String::resize(u32 size)
	{
		String cpy = *this;
		clear();
		if (size > 0)
		{
			_str = (char*)::malloc(sizeof(char) * (size + 1));
			MUON_ASSERT_BREAK(_str != NULL, "resize() failed!");
			::strncpy(_str, cpy._str, cpy._charcount < size ? cpy._charcount : size);
			*(_str + size) = 0;
			_memsize = cpy._memsize;
			_charcount = cpy._charcount;
		}
	}

	const char* String::cStr() const
	{
		return (_memsize > 0 ? _str : "");
	}

	bool String::empty() const
	{
		return _charcount == 0;
	}

	u32 String::size() const
	{
		return _charcount;
	}

	u32 String::find(char c, u32 pos) const
	{
		if (pos >= _charcount)
		{
			return INVALID_INDEX;
		}

		for (u32 i = pos; i < _charcount; ++i)
		{
			if (*(_str + i) == c)
			{
				return i;
			}
		};
		return INVALID_INDEX;
	}

	u32 String::find(const char* other, u32 pos) const
	{
		u32 slen = ::strlen(other);
		if (pos + slen > _charcount)
		{
			return INVALID_INDEX;
		}

		u32 maxcount = _charcount - slen;
		while (pos <= maxcount)
		{
			if (*(_str + pos) == *other)
			{
				bool ok = true;
				u32 i = 0;
				for (; i < slen; ++i)
				{
					ok &= (*(_str + i + pos) == *(other + i));
					//Useless to continue iterating
					if (!ok)
					{
						break;
					}
				}

				if (ok)
				{
					return pos;
				}
				else
				{
					pos += i;
				}
			}
			++pos;
		}
		return INVALID_INDEX;
	}

	u32 String::find(const String& other, u32 pos) const
	{
		return find(other.cStr(), pos);
	}

	String String::substr(u32 pos, u32 len) const
	{
		if (pos >= _charcount)
		{
			return String();
		}

		if (len == 0)
		{
			len = _charcount;
		}

		if ((pos + len) >= _charcount)
		{
			len = _charcount - pos;
		}

		String s(len + 1, 0);
		::strncpy(s._str, _str + pos, len);
		*(s._str + len) = 0;
		s._charcount = len;
		return s;
	}

	bool String::checkUTF8() const
	{
		// Reference:
		// http://en.wikipedia.org/wiki/UTF-8
		// This will be used to know if current byte is either a "single" or "multi" byte indicator
		const u8 mask[4] = { 0x80, 0xE0, 0xF0, 0xF8 };

		// if(c & mask[i]) == maskresult then it's (i+1) byte(s)
		const u8 maskResult[4] = { 0x00, 0xC0, 0xE0, 0xF0 };

		//Check, for multibyte char, sub byte
		const u8 subByte = 0xC0;
		const u8 subByteResult = 0x80;

		//Iterate over all character
		for (u32 c = 0; c < _charcount; ++c)
		{
			u8 nbByte = 0;
			u8 idxByte = 0;

			// Look for byte indicator, reverse order
			for (int i = 0; i < 4; ++i)
			{
				if (((u8)*(_str + c) & mask[i]) == maskResult[i])
				{
					idxByte = i;
					nbByte = i + 1;
					break;
				}
			}

			// If byte indicator is invalid, then just skip
			if (nbByte == 0)
			{
				return false;
			}

			// Loop for (if any) others
			for (int i = 1; i < nbByte; ++i)
			{
				if ((*(_str + c + i) & subByte) != subByteResult)
				{
					// An error occured, return false.
					return false;
				}
				++c;
			}
		}

		return true;
	}

	u64 String::hash() const
	{
		// www.cse.yorku.ca/~oz/hash.html
		u64 v = 5381;
		if (_str)
		{
			char* str = (char*)_str;
			int c;
			while ((c = *str++) != 0)
			{
				v = ((v << 5) + v) + c;
			}
		}
		return v;
	}

	MUON_ILOGGABLE_IMPL(String, stream)
	{
		return stream << cStr();
	}
}

muon::String operator+(const muon::String& str, const char* other)
{
	muon::String s(str);
	s += other;
	return s;
}

muon::String operator+(const muon::String& str, const muon::String& other)
{
	muon::String s(str);
	s += other;
	return s;
}

muon::String operator+(const muon::String& str, const char c)
{
	muon::String s(str);
	s += c;
	return s;
}

muon::String operator+(const char* str, const muon::String& other)
{
	return muon::String(str) + other;
}
