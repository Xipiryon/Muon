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

#include "Muon/Memory/StackAllocator.hpp"

namespace m
{
	namespace memory
	{
		StackAllocator::StackAllocator(u32 blockSize)
			: m_blockSize(blockSize)
			, m_top(0)
		{
			m_data = ::malloc(m_blockSize);
		}

		StackAllocator::~StackAllocator()
		{
			::free(m_data);
		}

		void* StackAllocator::alloc(u32 size)
		{
			MUON_ASSERT_BREAK(m_top + size <= m_blockSize
							  , "Cannot allocate %u: %u left!"
							  , size, m_blockSize - m_top);
			void* ptr = (u8*)m_data + m_top;
			m_top += size;
		}

		void StackAllocator::free(Marker marker)
		{
			MUON_ASSERT_BREAK(marker <= m_blockSize, "Marker outside Stack boundary");
			m_top = marker;
		}

		StackAllocator::Marker StackAllocator::getMarker() const
		{
			return m_top;
		}

		void StackAllocator::clear()
		{
			m_top = 0;
		}
	}
}
