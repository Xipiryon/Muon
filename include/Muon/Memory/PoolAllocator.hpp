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

#ifndef _MUON_POOLALLOCATOR_H_INCLUDED
#define _MUON_POOLALLOCATOR_H_INCLUDED

#include "Muon/Core/Typedef.hpp"
#include "Muon/System/Assert.hpp"

namespace muon
{
	namespace memory
	{
		namespace pool
		{
			MUON_API void* allocate(const char* file, const char* func, u32 line, u32 size, u32 count);
			MUON_API void deallocate(const char* file, const char* func, u32 line, u32 size, u32 count, void* ptr);
			MUON_API void info();
		}
	}
}

/*!
* @def MUON_NEW(Class)
* Allocate memory for given Class instance
* @param Class Class or Struct to be allocated
*/
#define MUON_NEW(Class) ((Class*)muon::memory::pool::allocate(__FILE__, __FUNCTION__, __LINE__, sizeof(Class), 1))

/*!
* @def MUON_CNEW(Class)
* Allocate memory for given Class instance, and
* call the placement new constructor on it.
* @param Class Class or Classure to be allocated and constructed
*/
#define MUON_CNEW(Class, ...) new MUON_NEW(Class) Class(__VA_ARGS__)

/*!
* @def MUON_DELETE(Pointer)
* Free an object memory
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be freed from memory
*/
#define MUON_DELETE(Pointer) muon::memory::pool::deallocate(__FILE__, __FUNCTION__, __LINE__, sizeof(*Pointer), 1, (void*)Pointer)

/*!
* @def MUON_CDELETE(Pointer, Class)
* Call the destructor Class on Pointer, and free the object
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be destructed and freed from memory
*/
#define MUON_CDELETE(Pointer, Class) Pointer->~Class(); MUON_DELETE(Pointer)

#endif
