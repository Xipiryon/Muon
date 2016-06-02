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
#include "Muon/Reflect/Detail/ValueMapper.hpp"

namespace m
{
	namespace reflect
	{
		class MUON_API Value
		{
		public:
			Value();

			template<typename T>
			Value(const T& value)
			{
				m_value = detail::ValueMapper<T>::to(value);
			}

			template<typename T>
			operator T() const
			{
				return get<T>();
			}

			template<typename T>
			T get() const
			{
				return m::reflect::detail::ValueMapper<T>::from(m_value);
			}

			template<typename T>
			bool isCompatible() const
			{
				return detail::ValueCompatibility<T>::compatible(m_value);
			}

			u64 id() const;
			u32 size() const;
			const String& name() const;

		private:
			detail::ValueVariant m_value;
		};
	}
}

#endif
