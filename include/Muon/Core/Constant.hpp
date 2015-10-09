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

#ifndef INCLUDE_MUON_CONSTANT_HPP
#define INCLUDE_MUON_CONSTANT_HPP

#include "Muon/Core/Typedef.hpp"

/*!
* @file Constant.hpp
*/

namespace muon
{
	/*!
	* @brief PI constant as float
	* As math.h doens't always include it, here is a const float declaration.
	* Its value is 3.1415927f
	*/
	static const float PI_f = 3.1415927f;

	/*!
	* @brief PI constant as double
	* As math.h doens't always include it, here is a const double declaration.
	* Its value is 3.14159265358979323846
	*/
	static const double PI_d = 3.14159265358979323846;

	//! Epsilon value (used, for exemple, in float operations)
	static const float EPSILON_f = 10e-5f;

	//! Epsilon value (used, for exemple, in double operations)
	static const double EPSILON_d = 10e-5;

#ifdef _DOXYGEN
	//! Path character separator ( '\' on Windows, '/' on Unix)
	static const char PATH_SEPARATOR;
#else
#	ifdef MUON_PLATFORM_WINDOWS
	static const char PATH_SEPARATOR = '\\';
#	else
	static const char PATH_SEPARATOR = '/';
#	endif
#endif

	//! Constant used to check against invalid index value
	static const u32 INVALID_INDEX = 0xFFFFFFFF;
}

#endif
