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

#ifndef INCLUDE_MUON_NONINSTANTIABLE_HPP
#define INCLUDE_MUON_NONINSTANTIABLE_HPP

#include "Muon/Core/Typedef.hpp"

/*
* @file NonInstantiable.hpp
*/
namespace muon
{
	namespace helper
	{
		/*!
		* @brief Disallow instantiation of a class
		*
		* When a class inherits from NonInstantiable, it will not
		* be possible to instantiate this class.
		* (Unless by specifying constructor, etc... with the public keyword,
		*  which in a way defy the purpose of this class.)
		*
		* @note
		* Destructor is made protected, while 
		* Constructor, Copy and Assignment operator are made private.
		*/
		class MUON_API NonInstantiable
		{
		protected:
			~NonInstantiable() {}

		private:
			NonInstantiable();
			NonInstantiable(const NonInstantiable&);
			NonInstantiable& operator=(const NonInstantiable&);
		};
	}
}

#endif
