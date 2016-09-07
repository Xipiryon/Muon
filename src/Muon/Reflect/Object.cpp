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

#include "Muon/Reflect/Object.hpp"
#include "Muon/Reflect/Detail/ClassDatabase.hpp"

namespace m
{
	namespace reflect
	{
		Object::Object(detail::IObjectHolder* object, const String& className)
			: m_objectHolder(object)
			, m_class(&detail::ClassDatabase::getInstance().get(className))
		{
		}

		Object::Object()
			: m_objectHolder()
			, m_class(&detail::ClassDatabase::getInstance().get("void"))
			, m_isCopy(true)
		{
		}

		Object::Object(const Object& o)
			: m_objectHolder(o.m_objectHolder)
			, m_class(o.m_class)
			, m_isCopy(o.m_isCopy)
		{
		}

		Object Object::copy(const char* str)
		{
			return copy(String(str));
		}

		Object Object::ref(Object& obj)
		{
			return obj;
		}

		void* Object::object() const
		{
			return m_objectHolder.get()->object();
		}

		bool Object::isCopy() const
		{
			return m_isCopy;
		}

		u64 Object::id() const
		{
			return m_class->id();
		}

		u32 Object::size() const
		{
			return m_class->size();
		}

		const String& Object::name() const
		{
			return m_class->name();
		}
	}
}
