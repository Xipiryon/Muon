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

#include "Muon/IO/IStream.hpp"
#include "Muon/System/Log.hpp"
#include "Muon/System/Assert.hpp"
#include "Muon/String.hpp"

namespace muon
{
	String::String()
		: m_str(NULL)
		, m_charcount(0)
		, m_memsize(0)
	{
	}

	String::String(const char* str)
		: String()
	{
		if (str && *str != 0)
		{
			m_memsize = ::strlen(str);
			m_charcount = m_memsize;
			m_str = (char*)::malloc(sizeof(char) * (m_memsize + 1));
			MUON_ASSERT_BREAK(m_str != NULL, "String(const char*) failed!");
			::strcpy(m_str, str);
			*(m_str + m_memsize) = 0;
		}
	}

	String::String(const String& str)
		: String()
	{
		if (str.m_charcount > 0)
		{
			m_charcount = str.m_charcount;
			m_memsize = m_charcount;
			m_str = (char*)::malloc(sizeof(char) * (m_charcount + 1));
			MUON_ASSERT_BREAK(m_str != NULL, "String(const String&) failed!");
			::strcpy(m_str, str.m_str);
			*(m_str + m_charcount) = 0;
		}
	}

	String::String(u32 size, char c)
		: String()
	{
		if (size != 0)
		{
			m_memsize = (size + 1) * sizeof(char);
			m_str = (char*)::malloc(m_memsize);
			MUON_ASSERT_BREAK(m_str != NULL, "String(u32, char) failed!");
			if (c != 0)
			{
				m_charcount = size;
				::memcpy(m_str, &c, m_memsize);
				*(m_str + m_charcount) = 0;
			}
			else
			{
				(*m_str) = 0;
			}
		}
	}

	String::~String()
	{
		if (m_str)
		{
			free(m_str);
		}
	}

	char String::operator[](u32 i) const
	{
		return *(m_str + i);
	}

	char& String::operator[](u32 i)
	{
		return *(m_str + i);
	}


	bool String::operator==(const char* other) const
	{
		if (m_str == NULL || other == NULL)
		{
			return false;
		}
		return (strcmp(m_str, other) == 0);
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
			if (m_charcount + size >= m_memsize)
			{
				m_str = (char*)::realloc(m_str, sizeof(char) * (m_charcount + size + 1));
				m_memsize = m_charcount + size;
			}
			::strncpy(m_str + m_charcount, other, size);
			m_charcount += size;
			*(m_str + m_charcount) = 0;
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
			m_charcount = ::strlen(str);
			m_memsize = m_charcount;
			m_str = (char*)::malloc(sizeof(char) * (m_charcount + 1));
			MUON_ASSERT_BREAK(m_str != NULL, "operator=() failed!");
			::strcpy(m_str, str);
			*(m_str + m_charcount) = 0;
		}
		return *this;
	}

	String& String::operator=(const String& other)
	{
		return operator=(other.cStr());
	}

	void String::clear()
	{
		if (m_str)
		{
			::free(m_str);
			m_str = NULL;
			m_memsize = 0;
			m_charcount = 0;
		}
	}

	void String::resize(u32 size)
	{
		String cpy = *this;
		clear();
		if (size > 0)
		{
			m_str = (char*)::malloc(sizeof(char) * (size + 1));
			MUON_ASSERT_BREAK(m_str != NULL, "resize() failed!");
			::strncpy(m_str, cpy.m_str, cpy.m_charcount < size ? cpy.m_charcount : size);
			*(m_str + size) = 0;
			m_memsize = cpy.m_memsize;
			m_charcount = cpy.m_charcount;
		}
	}

	const char* String::cStr() const
	{
		return (m_memsize > 0 ? m_str : "");
	}

	bool String::empty() const
	{
		return m_charcount == 0;
	}

	u32 String::size() const
	{
		return m_charcount;
	}

	u32 String::find(char c, u32 pos) const
	{
		if (pos >= m_charcount)
		{
			return INVALID_INDEX;
		}

		for (u32 i = pos; i < m_charcount; ++i)
		{
			if (*(m_str + i) == c)
			{
				return i;
			}
		};
		return INVALID_INDEX;
	}

	u32 String::find(const char* other, u32 pos) const
	{
		u32 slen = ::strlen(other);
		if (pos + slen > m_charcount)
		{
			return INVALID_INDEX;
		}

		u32 maxcount = m_charcount - slen;
		while (pos <= maxcount)
		{
			if (*(m_str + pos) == *other)
			{
				bool ok = true;
				u32 i = 0;
				for (; i < slen; ++i)
				{
					ok &= (*(m_str + i + pos) == *(other + i));
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
		if (pos >= m_charcount)
		{
			return String();
		}

		if (len == 0)
		{
			len = m_charcount;
		}

		if ((pos + len) >= m_charcount)
		{
			len = m_charcount - pos;
		}

		String s(len + 1, 0);
		::strncpy(s.m_str, m_str + pos, len);
		*(s.m_str + len) = 0;
		s.m_charcount = len;
		return s;
	}

	String String::replace(const String& src, const String& dst) const
	{
		return replace(src.cStr(), dst.cStr());
	}

	String String::replace(const char* src, const char* dst) const
	{
		u32 src_size = strlen(src);
		String s;
		u32 pos[2] = { 0, 0 };
		u32 off = 0;
		u32 len = 0;
		while ((pos[0] = find(src, pos[1])) != INVALID_INDEX)
		{
			len = pos[0] - off;
			if(len > 0)
			{
				s += substr(off, len);
			}
			s += dst;

			off = pos[0] + src_size;
			pos[1] = pos[0] + 1;
		}
		if(off < size())
		{
			s += substr(off);
		}
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
		for (u32 c = 0; c < m_charcount; ++c)
		{
			u8 nbByte = 0;
			u8 idxByte = 0;

			// Look for byte indicator, reverse order
			for (int i = 0; i < 4; ++i)
			{
				if (((u8)*(m_str + c) & mask[i]) == maskResult[i])
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
				if ((*(m_str + c + i) & subByte) != subByteResult)
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
		if (m_str)
		{
			char* str = (char*)m_str;
			int c;
			while ((c = *str++) != 0)
			{
				v = ((v << 5) + v) + c;
			}
		}
		return v;
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

muon::system::ILogImpl& operator<<(muon::system::ILogImpl& log, const muon::String& str)
{
	return (log << str.cStr());
}
