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

#ifndef INCLUDE_MUON_REFLECT_ENUM_HPP
#define INCLUDE_MUON_REFLECT_ENUM_HPP

#include <unordered_map>
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace reflect
	{
		class Enum;

		class MUON_API EnumBuilder
		{
		public:
			EnumBuilder(Enum&);
			EnumBuilder& value(const String& name, i32 value);

		private:
			Enum& m_enum;
		};

		class MUON_API EnumValue
		{
		public:
			EnumValue();
			EnumValue(const Enum&, const String&, i32);
			EnumValue& operator=(const EnumValue& o);

			const Enum& getEnum() const;
			const String& name() const;
			i32 value() const;

		private:
			const Enum& m_enum;
			const String& m_name;
			const i32 m_value;
		};

		class MUON_API Enum
		{
			friend class EnumBuilder;
		public:

			static EnumBuilder declare(const String& name);

			String name() const;
			u32 size() const;

			EnumValue getByIndex(u32 index) const;
			EnumValue getByName(const String& name) const;
			EnumValue getByValue(i32 value) const;
		private:
			String m_name;
			std::unordered_map<String, i32> m_pairs;
		};
	}
}

MUON_TRAITS_DECL(m::reflect::EnumValue);

#endif
