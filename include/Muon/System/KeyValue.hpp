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

#ifndef INCLUDE_MUON_KEYVALUE_HPP
#define INCLUDE_MUON_KEYVALUE_HPP

#include "Muon/String.hpp"

namespace muon
{
	namespace system
	{
		class MUON_API KeyValue
		{
		public:
			static bool exists(const String& name);
			static bool erase(const String& name);

			static bool store(const String& name, const String& val);
			static bool store(const String& name, const char* val);
			static bool store(const String& name, u64 val);
			static bool store(const String& name, u32 val);
			static bool store(const String& name, u16 val);
			static bool store(const String& name, u8 val);
			static bool store(const String& name, i64 val);
			static bool store(const String& name, i32 val);
			static bool store(const String& name, i16 val);
			static bool store(const String& name, i8 val);
			static bool store(const String& name, f64 val);
			static bool store(const String& name, f32 val);
			static bool store(const String& name, bool val);

			static bool retrieve(const String& name, String& val);
			static bool retrieve(const String& name, u64& val);
			static bool retrieve(const String& name, u32& val);
			static bool retrieve(const String& name, u16& val);
			static bool retrieve(const String& name, u8& val);
			static bool retrieve(const String& name, i64& val);
			static bool retrieve(const String& name, i32& val);
			static bool retrieve(const String& name, i16& val);
			static bool retrieve(const String& name, i8& val);
			static bool retrieve(const String& name, f64& val);
			static bool retrieve(const String& name, f32& val);
			static bool retrieve(const String& name, bool& val);
		};
	}
}

#endif
