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
		: _meta(m)
		, _data(d)
	{
	}

	Variant::Variant(const Variant& rhs)
		: Variant(rhs._meta, rhs._data)
	{
	}

	Variant::Variant()
		: _meta(MUON_META(void))
		, _data(NULL)
	{
	}

	meta::MetaData* Variant::getMeta() const
	{
		return _meta;
	}

	Variant& Variant::operator=(const Variant& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		MUON_ASSERT(rhs._meta, "Cannot copy an NULL MetaData!");
		// Meta are different, erase the stored one and replace by the new
		if (_meta != rhs._meta)
		{
			::free(_data);
			_meta = rhs._meta;
			_data = ::malloc(_meta->size());
			::memcpy(_data, rhs._data, _meta->size());
		}
		else
		{
			// They are the same, just copy the value
			::memcpy(_data, rhs._data, _meta->size());
		}
		return *this;
	}

	Variant& Variant::set(const Variant& rhs)
	{
		return this->operator=(rhs);
	}
}