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

#ifndef INCLUDE_MUON_TYPEINFO_HPP
#define INCLUDE_MUON_TYPEINFO_HPP

#include "Muon/Core/Constant.hpp"

namespace muon
{
	namespace traits
	{
		enum TypeInfoShift
		{
			USE_POINTER_SHIFT = 0,
			USE_REFERENCE_SHIFT,

			IS_CUSTOM_SHIFT,
		};

		enum TypeInfoFlag
		{
			USE_POINTER_FLAG = 1 << USE_POINTER_SHIFT,
			USE_REFERENCE_FLAG = 1 << USE_REFERENCE_SHIFT,

			IS_CUSTOM_FLAG = 1 << IS_CUSTOM_SHIFT,
		};

		template<typename T> struct UsePointer
		{
			static const bool value = false;
			static const u32 flag = 0;
		};

		template<typename T> struct UseReference
		{
			static const bool value = false;
			static const u32 flag = 0;
		};
	}
}

#define _MUON_META_SETNAMESPACE(Decl) namespace muon { namespace traits { template<> struct Decl } }
#define _MUON_META_SETATTRIB(Flag) static const bool value = true; static const u32 flag = Flag;
#define _MUON_META_SETINFO(Class, Struct, Flag) _MUON_META_SETNAMESPACE(Struct<Class> { _MUON_META_SETATTRIB(Flag) };)
#define MUON_META_USEPOINTER(Class) _MUON_META_SETINFO(Class, UsePointer, USE_POINTER_FLAG)
#define MUON_META_USEREFERENCE(Class) _MUON_META_SETINFO(Class, UseReference, USE_REFERENCE_FLAG)

#endif

