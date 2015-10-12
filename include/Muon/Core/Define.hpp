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
*	claim that you wrote the original software. If you use this software
*	in a product, an acknowledgment in the product documentation would
*	be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*	be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*	distribution.
*
*************************************************************************/

#ifndef INCLUDE_MUON_DEFINE_HPP
#define INCLUDE_MUON_DEFINE_HPP

/*!
* @file Define.hpp
*/

//		--------------------------
//				NAMESPACE
//		--------------------------

enum { s_namespaceMuon = false };
//! Library main namespace
namespace muon { enum { s_namespaceMuon = true }; }

//		--------------------------
//				EXPORT
//		--------------------------
//Windows
#	ifdef _MSC_VER
#		if MUON_EXPORTS
#			define MUON_API __declspec(dllexport)
#		else
#			define MUON_API __declspec(dllimport)
#		endif
#	else
//Unix
#		if MUON_EXPORTS
#			define MUON_API __attribute__ ((visibility("default")))
#		else
#			define MUON_API
#		endif
#	endif

//		-------------------------
//				PLATFORMS
//		-------------------------

#if defined(_DOXYGEN)
//! Is defined if target platform is a Windows OS
#	define MUON_PLATFORM_WINDOWS
//! Is defined if target platform is Web Browser. Currently only Emscripten is supported
#	define MUON_PLATFORM_HTML
//! Is defined if target platform is a Linux OS
#	define MUON_PLATFORM_LINUX
//! Is defined if target platform is an Apple OS (Desktop / Mobile)
#	define MUON_PLATFORM_APPLE
#endif

#if	defined(_WIN32) || defined(_WIN64)
//	Is defined on Windows platforms
#	define MUON_PLATFORM_WINDOWS
#elif defined(EMSCRIPTEN)
//	Emscripten is using unix platform target as well, so define it
//	before so we don't have any conflict
#	define MUON_PLATFORM_HTML
#	include <emscripten.h>
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__linux__)
//	Is defined on Linux platforms
#	define MUON_PLATFORM_LINUX
#elif defined(__APPLE) || defined(__APPLE__) || defined(__MACH__)
//	Is defined on Apple platforms
#	define MUON_PLATFORM_APPLE
#else
//	If compiled on an unknow platform
#	error("Unknown platform!")
#endif
//		-------------------------
//				UTILS MACROS
//		-------------------------

#ifdef _DOXYGEN
//! This macro only defines 'inline' when supported by the target platform
#	define MUON_INLINE
//! This macro only defines 'constexpr' when supported by the target platform
#	define MUON_CONSTEXPR
//! Remove warning about unused variable
#	define MUON_UNUSED(var)
//! Stringify the 'exp' parameter
#	define MUON_STR(exp)
#endif

// Remove inline when compiling with MUON_PLATFORM_HTML
#	define MUON_INLINE inline

// Remove constexpr from Windows, as it doesn't handle it very well (yet)
#if defined(_MSC_VER)
#	define MUON_CONSTEXPR
// Emscripten doesn't seem to support it either
#elif defined(MUON_PLATFORM_HTML)
#	define MUON_CONSTEXPR
#else
#	define MUON_CONSTEXPR constexpr
#endif

// Remove warning about unused variable in functions
#define MUON_UNUSED(exp) (void)(exp)

#define _MUON_STR(T) #T
#define _MUON_GLUE(L, R) L ## R

// Stringify token
#define MUON_STR(T) _MUON_STR(T)
// Append second argument to first (tokenize both)
#define MUON_GLUE(L, R) _MUON_GLUE(L, R)
// Append the line number to the first argument
#define MUON_GLUE_LINE(L) MUON_GLUE(L, __LINE__)
// Append the compiler count value to the first argument
#define MUON_GLUE_COUNTER(L) MUON_GLUE(L, __COUNTER__)

//		--------------------------
//				ASSEMBLY
//		-------------------------
#ifdef _DOXYGEN
//! Execute the 'asm' instruction
#	define MUON_ASM(asm)
//! Make the compiler break with an assembler interruption code
#	define MUON_ASM_BREAK
#endif

#ifdef _MSC_VER
#	define MUON_ASM(asm) __asm{asm}
#	define MUON_ASM_BREAK MUON_ASM(int 3)
#elif defined(MUON_PLATFORM_HTML)
#	define MUON_ASM(asm)
#	define MUON_ASM_BREAK
#else
#	define MUON_ASM(asm) __asm__(#asm)
#	define MUON_ASM_BREAK MUON_ASM(int $3)
#endif

//		--------------------------
//				COMPILER
//		--------------------------

// Avoid Visual Studio to panic about some unsafe functions
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning(disable: 4996)
#endif

#endif //INCLUDE_MUON_DEFINE_HPP
