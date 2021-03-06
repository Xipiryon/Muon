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

#ifndef INCLUDE_MUON_RAWTYPE_HPP
#define INCLUDE_MUON_RAWTYPE_HPP

#include "Muon/Traits/TypeInfo.hpp"

namespace m
{
	namespace traits
	{
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
	}
}

#endif
