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

#include "Muon/Memory/PoolAllocator.hpp"

namespace m
{
	namespace memory
	{
		union UintToPtr
		{
			void* ptr;
			u32 i;
		};

		PoolAllocator::PoolAllocator(u32 blockSize)
			: m_blockSize(blockSize)
		{
			m_data = ::malloc(m_blockSize);
			m_end = ((u8*)m_data + m_blockSize);
			u32* mem = (u32*)m_data;
			UintToPtr cast;
			while (mem < m_end)
			{
				cast.ptr = mem + 1;
				*(mem++) = cast.i;
			}
			m_free = m_data;
		}

		PoolAllocator::~PoolAllocator()
		{
			::free(m_data);
		}

		void* PoolAllocator::alloc()
		{
			MUON_ASSERT_BREAK(m_free != m_end, "Pool is full!");
			u32* ptr = (u32*)m_free;
			u32* nextFree = (u32*)m_free;
			UintToPtr cast;
			cast.i = *nextFree;
			m_free = cast.ptr;
			return ptr;
		}

		void PoolAllocator::free(u32* ptr)
		{
			u32* uptr = (u32*)ptr;
			MUON_ASSERT_BREAK(uptr >= m_data && uptr <= m_end, "Given pointer is not stored in Pool!");

			UintToPtr cast;
			cast.ptr = m_free;
			*uptr = cast.i;
			m_free = uptr;
		}
	}
}
