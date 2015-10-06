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

#include <cstring>
#include "Muon/Memory/Allocator.hpp"
#include "Muon/Meta/MetaData.hpp"
#include "Muon/Meta/MetaDatabase.hpp"

namespace muon
{
	namespace meta
	{
		// *****************************
		// 			META FUNCTION
		// *****************************

		// *****************************
		// 			META MEMBER
		// *****************************
		MetaMember::MetaMember(MetaData* meta, const String& name, u32 offset)
			: _meta(meta)
			, _name(name)
			, _offset(offset)
		{
		}

		MetaMember::~MetaMember()
		{
		}

		MetaMember::MetaMember(const MetaMember& o)
			: _meta(o._meta)
			, _name(o._name)
			, _offset(o._offset)
		{
		}

		MetaMember& MetaMember::operator=(const MetaMember& o)
		{
			if(this != &o)
			{
				_meta = o._meta;
				_name = o._name;
				_offset = o._offset;
			}
			return *this;
		}

		const String& MetaMember::name() const
		{
			return _name;
		}

		u32 MetaMember::offset() const
		{
			return _offset;
		}

		MetaData* MetaMember::meta() const
		{
			return _meta;
		}

		// *****************************
		// 			META DATA
		// *****************************
		void MetaData::init()
		{
			_members = MUON_CNEW(MetaMemberMap);
			_methods = MUON_CNEW(MetaMethodMap);
		}

		MetaData::MetaData()
			: _name("#Unregistered#")
			, _id(TYPE_ID_INVALID)
			, _size(0)
		{
			init();
		}

		MetaData::MetaData(const MetaData& o)
			: _name(o._name)
			, _id(o._id)
			, _size(o._size)
		{
			init();
			_members->operator=(*o._members);
			_methods->operator=(*o._methods);
		}

		MetaData& MetaData::operator=(const MetaData& o)
		{
			if(this != &o)
			{
				_name = o._name;
				_id = o._id;
				_size = o._size;
				_members->operator=(*o._members);
				_methods->operator=(*o._methods);
			}
			return *this;
		}

		MetaData::~MetaData()
		{
			MUON_CDELETE(_members);
			MUON_CDELETE(_methods);
		}

		const String& MetaData::name() const
		{
			return _name;
		}

		u64 MetaData::id() const
		{
			return _id;
		}

		u32 MetaData::size() const
		{
			return _size;
		}

		bool MetaData::isCustom() const
		{
			return (_id & TYPE_ID_CUSTOM_MASK) == TYPE_ID_CUSTOM_MASK;
		}

		u32 MetaData::getAttributeCount() const
		{
			return _members->size();
		}

		u32 MetaData::getMethodCount() const
		{
			return _methods->size();
		}

		MetaData* MetaData::addAttribute(const char* name, const MetaData* meta)
		{
			auto it = _members->find(name);
			if(it != _members->end())
			{
				MUON_ERROR("Cannot add \"%s\" as %s: already added as %s!"
					, name, meta->name().cStr(), it->second.meta()->name().cStr());
				return it->second.meta();
			}

			// Use MetaDatabase pointer instead of given one
			MetaData* dbMeta = MUON_META_NAME(meta->name().cStr());
			_members->insert( {name, MetaMember(dbMeta, name, _size)} );
			_size += meta->size();

			return this;
		}

		MetaMember MetaData::getAttribute(const char* name) const
		{
			for(auto pair : *_members)
			{
				if(pair.second.name() == name)
				{
					return pair.second;
				}
			}
			return MetaMember(MUON_META(void), TYPE_NAME_UNREGISTERED, 0);
		}

		MetaMember MetaData::getAttribute(u32 offset) const
		{
			for(auto pair : *_members)
			{
				if(pair.second.offset() == offset)
				{
					return pair.second;
				}
			}
			return MetaMember(MUON_META(void), TYPE_NAME_UNREGISTERED, 0);
		}

		MetaFunction MetaData::getMethod(const char* name) const
		{
			return MetaFunction();
		}

		MetaFunction MetaData::getMethod(u32 offset) const
		{
			return MetaFunction();
		}
	}
}
