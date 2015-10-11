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

namespace muon
{
	namespace memory
	{
		namespace priv
		{
			PoolMemBlock::PoolMemBlock(u32 size)
				: data((u32*)::malloc(size))
			{
			}

			PoolFreeBlock::PoolFreeBlock(u32* startIndex, u32 freeSize)
				: size(freeSize)
				, start(startIndex)
			{
			}


			std::deque<PoolMemBlock> s_poolMem;
			std::map<u32, std::deque<PoolFreeBlock>> s_poolFree;
		}

		void PoolAllocator::mergeFreeBlock(u32 poolId, priv::PoolFreeBlock block)
		{
			auto itDeque = priv::s_poolFree.find(poolId);
			if (itDeque == priv::s_poolFree.end())
			{
				MUON_ERROR_BREAK("Trying to free memory from a pool that doesn't exist!");
			}
			auto& freeDeque = itDeque->second;

			// If we either find a free block that begin where we end, or end where we begin, update it
			bool add = true;
			for (auto& it : freeDeque)
			{
				if ((it.start + it.size) == block.start)
				{
					it.size += block.size;
					add = false;
					break;
				}

				if (it.start == (block.start + block.size))
				{
					it.start = block.start;
					it.size += block.size;
					add = false;
					break;
				}
			}

			if(add)
			{
				freeDeque.push_back(block);
			}
		}
	}
}
