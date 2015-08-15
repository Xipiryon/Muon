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

#ifndef _MUON_STRING_H_INCLUDED
#define _MUON_STRING_H_INCLUDED

#include <functional>
#include <ostream>
#include "Muon/Core/Constant.hpp"
#include "Muon/Meta/TypeTraits.hpp"
#include "Muon/Memory/ISerializable.hpp"
#include "Muon/System/ILoggable.hpp"

namespace muon
{
	//! Represents array of characters.
	class MUON_API String : public system::ILoggable
	{
	public:
		MUON_ILOGGABLE_DECL;

		String();
		String(const String& str);
		String(const char* str);
		String(u32 size, char c);
		~String();

		String& operator=(const char* other);
		String& operator=(const String& other);

		char operator[](u32 i) const;
		char& operator[](u32 i);

		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator==(const muon::String& other) const;
		bool operator!=(const muon::String& other) const;

		muon::String& operator+=(const char* other);
		muon::String& operator+=(const muon::String& other);
		muon::String& operator+=(const char c);


		const char* cStr() const;
		bool empty() const;
		u32 size() const;

		void resize(u32 size);
		void clear();

		u32 find(char c, u32 pos = 0) const;
		u32 find(const char* s, u32 pos = 0) const;
		u32 find(const char* s, u32 pos, u32 len) const;
		u32 find(const String&, u32 pos = 0) const;

		String substr(u32 pos, u32 len = 0) const;

		/*!
		* @brief Check that current stored string is UTF8.
		* @return True if every characters are encoded as UTF8.
		*/
		bool checkUTF8() const;

		u64 hash() const;

	private:
		u32 _charcount;
		u32 _memsize;
		char* _str;
	};
}

MUON_API muon::String operator+(const muon::String& str, const char other);
MUON_API muon::String operator+(const muon::String& str, const char* other);
MUON_API muon::String operator+(const muon::String& str, const muon::String& other);
MUON_API muon::String operator+(const char* str, const muon::String& other);

MUON_TRAITS(muon::String)

namespace std
{
	//Return address of first char
	template<> struct hash<muon::String>
	{
		muon::u32 operator()(const muon::String& str) const
		{
			return (muon::u32)str.hash();
		}
	};
}

#endif
