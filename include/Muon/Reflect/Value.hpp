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

#ifndef INCLUDE_MUON_REFLECT_VALUE_HPP
#define INCLUDE_MUON_REFLECT_VALUE_HPP

#include "Muon/Traits/Variant.hpp"
#include "Muon/Reflect/Type.hpp"
//#include "Muon/Reflect/Enum.hpp"
//#include "Muon/Reflect/UserObject.hpp"

namespace m
{
	namespace reflect
	{
		class MUON_API Value
		{
		public:
			static const Value EMPTY;

			Value();
			Value(const Value& o);

			template<typename T>
			static Value copy(const T& o);
			static Value copy(const char* o);

			template<typename T>
			static Value ref(const T& o);

			template<typename T>
			operator T&() const;

			template<typename T>
			T& get() const;

			template<typename T>
			bool compatible() const;

			bool isReference() const;
			u64 id() const;
			u32 size() const;
			const String& name() const;

		private:
			m::traits::Variant<None
				, bool, u64, f64, String
				//, EnumValue
				//, UserObject
			> m_value;
			void* m_pointer;

			u64 m_id;
			u32 m_size;
			String m_name;

			template<typename T>
			void _setup()
			{
				m_id = traits::TypeTraits<T>::id();
				m_size = traits::TypeTraits<T>::size();
				m_name = traits::TypeTraits<T>::name();
			}
		};
	}
}

#include "Muon/Reflect/Detail/ValueMapper.hpp"

template<typename T>
m::reflect::Value m::reflect::Value::copy(const T& value)
{
	Value v;
	v.m_value = value;
	v.m_pointer = NULL;
	v._setup<T>();
	return v;
}

template<typename T>
m::reflect::Value m::reflect::Value::ref(const T& value)
{
	Value v;
	v.m_value.reset();
	v.m_pointer = (void*)&value;
	v._setup<T>();
	return v;
}

template<typename T>
m::reflect::Value::operator T&() const
{
	return get<T>();
}

template<typename T>
T& m::reflect::Value::get() const
{
	return  (m_pointer == NULL ? m_value.get<T>() : *(T*)m_pointer);
}

template<typename T>
bool m::reflect::Value::compatible() const
{
	return false;//detail::ValueCompatibility<T>::compatible(*this);
}

#endif
