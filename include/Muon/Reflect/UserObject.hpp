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
		public:

			UserObject();

			template<typename T>
			UserObject(const T& obj)
			{
			}

			template<typename T>
			T getObjectCopy() const
			{
				return *static_cast<T*>(m_object->getObject());
			}

			template<typename T>
			T* getObjectPointer() const
			{
				return static_cast<T*>(m_object->getObject());
			}

			template<typename T>
			const T& getObjectConstRef() const
			{
				return *static_cast<T*>(m_object->getObject());
			}

		private:
			std::shared_ptr<detail::IObjectHolder> m_object;
		};
	}
}

MUON_TRAITS_DECL(m::reflect::UserObject);

#endif
