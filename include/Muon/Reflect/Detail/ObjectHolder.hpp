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

#ifndef INCLUDE_MUON_REFLECT_OBJECTHOLDER_HPP
#define INCLUDE_MUON_REFLECT_OBJECTHOLDER_HPP

#include "Muon/System/Assert.hpp"
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Type.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			class MUON_API IObjectHolder
			{
			public:
				virtual ~IObjectHolder()
				{
				}

				virtual void* object() const = 0;

			protected:
				IObjectHolder()
				{
				}
			};

			template<typename T>
			class ObjectHolderCopy : public IObjectHolder
			{
			public:
				ObjectHolderCopy(const T& obj)
					: m_object(obj)
				{
				}

				virtual void* object() const
				{
					return (void*)&m_object;
				}

			private:
				typename std::remove_const<T>::type m_object;
			};

			template<typename T>
			class ObjectHolderRef : public IObjectHolder
			{
			public:
				ObjectHolderRef(T* obj)
					: m_object(obj)
				{
				}

				virtual void* object() const
				{
					return static_cast<void*>(m_object);
				}

			private:
				T* m_object;
			};

			template<typename T>
			class ObjectHolderConstRef : public IObjectHolder
			{
			public:
				ObjectHolderConstRef(T* obj)
					: m_object(obj)
				{
				}

				virtual void* object() const
				{
					return static_cast<void*>(m_object);
				}

			private:
				T* m_object;
			};
		}
	}
}

#endif
