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

#include "Muon/Reflect/Detail/ClassDatabase.hpp"

namespace
{
	static const m::reflect::Class Default;
}

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			ClassDatabase::~ClassDatabase()
			{
			}

			Class& ClassDatabase::add(const m::String& name)
			{
				auto it = m_classes.find(name);
				MUON_ASSERT_BREAK(it == m_classes.end(), "Class '%s' already registered!", name.cStr());

				if (it == m_classes.end())
				{
					return m_classes[name];
				}

				return it->second;
			}

			void ClassDatabase::remove(const String& name)
			{
				auto it = m_classes.find(name);
				MUON_ASSERT_BREAK(it != m_classes.end(), "Class '%s' not registered!", name.cStr());

				if (it != m_classes.end())
				{
					m_classes.erase(it);
				}
			}

			const Class& ClassDatabase::retrieve(const String& name)
			{
				auto it = m_classes.find(name);
				MUON_ASSERT_BREAK(it != m_classes.end(), "Class '%s' not registered!", name.cStr());

				if (it != m_classes.end())
				{
					return it->second;
				}
				return Default;
			}

			u32 ClassDatabase::size() const
			{
				return m_classes.size();
			}

			bool ClassDatabase::exists(const String& name)
			{
				return (m_classes.find(name) != m_classes.end());
			}
		}
	}
}
