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

#ifndef INCLUDE_MUON_HEAPALLOCATOR_HPP
#define INCLUDE_MUON_HEAPALLOCATOR_HPP

#include <new>
#include <stdlib.h>
#include "Muon/Core/Typedef.hpp"

/*
* @file Allocator.hpp
*/
namespace m
{
	namespace memory
	{
		/*!
		* @brief
		*
		*/
		class HeapAllocator
		{
		public:

			static void* allocate(u32 size)
			{
				return ::malloc(size);
			}

			template<typename T, typename... Args>
			static T* construct(T* p, Args&&... args)
			{
				new (p)T(args...);
				return p;
			}

			template <typename T>
			static void destroy(T* p)
			{
				p->~T();
			}

			static void deallocate(void* p)
			{
				::free(p);
			}
		};
	}
}

#endif
