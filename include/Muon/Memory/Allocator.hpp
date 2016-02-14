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

#include <map>
#include "Muon/Helper/Singleton.hpp"
#include "Muon/Memory/RawAllocator.hpp"

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
		class MUON_API Allocator : public helper::Singleton<Allocator>
		{
			friend class helper::Singleton<Allocator>;
		public:

			template<typename T>
			T* allocate(u64 n, const void * = 0)
			{
				return (T*)::malloc((u32)n * sizeof(T));
			}

			template<typename T>
			void deallocate(void* p, u64)
			{
				if (p)
				{
					::free(p);
				}
			}

			template<typename T>
			void construct(T* p)
			{
				new ((T*) p) T();
			}

			template<typename T>
			void construct(T* p, const T& val)
			{
				new ((T*) p) T(val);
			}

			template<typename T>
			void destroy(T* p)
			{
				p->~T();
			}

		private:
			Allocator();
			~Allocator();

			std::map<u64, void*>* m_allocators;
		};
	}
}

/*!
* @def MUON_NEW(Class)
* Allocate memory for given Class instance
* @param Class Class or Struct to be allocated
*/
#define MUON_NEW(Class) (Class*)::malloc(sizeof(Class))

/*!
* @def MUON_CNEW(Class, ...)
* Allocate memory for given Class instance, and
* call the placement new constructor on it.
* @param Class Class or Struct to be allocated and constructed
* @param ... Variadic parameters to be forwarded to the constructor
*/
#if defined(MUON_PLATFORM_WINDOWS)
#	define MUON_CNEW(Class, ...) new (MUON_NEW(Class)) Class( __VA_ARGS__ )
#else
#	define MUON_CNEW(Class, args...) new (MUON_NEW(Class)) Class(args)
#endif //MUON_PLATFORM_WINDOWS

/*!
* @def MUON_DELETE(Pointer)
* Free an object memory
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be freed from memory
*/
#define MUON_DELETE(Pointer) ::free(Pointer)

/*!
* @def MUON_CDELETE(Pointer)
* Call the destructor on Pointer, and free the object
* The given pointer will *not* be set to NULL after deletion.
* @param Pointer Object to be destructed and freed from memory
*/
#define MUON_CDELETE(Pointer) (MUON_DELETE(Pointer), Pointer)

#endif
