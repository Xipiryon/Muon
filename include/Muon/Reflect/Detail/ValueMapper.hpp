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
#include "Muon/Traits/Variant.hpp"
#include "Muon/Reflect/Type.hpp"
#include "Muon/Reflect/Enum.hpp"
#include "Muon/Reflect/UserObject.hpp"
#include "Muon/String.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			typedef m::traits::Variant<None, bool,
				i8, i16, i32, i64,
				u8, u16, u32, u64,
				f32, f64,
				String, EnumValue, UserObject> ValueVariant;

			template<typename T, typename Check = void> struct ValueMapper;
			template<typename T, typename Check = void> struct ValueCompatibiliy;

			template<typename T>
			eType mapToEnum()
			{
				return static_cast<eType>(ValueMapper<T>::type);
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

#define _MUON_CONVERT_FROM_USEROBJECT(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, const UserObject&, ConvertCode);

#define _MUON_CONVERT_FROM_VALUEVARIANT(RetType, ConvertCode) \
	_MUON_CONVERT_FROM(RetType, const ValueVariant&, ConvertCode);

#define _MUON_CONVERT_ERROR(RetValue) \
	MUON_ERROR("Impossible to cast from '%s' to '%s'"\
			, m::reflect::eTypeStr[type] \
			, m::reflect::eTypeStr[ValueMapper<decltype(value)>::type]); \
	return RetValue;

			template<typename T, typename Check>
			struct ValueMapper
			{
				static const i32 type = eType::USER_OBJECT;
				static UserObject to(const T& value)
				{
					return UserObject(/*source*/);
				}

				_MUON_CONVERT_FROM_BOOL(T, _MUON_CONVERT_ERROR(UserObjec()));
				_MUON_CONVERT_FROM_INTEGER(T, _MUON_CONVERT_ERROR(UserObjec()));
				_MUON_CONVERT_FROM_FLOATING(T, _MUON_CONVERT_ERROR(UserObjec()));
				_MUON_CONVERT_FROM_STRING(T, _MUON_CONVERT_ERROR(UserObjec()));
				_MUON_CONVERT_FROM_ENUM(T, _MUON_CONVERT_ERROR(UserObjec()));
				_MUON_CONVERT_FROM_USEROBJECT(T, return value.getObjectConstRef<T>());
			};

			// Muon to Enum type
			// *****************
			template<>
			struct ValueMapper<bool>
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
				_MUON_CONVERT_FROM_USEROBJECT(bool, return false);
				_MUON_CONVERT_FROM_VALUEVARIANT(bool, return value.get<bool>());
			};

			template<typename T>
			struct ValueMapper<T, typename std::enable_if<std::is_integral<T>::value>::type>
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
				_MUON_CONVERT_FROM_USEROBJECT(T, _MUON_CONVERT_ERROR(0));
				_MUON_CONVERT_FROM_VALUEVARIANT(T, return value.get<T>());
			};

			template<typename T>
			struct ValueMapper<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
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
				_MUON_CONVERT_FROM_USEROBJECT(T, _MUON_CONVERT_ERROR(0.));
				_MUON_CONVERT_FROM_VALUEVARIANT(T, return value.get<T>());
			};

			template<>
			struct ValueMapper<m::String>
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
				_MUON_CONVERT_FROM_USEROBJECT(String, _MUON_CONVERT_ERROR(""));
				_MUON_CONVERT_FROM_VALUEVARIANT(String, return value.get<String>());
			};

			template<>
			struct ValueMapper<EnumValue>
			{
				static const i32 type = eType::ENUM;
				static const EnumValue& to(const EnumValue& value)
				{
					return value;
				}

				_MUON_CONVERT_FROM_BOOL(EnumValue, _MUON_CONVERT_ERROR(EnumValue()));
				_MUON_CONVERT_FROM_INTEGER(EnumValue, _MUON_CONVERT_ERROR(EnumValue()));
				_MUON_CONVERT_FROM_FLOATING(EnumValue, _MUON_CONVERT_ERROR(EnumValue()));
				_MUON_CONVERT_FROM_STRING(EnumValue, _MUON_CONVERT_ERROR(EnumValue()));
				_MUON_CONVERT_FROM_ENUM(EnumValue, return value);
				_MUON_CONVERT_FROM_USEROBJECT(EnumValue, _MUON_CONVERT_ERROR(EnumValue()));
				_MUON_CONVERT_FROM_VALUEVARIANT(EnumValue, return value.get<EnumValue>());
			};

			template<>
			struct ValueMapper<void>
			{
				static const i32 type = eType::NONE;
			};

			template<>
			struct ValueMapper<const char*>
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
				_MUON_CONVERT_FROM_VALUEVARIANT(const char*, return value.get<String>().cStr());
			};

			template<typename T>
			struct ValueMapper<const T> : ValueMapper<T>
			{
			};

			template<typename T>
			struct ValueMapper<const T&> : ValueMapper<T>
			{
			};

			// Enum to Muon type
			// *****************

			// Compatibility
			// *****************
			template<typename T, typename = void>
			struct ValueCompatibility
			{
				static bool compatible(const ValueVariant& value)
				{
					return value.id() == traits::TypeTraits<UserObject>::id();
				}
			};

			template<typename T>
			struct ValueCompatibility<T, typename std::enable_if<std::is_integral<T>::value>::type>
			{
				static bool compatible(const ValueVariant& value)
				{
					return value.id() == traits::TypeTraits<T>::id();
				}
			};

			template<typename T>
			struct ValueCompatibility<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
			{
				static bool compatible(const ValueVariant& value)
				{
					return value.id() == traits::TypeTraits<T>::id();
				}
			};

			template<>
			struct ValueCompatibility<bool>
			{
				static bool compatible(const ValueVariant& value)
				{
					return value.id() == traits::TypeTraits<bool>::id();
				}
			};

			template<typename T>
			struct ValueCompatibility<const T> : ValueCompatibility<T>
			{
			};

			template<typename T>
			struct ValueCompatibility<const T&> : ValueCompatibility<T>
			{
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
#undef _MUON_CONVERT_FROM_USEROBJECT
#undef _MUON_CONVERT_FROM_VALUEVARIANT
#undef _MUON_CONVERT_ERROR

#endif
