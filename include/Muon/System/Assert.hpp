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

#ifndef INCLUDE_MUON_ASSERT_HPP
#define INCLUDE_MUON_ASSERT_HPP

#include "Muon/Core/Constant.hpp"
#include "Muon/Core/Typedef.hpp"

namespace muon
{
	namespace system
	{
		MUON_API void assertMessage(const char* file, const char* func, u32 line, const char* format, ...);
	}
}
//		--------------------------
//				ASSERTIONS
//		--------------------------

#if defined(MUON_PLATFORM_HTML)

#	define MUON_ASSERT(cond, format, ...)
#	define MUON_ASSERT_BREAK(cond, format, ...)
#	define MUON_ERROR(format, ...)
#	define MUON_ERROR_BREAK(format, ...)

#elif defined(MUON_PLATFORM_WINDOWS)

#	define _MUON_ASSERT(format, ...) muon::system::assertMessage(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
#	define MUON_ASSERT(cond, format, ...) do { if ((cond) == false) { _MUON_ASSERT(format, __VA_ARGS__); } } while(0)
#	define MUON_ASSERT_BREAK(cond, format, ...) do { if ((cond) == false) { _MUON_ASSERT(format, __VA_ARGS__); MUON_ASM_BREAK;} } while(0)
#	define MUON_ERROR(format, ...) do { _MUON_ASSERT(format, __VA_ARGS__); } while(0)
#	define MUON_ERROR_BREAK(format, ...) do { _MUON_ASSERT(format, __VA_ARGS__); MUON_ASM_BREAK; } while(0)

#else

#	define _MUON_ASSERT(format, args...) muon::system::assertMessage(__FILE__, __FUNCTION__, __LINE__, format, ##args)
#	define MUON_ASSERT(cond, format, args...) do { if ((cond) == false) { _MUON_ASSERT(format, ##args); } } while(0)
#	define MUON_ASSERT_BREAK(cond, format, args...) do { if ((cond) == false) { _MUON_ASSERT(format, ##args); MUON_ASM_BREAK;} } while(0)
#	define MUON_ERROR(format, args...) do { _MUON_ASSERT(format, ##args); } while(0)
#	define MUON_ERROR_BREAK(format, args...) do { _MUON_ASSERT(format, ##args); MUON_ASM_BREAK; } while(0)

#endif //MUON_PLATFORM_WINDOWS


#endif	//INCLUDE_MUON_ASSERT_HPP
