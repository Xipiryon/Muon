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

#ifndef _MUON_METADATA_H_INCLUDED
#define _MUON_METADATA_H_INCLUDED

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
			MetaData* _meta;
			String _name;
			u32 _offset;
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
			MetaData* _meta;
			String _name;
			u32 _offset;
		};

		/*!
		* @brief Meta class representation
		*
		*/
		class MUON_API MetaData
		{
			friend class MetaDatabase;
			friend class Variant;
		public:
			MetaData();
			MetaData(const MetaData&);
			MetaData& operator=(const MetaData&);

			template<typename T>
			MetaData(const TypeTraits<T>&)
				: _name(TypeTraits<T>::name())
				, _id(TypeTraits<T>::id())
				, _size(TypeTraits<T>::size())
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

			// Memory Management
			void* memNew() const;
			void* memNewCopy(const void* data) const;
			void memDelete(void* data) const;
			void memCopy(void* dest, const void* data) const;

			String _name;
			u64 _id;
			u32 _size;

			typedef std::unordered_map<String, meta::MetaMember> MetaMemberMap;
			typedef std::unordered_map<String, meta::MetaFunction> MetaMethodMap;
			MetaMemberMap* _members;
			MetaMethodMap* _methods;
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

