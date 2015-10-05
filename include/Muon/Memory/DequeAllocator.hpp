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

#ifndef _MUON_DEQUEALLOCATOR_H_INCLUDED
#define _MUON_DEQUEALLOCATOR_H_INCLUDED

#include <deque>
#include "Muon/Memory/Allocator.hpp"

/*
* @file DequeAllocator.hpp
*/
namespace muon
{
	namespace memory
	{
		/*!
		* @brief
		*
		*/
		class MUON_API DequeAllocator : public modifier::NonInstantiable
		{
		public:
			template<typename T>
			static T* allocate(u32 count, const AllocatorInfo& info = {})
			{
				if(_free->empty())
				{
					_allocated->push_back(::malloc(sizeof(T) * count));
				}
				else
				{
					_allocated->at(_free->back()) = ::malloc(sizeof(T) * count);
					_free->pop_back();
				}
				return (T*)_allocated->back();
			}

			template<typename T>
			static void destroy(T* ptr, const AllocatorInfo& info = {})
			{
				u32 free = 0;
				for(auto it = _allocated->begin(); it != _allocated->end(); ++it)
				{
					RawPointer alloc = *it;
					if (alloc == ptr)
					{
						ptr->~T();
					}
					++free;
				}
			}

			template<typename T>
			static void deallocate(u32 count, T* ptr, const AllocatorInfo& info = {})
			{
				u32 free = 0;
				for (auto it = _allocated->begin(); it != _allocated->end(); ++it)
				{
					RawPointer alloc = *it;
					if (alloc == ptr)
					{
						::free((RawPointer)ptr);
						_free->push_back(free);
					}
					++free;
				}
			}

		private:
			static std::deque<RawPointer>* _allocated;
			static std::deque<u32>* _free;
		};
	}
}

#endif
