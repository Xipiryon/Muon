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

#include <iostream>
#include "Muon/System/Log.hpp"
#include "Muon/Meta/MetaDatabase.hpp"

namespace muon
{
	namespace meta
	{
		MetaDatabase::MetaDatabase()
			: m_metadb(MUON_CNEW(MapMetaType))
		{
			registerInternal();
		}

		MetaDatabase::~MetaDatabase()
		{
			MUON_CDELETE(m_metadb);
		}

		MetaData* MetaDatabase::createMeta(const String& name)
		{
			auto it = m_metadb->find(name);
			if (it != m_metadb->end())
			{
				return &(it->second);
			}

			(*m_metadb)[name] = MetaData();
			MetaData* metadb = &(*m_metadb)[name];
			metadb->m_name = name;
			metadb->m_id = traits::TYPE_ID_CUSTOM_MASK | (traits::TYPE_ID_BASE_MASK & name.hash());
			metadb->m_size = 0;
			metadb->m_flags = traits::IS_CUSTOM_FLAG;

			return metadb;
		}

		MetaData* MetaDatabase::getMeta(const String& name)
		{
			auto it = m_metadb->find(name);
			if (it != m_metadb->end())
			{
				return &(it->second);
			}
			MUON_ERROR("There is no such MetaData registered for the name %s", name.cStr());
			return NULL;
		}
	}
}
