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

#ifndef INCLUDE_MUON_REFLECT_VALUEMAPPER_HPP
#define INCLUDE_MUON_REFLECT_VALUEMAPPER_HPP

#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Type.hpp"
#include "Muon/Reflect/Enum.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			template<typename T, typename Check = void> struct ObjectValueMapper;
			template<typename T, typename Check = void> struct ObjectCompatibility;

			template<typename T>
			eType mapToEnum()
			{
				return static_cast<eType>(ObjectValueMapper<T>::type);
			}

#define _MUON_CONVERT_FROM(RetType, ArgType, ConvertCode) static RetType from(ArgType value) { ConvertCode ;}

#define _MUON_CONVERT_FROM_BOOL(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, bool, ConvertCode);

#define _MUON_CONVERT_FROM_INTEGER_SIGNED(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, i8, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, i16, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, i32, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, i64, ConvertCode);
#define _MUON_CONVERT_FROM_INTEGER_UNSIGNED(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, u8, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, u16, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, u32, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, u64, ConvertCode);
#define _MUON_CONVERT_FROM_INTEGER(RetType, ConvertCode) \
	_MUON_CONVERT_FROM_INTEGER_SIGNED(RetType, ConvertCode) \
	_MUON_CONVERT_FROM_INTEGER_UNSIGNED(RetType, ConvertCode)

#define _MUON_CONVERT_FROM_FLOATING(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, f32, ConvertCode); \
	_MUON_CONVERT_FROM(RetType, f64, ConvertCode);

#define _MUON_CONVERT_FROM_STRING(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, const String&, ConvertCode);

#define _MUON_CONVERT_FROM_ENUM(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, const EnumValue&, ConvertCode);

