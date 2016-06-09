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

#ifndef INCLUDE_MUON_TRAITS_VARIANTDETAIL_HPP
#define INCLUDE_MUON_TRAITS_VARIANTDETAIL_HPP

#include "Muon/Traits/Variant.hpp"

namespace m
{
	namespace traits
	{
		namespace detail
		{
			// Recursive VariantHelper
			// ***************
			template<typename T, typename...Ts>
			void VariantHelper<T, Ts...>::create(u64 cId, u64 nId, const char* name, void* data)
			{
				MUON_ASSERT_BREAK(cId == INVALID_TYPE_ID
								  , "Variant not empty when trying to create '%s', stored '%s'"
								  , TypeTraits<T>::name(), name);
				if (cId == INVALID_TYPE_ID)
				{
					(nId == TypeTraits<T>::id() ? VariantHelper<T>::create(cId, nId, name, data) : VariantHelper<Ts...>::create(cId, nId, name, data));
				}
			}

			template<typename T, typename...Ts>
			void VariantHelper<T, Ts...>::copy(u64 id, const char* name, void* dst, const void* src)
			{
				(id == TypeTraits<T>::id() ? VariantHelper<T>::copy(id, name, dst, src) : VariantHelper<Ts...>::copy(id, name, dst, src));
			}

			template<typename T, typename...Ts>
			void VariantHelper<T, Ts...>::destroy(u64 id, const char* name, void* data)
			{
				(id == TypeTraits<T>::id() ? VariantHelper<T>::destroy(id, name, data) : VariantHelper<Ts...>::destroy(id, name, data));
			}

			// Final VariantHelper
			// ***************

			template<typename T>
			void VariantHelper<T>::create(u64 /*unused*/, u64 id, const char* name, void* data)
			{
				MUON_ASSERT_BREAK(id == TypeTraits<T>::id()
								  , "Variant not able to store '%s' type!"
								  , TypeTraits<T>::name());
				if (id == TypeTraits<T>::id())
				{
					new (data)T();
				}
			}

			template<typename T>
			void VariantHelper<T>::copy(u64 id, const char* name, void* dst, const void* src)
			{
				MUON_ASSERT_BREAK(id == TypeTraits<T>::id()
								  , "Variant type mismatch when trying to copy '%s' (current '%s')"
								  , TypeTraits<T>::name(), name);
				if (id == TypeTraits<T>::id())
				{
					*(T*)dst = *(T*)src;
				}
			}

			template<typename T>
			void VariantHelper<T>::destroy(u64 id, const char* name, void* data)
			{
				MUON_ASSERT_BREAK(id == INVALID_TYPE_ID || id == TypeTraits<T>::id()
								  , "Variant Type mismatch when trying to destroy '%s' (current '%s')"
								  , TypeTraits<T>::name(), name);

				if (id != INVALID_TYPE_ID && id == TypeTraits<T>::id())
				{
					((T*)data)->~T();
				}
			}
		}
	}
}

#endif
