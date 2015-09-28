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

#ifndef _MUON_VARIANTSTRING_H_INCLUDED
#define _MUON_VARIANTSTRING_H_INCLUDED

#include "Muon/Meta/Variant.hpp"
#include "Muon/Type/String.hpp"

namespace muon
{
	namespace meta
	{
		template <>
		muon::String& Variant::get()
		{
			return *reinterpret_cast<muon::String*>(*reinterpret_cast<muon::String**>(_data));
		}

		template <>
		const muon::String& Variant::get() const
		{
			return *reinterpret_cast<muon::String*>(*reinterpret_cast<muon::String**>(_data));
		}

		template <>
		Variant& Variant::operator=(const muon::String& rhs)
		{
			return this->operator=(&rhs);
		}

		template <>
		Variant& Variant::set(const muon::String& rhs)
		{
			return this->operator=(&rhs);
		}
	}
}

#endif