#define _MUON_CONVERT_FROM_OBJECT(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, const Object&, ConvertCode);

			// From any Type to Object
			template<typename T, typename C>
			struct ObjectValueMapper
			{
				static const i32 type = eType::OBJECT;

				static Object to(T& value)
				{
					return Object::ref(value);
				}

				_MUON_CONVERT_FROM_OBJECT(T, return *static_cast<T*>(value.object()));
			};

			// Type Mapper Specialization
			// *****************
			template<>
			struct ObjectValueMapper<bool>
			{
				static const i32 type = eType::BOOL;
				static bool to(bool value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_BOOL(bool, return value);
				_MUON_CONVERT_FROM_INTEGER(bool, return (value != 0));
				_MUON_CONVERT_FROM_FLOATING(bool, return (value != 0.));
				_MUON_CONVERT_FROM_STRING(bool, return (value == "true"));
				_MUON_CONVERT_FROM_ENUM(bool, return value.value() != 0);
				_MUON_CONVERT_FROM_OBJECT(bool, return false);
			};

			template<typename T>
			struct ObjectValueMapper<T, typename std::enable_if<std::is_integral<T>::value>::type>
			{
				static const i32 type = eType::INTEGER;
				static T to(T value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_BOOL(T, return static_cast<T>(value));
				_MUON_CONVERT_FROM_INTEGER(T, return value);
				_MUON_CONVERT_FROM_FLOATING(T, return static_cast<T>(value));
				_MUON_CONVERT_FROM_STRING(T, return ::atoi(value.cStr()));
				_MUON_CONVERT_FROM_ENUM(T, return static_cast<T>(value.value()));
			};

			template<typename T>
			struct ObjectValueMapper<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
			{
				static const i32 type = eType::FLOAT;
				static T to(T value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_BOOL(T, return static_cast<T>(value));
				_MUON_CONVERT_FROM_INTEGER(T, return static_cast<T>(value));
				_MUON_CONVERT_FROM_FLOATING(T, return value);
				_MUON_CONVERT_FROM_STRING(T, return ::atof(value.cStr()));
				_MUON_CONVERT_FROM_ENUM(T, return static_cast<T>(value.value()));
			};

			template<>
			struct ObjectValueMapper<m::String>
			{
				static const i32 type = eType::STRING;
				static const String& to(const String& value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_BOOL(String, return (value ? "true" : "false"));
				_MUON_CONVERT_FROM_INTEGER_SIGNED(String, char buffer[32]; ::m::itoa((i64)value, buffer); return buffer);
				_MUON_CONVERT_FROM_INTEGER_UNSIGNED(String, char buffer[32]; ::m::itoa((u64)value, buffer); return buffer);
				_MUON_CONVERT_FROM_FLOATING(String, char buffer[32]; ::m::ftoa(value, buffer); return buffer);
				_MUON_CONVERT_FROM_STRING(String, return value);
				_MUON_CONVERT_FROM_ENUM(String, return value.name());
			};

			template<>
			struct ObjectValueMapper<EnumValue>
			{
				static const i32 type = eType::ENUM;
				static const EnumValue& to(const EnumValue& value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_ENUM(EnumValue, return value);
			};

			template<>
			struct ObjectValueMapper<None>
			{
				static const i32 type = eType::NONE;

				static const None& to(const None& value)
				{
					return value;
				}
			};

			template<>
			struct ObjectValueMapper<void>
			{
				static const i32 type = eType::NONE;
			};

			template<typename T>
			struct ObjectValueMapper<T&> : ObjectValueMapper<T>
			{
				static T& from(T& value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_OBJECT(T&, return *static_cast<T*>(value.object()));
			};

			template<>
			struct ObjectValueMapper<const char*>
			{
				static const i32 type = eType::STRING;
				static m::String to(const char* value)
				{
					return m::String(value);
				}

				template<typename T>
				static const char* from(const T&)
				{
					MUON_ERROR("Can't convert from '%s' to temporary const char*!", traits::TypeTraits<T>::name());
					return NULL;
				}

				_MUON_CONVERT_FROM_STRING(const char*, return value.cStr());
			};

			template <i32 N>
			struct ObjectValueMapper<char[N]>
			{
				static const i32 type = eType::STRING;
				static m::String to(const char value[N])
				{
					return m::String(value);
				}
			};

			template <i32 N>
			struct ObjectValueMapper<const char[N]>
			{
				static const i32 type = eType::STRING;
				static m::String to(const char value[N])
				{
					return m::String(value);
				}
			};

			// Compatibility
			// *****************
			template<typename T>
			struct ObjectCompatibility<T>
			{
				static bool check(const Object& value)
				{
					return value.id() == traits::TypeTraits<Object>::id();
				}
			};

			template<typename T>
			struct ObjectCompatibility<T, typename std::enable_if<std::is_integral<T>::value>::type>
			{
				static bool check(const Object& value)
				{
					return value.id() == traits::TypeTraits<T>::id();
				}
			};

			template<typename T>
			struct ObjectCompatibility<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
			{
				static bool check(const Object& value)
				{
					return value.id() == traits::TypeTraits<T>::id();
				}
			};

			template<>
			struct ObjectCompatibility<const char*>
			{
				static bool check(const Object& value)
				{
					return value.id() == traits::TypeTraits<String>::id();
				}
			};

			template<>
			struct ObjectCompatibility<char*>
			{
				static bool check(const Object& value)
				{
					return value.id() == traits::TypeTraits<String>::id();
				}
			};
		}
	}
}

#undef _MUON_CONVERT_FROM
#undef _MUON_CONVERT_FROM_BOOL
#undef _MUON_CONVERT_FROM_INTEGER_SIGNED
#undef _MUON_CONVERT_FROM_INTEGER_UNSIGNED
#undef _MUON_CONVERT_FROM_INTEGER
#undef _MUON_CONVERT_FROM_FLOATING
#undef _MUON_CONVERT_FROM_STRING
#undef _MUON_CONVERT_FROM_ENUM
#undef _MUON_CONVERT_FROM_OBJECT

#endif
