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
		namespace details
		{
			template<typename T, typename...Ts>
			struct VariantHelper
			{
				static const u32 size = sizeof(T) > VariantHelper<Ts...>::size ? sizeof(T) : VariantHelper<Ts...>::size;

				inline static void create(u64 id, void* data)
				{
					if (id == TypeTraits<T>::id())
					{
						new (data)T();
					}
					else
					{
						VariantHelper<Ts...>::create(id, data);
					}
				}

				inline static void copy(u64 id, void* dst, const void* src)
				{
					if (id == TypeTraits<T>::id())
					{
						*reinterpret_cast<T*>(dst) = *reinterpret_cast<const T*>(src);
					}
					else
					{
						VariantHelper<Ts...>::copy(id, dst, src);
					}
				}

				inline static void destroy(u64 id, void* data)
				{
					if (id == TypeTraits<T>::id())
					{
						reinterpret_cast<T*>(data)->~T();
					}
					else
					{
						VariantHelper<Ts...>::destroy(id, data);
					}
				}
			};

			template<typename T>
			struct VariantHelper<T>
			{
				static const u32 size = sizeof(T);

				inline static void create(u64 id, void* data)
				{
					if (id == TypeTraits<T>::id())
					{
						new (data)T();
					}
				}

				inline static void copy(u64 id, void* dst, const void* src)
				{
					if (id == TypeTraits<T>::id())
					{
						*reinterpret_cast<T*>(dst) = *reinterpret_cast<const T*>(src);
					}
				}

				inline static void destroy(u64 id, void* data)
				{
					if (id == TypeTraits<T>::id())
					{
						reinterpret_cast<T*>(data)->~T();
					}
				}
			};
		}

		template<typename...Ts>
		class MUON_API Variant
		{
			typedef details::VariantHelper<Ts...> Helper;

			template<u32 Size>
			struct RawData
			{
				u8 data[Size];
			};

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
			Variant& operator=(const T& rhs)
			{
				return set<T>(rhs);
			}

			template<typename T, typename...Args>
			Variant& set(Args&&...args)
			{
				Helper::destroy(m_id, &m_data);
				new (&m_data)T(std::forward<Args>(args)...);
				m_id = TypeTraits<T>::id();
				m_size = TypeTraits<T>::size();
				m_name = TypeTraits<T>::name();
				return *this;
			}

			template<typename T>
			T& get() const
			{
				return (T&)m_data;
			}

			void* getRaw() const
			{
				return (void*)&m_data;
			}

			void reset()
			{
				Helper::destroy(m_id, &m_data);
				m_id = INVALID_TYPE_ID;
				m_size = 0;
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

#endif
