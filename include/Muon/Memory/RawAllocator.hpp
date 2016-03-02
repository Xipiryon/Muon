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

#ifndef INCLUDE_MUON_RAWALLOCATOR_HPP
#define INCLUDE_MUON_RAWALLOCATOR_HPP

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
		template <typename T>
		class MUON_API RawAllocator
		{
		public:
			typedef u64			size_type;
			typedef u64			difference_type;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef T			value_type;

			RawAllocator() {}
			RawAllocator(const RawAllocator&) {}

			pointer allocate(u64 n, const void * = 0)
			{
				return (T*)::malloc(n * sizeof(T));
			}

			void deallocate(void* p, u64)
			{
				if (p)
				{
					::free(p);
				}
			}

			pointer address(reference x) const
			{
				return &x;
			}

			const_pointer address(const_reference x) const
			{
				return &x;
			}

			size_type max_size() const
			{
				return size_t(-1);
			}

			void construct(pointer p, const T& val)
			{
				new ((T*) p) T(val);
			}

			void destroy(pointer p)
			{
				p->~T();
			}

			RawAllocator<T>& operator=(const RawAllocator&)
			{
				return *this;
			}

			template <class U>
			struct rebind { typedef RawAllocator<U> other; };

			template <class U>
			RawAllocator(const RawAllocator<U>&) {}

			template <class U>
			RawAllocator& operator=(const RawAllocator<U>&) { return *this; }
		};
	}
}

#endif
