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

namespace
{
	const m::reflect::Enum INVALID_ENUM;
}

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			Enum& EnumDatabase::add(const String& name)
			{
				auto it = m_enums.find(name);
				MUON_ASSERT_BREAK(it == m_enums.end(), "Enum '%s' already registered!", name.cStr());

				if (it == m_enums.end())
				{
					return m_enums[name];
				}

				return it->second;
			}

			void EnumDatabase::remove(const String& name)
			{
				auto it = m_enums.find(name);
				MUON_ASSERT_BREAK(it != m_enums.end(), "Enum '%s' not registered!", name.cStr());

				if (it != m_enums.end())
				{
					m_enums.erase(it);
				}
			}

			const Enum& EnumDatabase::retrieve(const String& name)
			{
				auto it = m_enums.find(name);
				MUON_ASSERT_BREAK(it != m_enums.end(), "Enum '%s' not registered!", name.cStr());

				if (it != m_enums.end())
				{
					return it->second;
				}
				return INVALID_ENUM;
			}

			u32 EnumDatabase::size() const
			{
				return m_enums.size();
			}

			bool EnumDatabase::exists(const String& name)
			{
				return (m_enums.find(name) != m_enums.end());
			}
		}
	}
}
