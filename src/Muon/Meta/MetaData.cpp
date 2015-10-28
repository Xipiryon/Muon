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
			: m_meta(meta)
			, m_name(name)
			, m_offset(offset)
		{
		}

		MetaMember::~MetaMember()
		{
		}

		MetaMember::MetaMember(const MetaMember& o)
			: m_meta(o.m_meta)
			, m_name(o.m_name)
			, m_offset(o.m_offset)
		{
		}

		MetaMember& MetaMember::operator=(const MetaMember& o)
		{
			if(this != &o)
			{
				m_meta = o.m_meta;
				m_name = o.m_name;
				m_offset = o.m_offset;
			}
			return *this;
		}

		const char* MetaMember::name() const
		{
			return m_name.cStr();
		}

		u32 MetaMember::offset() const
		{
			return m_offset;
		}

		MetaData* MetaMember::meta() const
		{
			return m_meta;
		}

		// *****************************
		// 			META DATA
		// *****************************
		void MetaData::init()
		{
			m_members = MUON_CNEW(MetaMemberMap);
			m_methods = MUON_CNEW(MetaMethodMap);
		}

		MetaData::MetaData()
			: MetaData(muon::traits::TypeTraits<void>())
		{
			init();
		}

		MetaData::MetaData(const MetaData& o)
		{
			init();
			*this = o;
		}

		MetaData& MetaData::operator=(const MetaData& o)
		{
			if(this != &o)
			{
				m_name = o.m_name;
				m_id = o.m_id;
				m_size = o.m_size;
				m_flags = o.m_flags;
				m_create = o.m_create;
				m_destroy = o.m_destroy;
				m_copy = o.m_copy;
				m_members->operator=(*o.m_members);
				m_methods->operator=(*o.m_methods);
			}
			return *this;
		}

		MetaData::~MetaData()
		{
			MUON_CDELETE(m_members);
			MUON_CDELETE(m_methods);
		}

		const char* MetaData::name() const
		{
			return m_name.cStr();
		}

		u64 MetaData::id() const
		{
			return m_id;
		}

		u32 MetaData::size() const
		{
			return m_size;
		}

		u32 MetaData::flags() const
		{
			return m_flags;
		}

		u32 MetaData::getAttributeCount() const
		{
			return m_members->size();
		}

		u32 MetaData::getMethodCount() const
		{
			return m_methods->size();
		}

		MetaData* MetaData::addAttribute(const char* name, const MetaData* meta)
		{
			auto it = m_members->find(name);
			if(it != m_members->end())
			{
				MUON_ERROR("Cannot add \"%s\" as %s: already added as %s!"
					, name, meta->name(), it->second.meta()->name());
				return it->second.meta();
			}

			// Use MetaDatabase pointer instead of given one
			MetaData* dbMeta = MUON_META_NAME(meta->name());
			m_members->insert( {name, MetaMember(dbMeta, name, m_size)} );
			m_size += meta->size();

			return this;
		}

		MetaMember MetaData::getAttribute(const char* name) const
		{
			for(auto pair : *m_members)
			{
				if(pair.second.name() == name)
				{
					return pair.second;
				}
			}
			return MetaMember(MUON_META(void), traits::TYPE_NAME_UNREGISTERED, 0);
		}

		MetaMember MetaData::getAttribute(u32 offset) const
		{
			for(auto pair : *m_members)
			{
				if(pair.second.offset() == offset)
				{
					return pair.second;
				}
			}
			return MetaMember(MUON_META(void), traits::TYPE_NAME_UNREGISTERED, 0);
		}

		MetaFunction MetaData::getMethod(const char* name) const
		{
			return MetaFunction();
		}

		MetaFunction MetaData::getMethod(u32 offset) const
		{
			return MetaFunction();
		}

		void* MetaData::create()
		{
			return m_create();
		}

		void MetaData::copy(void* data, const void* value)
		{
			m_copy(data, value);
		}

		void MetaData::destroy(void* data)
		{
			m_destroy(data);
		}
	}
}
