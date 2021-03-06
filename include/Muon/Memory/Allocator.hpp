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

#ifndef INCLUDE_MUON_ALLOCATOR_HPP
#define INCLUDE_MUON_ALLOCATOR_HPP

#include "Muon/Traits/RawType.hpp"
#include "Muon/Memory/HeapAllocator.hpp"

/*
* @file Allocator.hpp
*/

namespace m
{
	namespace memory
	{
		template<typename T, typename... Args>
		static T* construct(T* p, Args&&... args)
		{
			return new (p)T(args...);
		}

		template<typename T>
		static void destroy(T* p)
		{
			p->~T();
		}
	}
}
/*!
* @def MUON_MALLOC(Class)
* Allocate memory for given Class instance
* @param Class Type to be allocated
* @param Count Number of element
*/
#define MUON_MALLOC(Class, Count) (Class*)::m::memory::HeapAllocator::alloc(sizeof(Class) * Count)

/*!
* @def MUON_NEW(Class, ...)
* Allocate memory for given Class instance, and
* call the placement new constructor on it.
* @param Class Class or Struct to be allocated and constructed
* @param ... Variadic parameters to be forwarded to the constructor
*/
#if defined(MUON_PLATFORM_WINDOWS)
#	define MUON_NEW(Class, ...) ::m::memory::construct<typename ::m::traits::RawType<Class>::type>(MUON_MALLOC(Class, 1), __VA_ARGS__ )
#else
#	define MUON_NEW(Class, args...) ::m::memory::construct<typename ::m::traits::RawType<Class>::type>(MUON_MALLOC(Class, 1), ##args)
#endif //MUON_PLATFORM_WINDOWS

/*!
* @def MUON_FREE(Pointer)
* Free an object memory
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be freed from memory
*/
#define MUON_FREE(Pointer) ::m::memory::HeapAllocator::free(Pointer)

/*!
* @def MUON_DELETE(Pointer)
* Call the destructor on Pointer, and free the object
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be destructed and freed from memory
*/
#define MUON_DELETE(Pointer) (::m::memory::destroy<typename ::m::traits::RawType<decltype(Pointer)>::type>(Pointer), MUON_FREE(Pointer))

#endif
