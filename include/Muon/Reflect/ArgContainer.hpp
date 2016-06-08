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

#ifndef INCLUDE_MUON_REFLECT_ARGCONTAINER_HPP
#define INCLUDE_MUON_REFLECT_ARGCONTAINER_HPP

#include <vector>
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Value.hpp"

namespace m
{
	namespace reflect
	{
		class MUON_API ArgContainer
		{
		public:
			static const ArgContainer EMPTY;

			template<typename T>
			ArgContainer& ref(const T& o);
			template<typename T>
			ArgContainer& copy(const T& o);

			ArgContainer& operator+=(const Value& o);
			ArgContainer operator+(const Value& o);

			u32 count() const;
			const Value& operator[](u32 i) const;
			Value& operator[](u32 i);

		private:
			std::vector<Value> m_args;
		};
	}
}

template<typename T>
m::reflect::ArgContainer& m::reflect::ArgContainer::ref(const T& o)
{
	*this += Value::ref(o);
	return *this;
}

template<typename T>
m::reflect::ArgContainer& m::reflect::ArgContainer::copy(const T& o)
{
	*this += Value::copy(o);
	return *this;
}

#endif
