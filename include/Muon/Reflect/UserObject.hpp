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

#ifndef INCLUDE_MUON_REFLECT_USEROBJECT_HPP
#define INCLUDE_MUON_REFLECT_USEROBJECT_HPP

#include <memory>
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Detail/ObjectHolder.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace reflect
	{
		class MUON_API UserObject
		{
			static UserObject copy(UserObject& obj);
		public:

			template<typename T>
			static UserObject copy(const T& obj);

			template<typename T>
			static UserObject ref(T& obj);
			static UserObject ref(UserObject& obj);

			UserObject();
			UserObject(const UserObject& o);

			void* object() const;
			bool isReference() const;

		private:
			std::shared_ptr<detail::IObjectHolder> m_objectHolder;
			bool m_isReference;
		};
	}
}

MUON_TRAITS_DECL(m::reflect::UserObject);

template<typename T>
m::reflect::UserObject m::reflect::UserObject::copy(const T& obj)
{
	UserObject userObj;
	userObj.m_objectHolder.reset(MUON_NEW(m::reflect::detail::ObjectHolderCopy<T>, const_cast<T*>(&obj)));
	userObj.m_isReference = false;
	return userObj;
}

template<typename T>
m::reflect::UserObject m::reflect::UserObject::ref(T& obj)
{
	UserObject userObj;
	userObj.m_objectHolder.reset(MUON_NEW(m::reflect::detail::ObjectHolderRef<T>, const_cast<T*>(&obj)));
	userObj.m_isReference = true;
	return userObj;
}

#endif
