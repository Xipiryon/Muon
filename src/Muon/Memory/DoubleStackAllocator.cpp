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

#include "Muon/Memory/DoubleStackAllocator.hpp"

namespace m
{
	namespace memory
	{
		DoubleStackAllocator::DoubleStackAllocator(u32 blockSize)
			: m_blockSize(blockSize)
		{
			m_data = ::malloc(m_blockSize);
			m_top[0] = 0;
			m_top[1] = m_blockSize;
		}

		DoubleStackAllocator::~DoubleStackAllocator()
		{
			::free(m_data);
		}

		DoubleStackAllocator::DoubleStackAllocator(const DoubleStackAllocator& o)
			: DoubleStackAllocator(o.m_blockSize)
		{
		}

		void* DoubleStackAllocator::allocLeft(u32 size)
		{
			MUON_ASSERT_BREAK(m_top[0] + size <= m_blockSize
							  , "Cannot allocate %u: %u left!"
							  , size, m_blockSize - m_top[0]);

			MUON_ASSERT_BREAK(m_top[0] + size < m_top[0]
							  , "Cannot allocate %u: %u left!"
							  , size, m_top[1] - (m_top[0] + size));

			void* ptr = (u8*)m_data + m_top[0];
			m_top[0] += size;
			return ptr;
		}

		void DoubleStackAllocator::freeLeft(Marker marker)
		{
			MUON_ASSERT_BREAK(marker <= m_blockSize, "Marker outside DoubleStack boundary");
			MUON_ASSERT_BREAK(marker <= m_top[0], "Trying to free more memory than allocated!");
			m_top[0] = marker;
		}

		DoubleStackAllocator::Marker DoubleStackAllocator::getMarkerLeft() const
		{
			return m_top[0];
		}

		void DoubleStackAllocator::clearLeft()
		{
			m_top[0] = 0;
		}

		void* DoubleStackAllocator::allocRight(u32 size)
		{
			MUON_ASSERT_BREAK(m_top[1] + size <= m_blockSize
							  , "Cannot allocate %u: %u left!"
							  , size, m_blockSize - m_top[1]);

			MUON_ASSERT_BREAK(m_top[1] - size > m_top[0]
							  , "Cannot allocate %u: %u left!"
							  , size, m_top[0] - (m_top[1] + size));

			m_top[1] -= size;
			void* ptr = (u8*)m_data + m_top[1];
			return ptr;
		}

		void DoubleStackAllocator::freeRight(Marker marker)
		{
			MUON_ASSERT_BREAK(marker <= m_blockSize, "Marker outside DoubleStack boundary");
			MUON_ASSERT_BREAK(marker >= m_top[1], "Trying to free more memory than allocated!");
			m_top[1] = marker;
		}

		DoubleStackAllocator::Marker DoubleStackAllocator::getMarkerRight() const
		{
			return m_top[1];
		}

		void DoubleStackAllocator::clearRight()
		{
			m_top[1] = m_blockSize;
		}
	}
}
