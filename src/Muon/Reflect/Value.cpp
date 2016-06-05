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

#include "Muon/Reflect/Value.hpp"

namespace m
{
	namespace reflect
	{
		Value::Value()
			: m_value()
			, m_pointer(NULL)
			, m_id(traits::TypeTraits<None>::id())
			, m_size(traits::TypeTraits<None>::size())
			, m_name(traits::TypeTraits<None>::name())
		{
		}

		Value::Value(const Value& o)
			: m_value(o.m_value)
			, m_pointer(o.m_pointer)
			, m_id(o.m_id)
			, m_size(o.m_size)
			, m_name(o.m_name)
		{
		}

		Value Value::copy(const char* value)
		{
			Value v;
			v.m_value = m::String(value);
			v.m_pointer = NULL;
			v._setup<m::String>();
			return v;
		}

		bool Value::isReference() const
		{
			return (m_pointer != NULL);
		}

		u64 Value::id() const
		{
			return m_id;
		}

		u32 Value::size() const
		{
			return m_size;
		}

		const String& Value::name() const
		{
			return m_name;
		}
	}
}
