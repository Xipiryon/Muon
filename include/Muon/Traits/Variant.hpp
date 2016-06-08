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

#ifndef INCLUDE_MUON_TRAITS_VARIANT_HPP
#define INCLUDE_MUON_TRAITS_VARIANT_HPP

#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/System/Assert.hpp"
#include "Muon/Memory/Utils.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace traits
	{
		namespace detail
		{
			template<typename T, typename...Ts>
			struct VariantHelper
			{
				static const u32 size = sizeof(T) > VariantHelper<Ts...>::size ? sizeof(T) : VariantHelper<Ts...>::size;
				inline static void create(u64 id, void* data);
				inline static void copy(u64 id, void* dst, const void* src);
				inline static void destroy(u64 id, void* data);
			};

			template<typename T>
			struct VariantHelper<T>
			{
				static const u32 size = sizeof(T);
				inline static void create(u64 id, void* data);
				inline static void copy(u64 id, void* dst, const void* src);
				inline static void destroy(u64 id, void* data);
			};
		}

		template<typename...Ts>
		class MUON_API Variant
		{
			typedef detail::VariantHelper<Ts...> Helper;

			template<u32 Size>
			struct RawData
			{
				u8 data[Size];
			};

			template<typename T>
			void setupTraitsInfo()
			{
				m_id = TypeTraits<T>::id();
				m_size = TypeTraits<T>::size();
				m_name = TypeTraits<T>::name();
			}

		public:
			Variant()
				: m_id(INVALID_TYPE_ID)
				, m_size(0)
			{
			}

			Variant(const Variant<Ts...>& o)
				: Variant()
			{
				operator=(o);
			}

			~Variant()
			{
				Helper::destroy(m_id, &m_data);
			}

			Variant& operator=(const char* str)
			{
				return (*this = String(str));
			}

			Variant& operator=(const Variant<Ts...>& o)
			{
				if (&o != this)
				{
					if (id() != o.id())
					{
						Helper::destroy(m_id, &m_data);
						Helper::create(o.m_id, &m_data);
						m_id = o.m_id;
						m_size = o.m_size;
						m_name = o.m_name;
					}
					Helper::copy(o.m_id, &m_data, &o.m_data);
				}
				return *this;
			}

			template<typename T>
			Variant& operator=(const T& o)
			{
				return set(o);
			}

			template<typename T>
			Variant& set(const T& o)
			{
				if (id() != TypeTraits<T>::id())
				{
					Helper::destroy(m_id, &m_data);
					Helper::create(m_id, &m_data);
					setupTraitsInfo<T>();
				}
				Helper::copy(m_id, &m_data, &o);
				return *this;
			}

			template<typename T>
			operator T&() const
			{
				return get<T>();
			}

			template<typename T>
			T& get() const
			{
				return (T&)m_data;
			}

			void* pointer() const
			{
				return (void*)&m_data;
			}

			void reset()
			{
				Helper::destroy(m_id, &m_data);
				m_id = INVALID_TYPE_ID;
				m_size = 0;
				m_name = "";
			}

			u64 id() const
			{
				return m_id;
			}

			u32 size() const
			{
				return m_size;
			}

			const String& name() const
			{
				return m_name;
			}

		private:
			u64 m_id;
			u32 m_size;
			String m_name;
			RawData<Helper::size> m_data;
		};
	}
}

#include "Muon/Traits/Detail/VariantDetail.hpp"

#endif
