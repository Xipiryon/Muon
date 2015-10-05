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

#ifndef _MUON_ALLOCATOR_H_INCLUDED
#define _MUON_ALLOCATOR_H_INCLUDED

#include <stdlib.h>
#include "Muon/Core/Typedef.hpp"
#include "Muon/System/Assert.hpp"
#include "Muon/Modifier/NonInstantiable.hpp"

/*
* @file Allocator.hpp
*/
namespace muon
{
	namespace memory
	{
		/*!
		* @brief
		*
		*/
		struct MUON_API AllocatorInfo
		{
			const char* file;
			const char* function;
			u32 line;
		};
#define _MUON_ALLOCINFO {__FILE__, __FUNCTION__, __LINE__}

		/*!
		* @brief
		*
		*/
		class DefaultAllocator : public modifier::NonInstantiable
		{
		public:
			template<typename T>
			static T* allocate(u32 count, const AllocatorInfo& info)
			{
				return (T*)::malloc(sizeof(T) * count);
			}

			template<typename T>
			static void destruct(T* ptr, const AllocatorInfo& info)
			{
				ptr->~T();
			}

			template<typename T>
			static void deallocate(u32 count, T* ptr, const AllocatorInfo& info)
			{
				::free((void*)ptr);
			}
		};
	}
}

/*!
* @def MUON_NEW(Class)
* Allocate memory for given Class instance
* @param Class Class or Struct to be allocated
*/
#define MUON_NEW(Class) (::muon::memory::DefaultAllocator::allocate<Class>(1, _MUON_ALLOCINFO))

/*!
* @def MUON_CNEW(Class, ...)
* Allocate memory for given Class instance, and
* call the placement new constructor on it.
* @param Class Class or Struct to be allocated and constructed
* @param ... Variadic parameters to be forwarded to the constructor
*/
#define MUON_CNEW(Class, ...) new MUON_NEW(Class) Class(__VA_ARGS__)

/*!
* @def MUON_DELETE(Pointer)
* Free an object memory
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be freed from memory
*/
#define MUON_DELETE(Pointer) ::muon::memory::DefaultAllocator::deallocate(1, Pointer, _MUON_ALLOCINFO)

/*!
* @def MUON_CDELETE(Pointer)
* Call the destructor on Pointer, and free the object
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be destructed and freed from memory
*/
#define MUON_CDELETE(Pointer) ::muon::memory::DefaultAllocator::destruct(Pointer, _MUON_ALLOCINFO); MUON_DELETE(Pointer)

#endif
