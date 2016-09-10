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
#include "Muon/Reflect/Class.hpp"

namespace m
{
	namespace reflect
	{
		ClassBuilder::ClassBuilder(Class& classParam)
			: m_class(classParam)
		{
		}

		ClassBuilder& ClassBuilder::value(const String& name, m::i32 value)
		{
			return *this;
		}

		Class::Class()
			: m_id(traits::TypeTraits<void>::id())
			, m_size(traits::TypeTraits<void>::size())
			, m_name(traits::TypeTraits<void>::name())
		{
		}


		ClassBuilder Class::declare(const String& name)
		{
			auto& c = detail::ClassDatabase::getInstance().add(name);
			c.m_name = name;
			return ClassBuilder(c);
		}

		detail::ClassDatabase& Class::database()
		{
			return detail::ClassDatabase::getInstance();
		}

		u64 Class::id() const
		{
			return m_id;
		}

		u32 Class::size() const
		{
			return m_size;
		}

		const String& Class::name() const
		{
			return m_name;
		}
	}
}
