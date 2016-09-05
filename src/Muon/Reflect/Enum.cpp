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

#include "Muon/Reflect/Detail/EnumDatabase.hpp"
#include "Muon/Reflect/Enum.hpp"

namespace
{
	const m::reflect::Enum EMPTY_ENUM;
	const m::reflect::EnumValue EMPTY_ENUM_VALUE(EMPTY_ENUM, "#InvalidEnumValue#", -1);
}

namespace m
{
	namespace reflect
	{
		EnumBuilder::EnumBuilder(Enum& enumParam)
			: m_enum(enumParam)
		{
		}

		EnumBuilder& EnumBuilder::value(const String& name, m::i32 value)
		{
			m_enum.m_pairs[name] = value;
			return *this;
		}

		EnumValue::EnumValue()
			: EnumValue(EMPTY_ENUM_VALUE)
		{
		}

		EnumValue& EnumValue::operator=(const EnumValue& o)
		{
			MUON_ERROR("EnumValue can't be copied!");
			return *this;
		}

		EnumValue::EnumValue(const Enum& enumRef, const String& name_, i32 value_)
			: m_enum(enumRef)
			, m_name(name_)
			, m_value(value_)
		{
		}

		const Enum& EnumValue::getEnum() const
		{
			return m_enum;
		}

		const String& EnumValue::name() const
		{
			return m_name;
		}

		i32 EnumValue::value() const
		{
			return m_value;
		}

		EnumBuilder Enum::declare(const String& name)
		{
			auto& e = detail::EnumDatabase::getInstance().add(name);
			e.m_name = name;
			return EnumBuilder(e);
		}

		void Enum::undeclare(const String& name)
		{
			detail::EnumDatabase::getInstance().remove(name);
		}

		const Enum& Enum::retrieve(const String& name)
		{
			return detail::EnumDatabase::getInstance().retrieve(name);
		}

		String Enum::name() const
		{
			return m_name;
		}

		u32 Enum::size() const
		{
			return m_pairs.size();
		}

		EnumValue Enum::getByIndex(u32 index) const
		{
			MUON_ASSERT_BREAK(index < size(), "Not enough element in Enum!");
			if (index < size())
			{
				auto it = m_pairs.begin();
				while (index-- > 0)
				{
					++it;
				}
				return EnumValue(*this, it->first, it->second);
			}
			return EMPTY_ENUM_VALUE;
		}

		EnumValue Enum::getByName(const String& name) const
		{
			auto it = m_pairs.find(name);
			MUON_ASSERT_BREAK(it != m_pairs.end(), "Value '%s' is not registered in '%s'!", name.cStr(), m_name.cStr());
			if (it != m_pairs.end())
			{
				return EnumValue(*this, it->first, it->second);
			}
			return EMPTY_ENUM_VALUE;
		}

		EnumValue Enum::getByValue(i32 value) const
		{
			for (auto it = m_pairs.begin(); it != m_pairs.end(); it++)
			{
				if (it->second == value)
				{
					return EnumValue(*this, it->first, it->second);
				}
			}

			MUON_ERROR("No value '%d' is registered in '%s'!", value, m_name.cStr());
			return EMPTY_ENUM_VALUE;
		}
	}
}
