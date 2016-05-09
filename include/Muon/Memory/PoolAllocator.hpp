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

#ifndef INCLUDE_MUON_POOLALLOCATOR_HPP
#define INCLUDE_MUON_POOLALLOCATOR_HPP

#include "Muon/Memory/HeapAllocator.hpp"
#include "Muon/System/Assert.hpp"

/*
* @file PoolAllocator.hpp
*/
namespace m
{
	namespace memory
	{
		/*!
		* @brief
		*
		*/
		template<typename T, u32 BlockSize>
		class MUON_API PoolAllocator
		{
		public:

			PoolAllocator()
			{
				MUON_ASSERT(sizeof(T) % BlockSize == 0, "Size of element is not a multiple of block size!");
				MUON_ASSERT_BREAK(sizeof(T) >= sizeof(u32*), "Size of element is smaller than 32-bit pointer!");

				m_data = (T*)::malloc(BlockSize);
				m_end = (T*)((u8*)m_data + BlockSize);
				T* mem = m_data;
				while (mem < m_end)
				{
					*(mem++) = (u32)(mem + 1);
				}
				m_free = m_data;
			}

			~PoolAllocator()
			{
				::free(m_data);
			}

			T* allocate()
			{
				MUON_ASSERT_BREAK(m_free != m_end, "Pool is full!");
				T* ptr = (T*)m_free;
				T* nextFree = m_free;
				m_free = (T*)*nextFree;
				return ptr;
			}

			void deallocate(T* ptr)
			{
				T* uptr = (T*)ptr;
				MUON_ASSERT_BREAK(uptr >= m_data && uptr <= m_end, "Given pointer is not stored in Pool!");

				*uptr = (u32)m_free;
				m_free = uptr;
			}

		private:
			T* m_free;
			T* m_data;
			T* m_end;
		};
	}
}

#endif
