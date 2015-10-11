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

#ifndef INCLUDE_MUON_TYPETRAITS_HPP
#define INCLUDE_MUON_TYPETRAITS_HPP

#include "Muon/Core/Constant.hpp"

#define _MUON_TRAITS_NAME_ATTRIB(Type) MUON_STR(Type)

#if defined(MUON_PLATFORM_WINDOWS) || defined(MUON_PLATFORM_HTML)

namespace priv
{
	MUON_INLINE muon::u64 _meta_hash(const char* m_str)
	{
		// www.cse.yorku.ca/~oz/hash.html
		muon::u64 v = 5381;
		if (m_str)
		{
			char* str = (char*)m_str;
			int c;
			while ((c = *str++) != 0)
			{
				v = ((v << 5) + v) + c;
			}
		}
		return v;
	}
}
#	define _MUON_TRAITS_ID_ATTRIB(Type) _meta_hash(_MUON_TRAITS_NAME_ATTRIB(Type))

#else
namespace priv
{
	MUON_CONSTEXPR muon::u64 _meta_const_hash(const char* str)
	{
		return *str ? static_cast<muon::u64>(*str) + 33 * _meta_const_hash(str + 1) : 5381;
	}

	MUON_CONSTEXPR muon::u64 operator "" _meta_hash(const char* str, muon::u64)
	{
		return _meta_const_hash(str);
	}
#	define _MUON_TRAITS_ID_ATTRIB(Type) #Type ## _meta_hash
}
#endif


#define _MUON_TRAITS_NAME(Type) static MUON_INLINE MUON_CONSTEXPR const char* name() { return _MUON_TRAITS_NAME_ATTRIB(Type); }
#define _MUON_TRAITS_ID(Type) static MUON_INLINE MUON_CONSTEXPR ::muon::u64 id() {using namespace priv; return ::muon::meta::TYPE_ID_BASE_MASK & _MUON_TRAITS_ID_ATTRIB(Type); }
#define _MUON_TRAITS_SIZE(Type) static MUON_INLINE MUON_CONSTEXPR ::muon::u32 size() { return sizeof(Type); }

#define _MUON_TRAITS_FUNCTION(Type)  _MUON_TRAITS_NAME(Type) _MUON_TRAITS_ID(::Type) _MUON_TRAITS_SIZE(::Type)
#define _MUON_TRAITS_STRUCT(Type) template<> struct TypeTraits < ::Type > { _MUON_TRAITS_FUNCTION(Type) };
#define MUON_TRAITS(Type) namespace muon { namespace meta { _MUON_TRAITS_STRUCT(Type) } }

namespace muon
{
	namespace meta
	{
		enum MetaConstant : u64
		{
			TYPE_ID_INVALID 	= 0x0FFFFFFFFFFFFFFF,
			TYPE_ID_BASE_MASK	= 0x0FFFFFFFFFFFFFFF,
			TYPE_ID_CUSTOM_MASK	= 0xF000000000000000,
		};

		static const char* TYPE_NAME_UNREGISTERED = "#Unregistered#";

		// Let the compiler not compile unregistered TypeTraits
		template<typename T> struct TypeTraits {};

		template<> struct TypeTraits<void>
		{
			static MUON_INLINE const char* name()
			{
				return "void";
			}
			static MUON_INLINE ::muon::u64 id()
			{
				return MetaConstant::TYPE_ID_INVALID;
			}
			static MUON_INLINE ::muon::u32 size()
			{
				return 0;
			}
		};

		template<> struct TypeTraits<bool>
		{
			_MUON_TRAITS_NAME(bool);
			_MUON_TRAITS_ID(bool);
			_MUON_TRAITS_SIZE(bool);
		};

		template<> struct TypeTraits<const char*>
		{
			_MUON_TRAITS_NAME(const char*);
				_MUON_TRAITS_ID(const char*);
				_MUON_TRAITS_SIZE(const char*);
		};

		template<> struct TypeTraits<char*>
		{
			_MUON_TRAITS_NAME(char*);
			_MUON_TRAITS_ID(char*);
			_MUON_TRAITS_SIZE(char*);
		};

		// Template Functions to remove qualifier around a Template
		template<typename T> struct RawType
		{
			typedef T type;
		};
		template<typename T> struct RawType<const T>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<T&>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<const T&>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<T&&>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<T*>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<const T*>
		{
			typedef typename RawType<T>::type type;
		};
		template<typename T> struct RawType<const T* const>
		{
			typedef typename RawType<T>::type type;
		};
		// Special case of char*
		template<> struct RawType<const char*>
		{
			typedef const char* type;
		};
		template<> struct RawType<char*>
		{
			typedef char* type;
		};

		// Function which return a class member offset (works with virtual and abstract classes)
		template<typename Class, typename MemberType>
		MUON_CONSTEXPR u64 offset(MemberType Class::*member)
		{
			return (u64)(&(((Class*)0)->*member));
		}
	}
}

MUON_TRAITS(muon::u8)
MUON_TRAITS(muon::u16)
MUON_TRAITS(muon::u32)
MUON_TRAITS(muon::u64)

MUON_TRAITS(muon::i8)
MUON_TRAITS(muon::i16)
MUON_TRAITS(muon::i32)
MUON_TRAITS(muon::i64)

MUON_TRAITS(muon::f32)
MUON_TRAITS(muon::f64)

MUON_TRAITS(muon::RawPointer)

#endif

