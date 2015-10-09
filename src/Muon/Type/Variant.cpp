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

#include "Muon/Type/Variant.hpp"

namespace muon
{
	Variant::Variant(meta::MetaData* m, void* d)
		: m_meta(m)
		, m_data(d)
	{
	}

	Variant::Variant(const Variant& rhs)
		: Variant(rhs.m_meta, rhs.m_data)
	{
	}

	Variant::Variant()
		: m_meta(MUON_META(void))
		, m_data(NULL)
	{
	}

	meta::MetaData* Variant::getMeta() const
	{
		return m_meta;
	}

	Variant& Variant::operator=(const Variant& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		MUON_ASSERT(rhs.m_meta, "Cannot copy an NULL MetaData!");
		// Meta are different, erase the stored one and replace by the new
		if (m_meta != rhs.m_meta)
		{
			::free(m_data);
			m_meta = rhs.m_meta;
			m_data = ::malloc(m_meta->size());
			::memcpy(m_data, rhs.m_data, m_meta->size());
		}
		else
		{
			// They are the same, just copy the value
			::memcpy(m_data, rhs.m_data, m_meta->size());
		}
		return *this;
	}

	Variant& Variant::set(const Variant& rhs)
	{
		return this->operator=(rhs);
	}
}
