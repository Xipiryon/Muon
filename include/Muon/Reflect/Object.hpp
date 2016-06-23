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

#ifndef INCLUDE_MUON_REFLECT_OBJECT_HPP
#define INCLUDE_MUON_REFLECT_OBJECT_HPP

#include <memory>
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Detail/ObjectHolder.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace reflect
	{
		class Class;
		class MUON_API Object
		{
			static Object ref(const char* str);
			static Object ref(char* str);

			static Object copy(Object& obj);

			Object(detail::IObjectHolder*, const String&);
		public:

			template<typename T>
			static Object copy(const T& obj);
			static Object copy(const char* str);

			template<typename T>
			static Object ref(T& obj);
			static Object ref(Object& obj);

			Object();
			Object(const Object& o);

			void* object() const;
			template<typename T>
			bool check() const;

			template<typename T>
			T& as() const;

			bool isCopy() const;

			u64 id() const;
			u32 size() const;
			const String& name() const;

		private:
			std::shared_ptr<detail::IObjectHolder> m_objectHolder;
			const Class* m_class;
			bool m_isCopy;
		};
	}
}

MUON_TRAITS_DECL(m::reflect::Object);

#include "Muon/Reflect/Detail/ValueMapper.hpp"

template<typename T>
m::reflect::Object m::reflect::Object::copy(const T& obj)
{
	Object userObj(MUON_NEW(m::reflect::detail::ObjectHolderCopy<T>, const_cast<T*>(&obj))
				   , traits::TypeTraits<T>::name());
	userObj.m_isCopy = true;
	return userObj;
}

template<typename T>
m::reflect::Object m::reflect::Object::ref(T& obj)
{
	Object userObj(MUON_NEW(m::reflect::detail::ObjectHolderRef<T>, const_cast<T*>(&obj))
				   , traits::TypeTraits<T>::name());
	userObj.m_isCopy = false;
	return userObj;
}

template<typename T>
bool m::reflect::Object::check() const
{
	return detail::ObjectCompatibility<T>::check(*this);
}

template<typename T>
T& m::reflect::Object::as() const
{
	return *(T*)object();
}

#endif
