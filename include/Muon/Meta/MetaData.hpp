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

#ifndef INCLUDE_MUON_METADATA_HPP
#define INCLUDE_MUON_METADATA_HPP

#include <unordered_map>
#include "Muon/System/Assert.hpp"
#include "Muon/Meta/TypeTraits.hpp"
#include "Muon/Type/String.hpp"

/*!
* @file MetaData.hpp
*/
namespace muon
{
	//! Meta classes are used mostly for reflection system
	namespace meta
	{
		class MetaData;
		class MetaDatabase;
		class Variant;

		/*!
		* @brief
		*
		*/
		class MUON_API MetaFunction
		{
			friend class MetaData;
		public:
			/*
			MetaFunction(MetaData* meta, const String& name, unsigned offset);
			~MetaFunction();

			const String& name() const;
			u32 offset() const;
			MetaData* meta() const;

			private:
			MetaData* m_meta;
			String m_name;
			u32 m_offset;
			// */
		};

		/*!
		* @brief
		*
		*/
		class MUON_API MetaMember
		{
			friend class MetaData;
		public:
			MetaMember(MetaData* meta, const String& name, u32 offset);
			MetaMember(const MetaMember& o);
			~MetaMember();

			MetaMember& operator=(const MetaMember& o);
			const String& name() const;
			u32 offset() const;
			MetaData* meta() const;

		private:
			MetaData* m_meta;
			String m_name;
			u32 m_offset;
		};

		/*!
		* @brief Meta class representation
		*
		*/
		class MUON_API MetaData
		{
			friend class MetaDatabase;
			typedef std::unordered_map<String, meta::MetaMember> MetaMemberMap;
			typedef std::unordered_map<String, meta::MetaFunction> MetaMethodMap;
			typedef void*(*MetaCreateFunc)();
			typedef void(*MetaDestroyFunc)(void*);
		public:
			MetaData();
			MetaData(const MetaData&);
			MetaData& operator=(const MetaData&);

			template<typename T>
			MetaData(const TypeTraits<T>&)
				: m_name(TypeTraits<T>::name())
				, m_id(TypeTraits<T>::id())
				, m_size(TypeTraits<T>::size())
				, m_create((MetaCreateFunc)&TypeTraits<T>::create)
				, m_destroy((MetaDestroyFunc)&TypeTraits<T>::destroy)
			{
				init();
			}

			~MetaData();

			// Accessor
			const String& name() const;
			u64 id() const;
			u32 size() const;
			bool isCustom() const;

			// Attribute & Methods
			u32 getAttributeCount() const;
			u32 getMethodCount() const;

			template<typename C, typename R>
			MetaData* bindAttribute(const char* name, R C::*member);

			template<typename T>
			MetaData* addAttribute(const char* name);
			MetaData* addAttribute(const char* name, const MetaData* meta);

			MetaMember getAttribute(const char* name) const;
			MetaMember getAttribute(u32 offset) const;

			MetaFunction getMethod(const char* name) const;
			MetaFunction getMethod(u32 offset) const;

		private:
			void init();

			String m_name;
			u64 m_id;
			u32 m_size;
			MetaCreateFunc m_create;
			MetaDestroyFunc m_destroy;

			MetaMemberMap* m_members;
			MetaMethodMap* m_methods;
		};


		template<typename C, typename R>
		MetaData* MetaData::bindAttribute(const char* name, R C::*member)
		{
			return this;
		}

		template<typename T>
		MetaData* MetaData::addAttribute(const char* name)
		{
			MetaData meta = TypeTraits<T>();
			return addAttribute(name, &meta);
		}
	}
}

#endif

