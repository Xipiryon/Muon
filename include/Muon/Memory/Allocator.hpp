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
#include "Muon/Memory/RawAllocator.hpp"

/*
* @file Allocator.hpp
*/

/*!
* @def MUON_MALLOC(Class)
* Allocate memory for given Class instance
* @param Class Class or Struct to be allocated
*/
#define MUON_MALLOC(Class) ::m::memory::RawAllocator<typename ::m::traits::RawType<Class>::type >::allocate(1)

/*!
* @def MUON_NEW(Class, ...)
* Allocate memory for given Class instance, and
* call the placement new constructor on it.
* @param Class Class or Struct to be allocated and constructed
* @param ... Variadic parameters to be forwarded to the constructor
*/
#if defined(MUON_PLATFORM_WINDOWS)
#	define MUON_NEW(Class, ...) ::m::memory::RawAllocator<typename ::m::traits::RawType<Class>::type >::construct(1, MUON_MALLOC(Class), __VA_ARGS__ )
#else
#	define MUON_NEW(Class, args...) ::m::memory::RawAllocator<typename ::m::traits::RawType<Class>::type>::construct(1, MUON_MALLOC(Class), args)
#endif //MUON_PLATFORM_WINDOWS

/*!
* @def MUON_FREE(Pointer)
* Free an object memory
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be freed from memory
*/
#define MUON_FREE(Pointer) ::m::memory::RawAllocator<typename ::m::traits::RawType<decltype(Pointer)>::type>::deallocate(Pointer)

/*!
* @def MUON_DELETE(Pointer)
* Call the destructor on Pointer, and free the object
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be destructed and freed from memory
*/
#define MUON_DELETE(Pointer) (::m::memory::RawAllocator<typename ::m::traits::RawType<decltype(Pointer)>::type>::destroy(1, Pointer), MUON_FREE(Pointer))

#endif
