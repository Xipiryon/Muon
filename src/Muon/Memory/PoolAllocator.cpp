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
		PoolAllocator::PoolAllocator(u32 elementSize, u32 blockSize)
			: m_elementSize(elementSize)
			, m_blockSize(blockSize)
		{
			MUON_ASSERT_BREAK(m_blockSize % m_elementSize == 0
							  , "PoolAllocator block (%u) is not a mutliple of element size (%u)"
							  , m_blockSize, m_elementSize);
			MUON_ASSERT_BREAK(elementSize >= sizeof(u32)
							  , "Element size (%u) is lower than m::u32 size (%u)!"
							  , elementSize, sizeof(u32));
			m_data = ::malloc(m_blockSize);
			m_end = ((u8*)m_data + m_blockSize);
			u8* mem = (u8*)m_data;
			UintToPtr cast;
			while (mem < m_end)
			{
				cast.pointer = mem + m_elementSize;
				*(mem++) = cast.value;
			}
			m_free = m_data;
		}

		PoolAllocator::~PoolAllocator()
		{
			::free(m_data);
		}

		PoolAllocator::PoolAllocator(const PoolAllocator& o)
			: PoolAllocator(o.m_elementSize, o.m_blockSize)
		{
		}

		void* PoolAllocator::alloc()
		{
			MUON_ASSERT_BREAK(m_free != m_end, "Pool is full!");
			u32* ptr = (u32*)m_free;
			u32* nextFree = (u32*)m_free;
			UintToPtr cast;
			cast.value = *nextFree;
			m_free = cast.pointer;
			return ptr;
		}

		void PoolAllocator::free(u32* ptr)
		{
			u32* uptr = (u32*)ptr;
			MUON_ASSERT_BREAK(uptr >= m_data && uptr <= m_end, "Given pointer is not stored in Pool!");

			UintToPtr cast;
			cast.pointer = m_free;
			*uptr = cast.value;
			m_free = uptr;
		}
	}
}
