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

#ifndef INCLUDE_MUON_METADATABASE_HPP
#define INCLUDE_MUON_METADATABASE_HPP

#include <unordered_map>
#include "Muon/Memory/Allocator.hpp"
#include "Muon/Meta/MetaData.hpp"
#include "Muon/Meta/MetaFactory.hpp"
#include "Muon/Helper/Singleton.hpp"

namespace muon
{
	namespace meta
	{
		/*!
		* @brief
		*
		*/
		class MUON_API MetaDatabase : public helper::Singleton<MetaDatabase>
		{
			friend class helper::Singleton<MetaDatabase>;
		public:
			template<typename T>
			MetaData* registerMeta();

			MetaData* createMeta(const String& name);

			template<typename T>
			MetaData* getMeta();

			MetaData* getMeta(const String& name);

		private:
			MetaDatabase();
			~MetaDatabase();

			void registerInternal();

			typedef std::unordered_map<String, MetaData> MapMetaType;
			MapMetaType* m_metadb;
		};


		template<typename T>
		MetaData* MetaDatabase::registerMeta()
		{
			String name = traits::TypeTraits<T>::name();
			if (m_metadb->find(name) == m_metadb->end())
			{
				(*m_metadb)[name] = MetaData(traits::TypeTraits<T>());
			}
			else
			{
				MUON_ERROR("\"%s\" has already been registered", name.cStr());
			}
			return &(*m_metadb)[name];
		}

		template<typename T>
		MetaData* MetaDatabase::getMeta()
		{
			return getMeta(traits::TypeTraits<T>::name());
		}

		namespace priv
		{
			template<typename T>
			struct MetaRegistrar
			{
				MetaRegistrar()
				{
					if(!::muon::helper::Singleton<MetaDatabase>::isInstantiated())
					{
						::muon::meta::MetaDatabase::instantiate();
					}
					::muon::meta::MetaDatabase::get().registerMeta<T>();
				}
			};
		}
	}
}

#define MUON_META(Type) ::muon::meta::MetaDatabase::get().getMeta<typename ::muon::traits::RawType<Type>::type >()
#define MUON_META_OBJECT(Object) MUON_META(::muon::traits::RawType<decltype(Object)>::type)
#define MUON_META_NAME(Name) ::muon::meta::MetaDatabase::get().getMeta(Name)

#define MUON_META_REGISTER(Type) ::muon::meta::MetaDatabase::get().registerMeta<Type>()
#define MUON_META_CREATE(TypeName) ::muon::meta::MetaDatabase::get().createMeta(TypeName);

#define MUON_TRAITS_META_REGISTER(Type) MUON_TRAITS(Type) static struct MUON_GLUE_LINE(Registrar__) : muon::meta::priv::MetaRegistrar<Type> { MUON_GLUE_LINE(Registrar__)() : MetaRegistrar<Type>() {} } MUON_GLUE_COUNTER(s_instance);

#endif

