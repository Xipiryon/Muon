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

#include <map>
#include <deque>
#include "Muon/Memory/RawAllocator.hpp"
#include "Muon/System/Assert.hpp"

#ifndef POOLALLOCATOR_SIZE
#	define POOLALLOCATOR_SIZE 4096
#endif

static_assert(POOLALLOCATOR_SIZE != 0, "POOLALLOCATOR_SIZE cannot be defined as 0!");

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
		template<typename T>
		class PoolAllocator
		{
		public:
			struct PoolMemBlock
			{
				explicit PoolMemBlock(u32 size)
					: data((u32*)::malloc(size))
				{
				}

				u32* data = NULL;
			};

			struct PoolFreeBlock
			{
				PoolFreeBlock(u32* startIndex, u32 freeSize)
					: size(freeSize)
					, start(startIndex)
				{
				}

				u32 size = POOLALLOCATOR_SIZE;
				u32* start = NULL;
			};

			static T* allocate(u32 n)
			{
				const u32 memBlock = n*sizeof(T);
				MUON_ASSERT_BREAK(memBlock <= POOLALLOCATOR_SIZE
								  , "Cannot allocate a single element which size (%d*%d=%d) is bigger than POOLALLOCATOR_SIZE (%d)"
								  , n, sizeof(T), memBlock, POOLALLOCATOR_SIZE);
				T* mem = NULL;
				u32 poolId = 0;
				u32 freeId = 0;
				while (!mem)
				{
					// Allocate a whole new pool, create a new FreeList
					if (poolId == s_poolMem.size())
					{
						s_poolMem.push_back(PoolMemBlock(POOLALLOCATOR_SIZE));
						s_poolFree[poolId].push_back(PoolFreeBlock(s_poolMem.back().data, POOLALLOCATOR_SIZE));
					}

					// Search a free block big enough
					auto& freeDeque = s_poolFree[poolId];
					while (!mem && freeId < freeDeque.size())
					{
						// We've got a free list in this pool
						PoolFreeBlock& freeBlock = freeDeque[freeId];
						if (memBlock <= freeBlock.size)
						{
							mem = (T*)freeBlock.start;
							// Update the free block
							if (freeBlock.size != memBlock)
							{
								freeBlock.size -= memBlock;
								freeBlock.start += memBlock;
							}
							// Just remove it
							else
							{
								freeDeque.erase(freeDeque.begin() + freeId);
							}
						}
						++freeId;
					}
					++poolId;
				}
				return mem;
			}

			template<typename... Args>
			static T* construct(u32 n, T* ptr, Args... args)
			{
				for (u32 c = 0; c < n; ++c)
				{
					new (ptr + c) T(args...);
				}
				return ptr;
			}

			static void destroy(u32 n, T* ptr)
			{
				for (u32 c = 0; c < n; ++c)
				{
					(ptr + c)->~T();
				}
			}

			static void deallocate(u32 n, T* ptr)
			{
				bool assert = true;
				for (u32 poolId = 0; poolId < s_poolMem.size(); ++poolId)
				{
					PoolMemBlock pool = s_poolMem.at(poolId);
					u32* start = pool.data;
					u32* end = pool.data + POOLALLOCATOR_SIZE;
					u32* uPtr = (u32*)ptr;
					if (uPtr >= start && uPtr <= end)
					{
						u32 memSize = sizeof(T)*n;
						if (uPtr + memSize > end)
						{
							const u32 memLeft = (u32)((u64)end - (u64)uPtr);
							MUON_UNUSED(memLeft);	// Remove warnings if MUON_ERROR_BREAK is not compiled
							MUON_ERROR_BREAK("Trying to deallocate too much memory (%u bytes, Pool size: %u bytes)", memSize, memLeft);
						}

						// Merge free blocks and stop the function (so we don't hit the MUON_ERROR)
						mergeFreeBlock(poolId, PoolFreeBlock(start, memSize));
						assert = false;
						break;
					}
				}

				if (assert)
				{
					MUON_ERROR_BREAK("Object %p has not been allocated with m::memory::PoolAllocator!", ptr);
				}
			}

		private:
			static void mergeFreeBlock(u32 poolId, PoolFreeBlock block)
			{
				auto itDeque = s_poolFree.find(poolId);
				if (itDeque == s_poolFree.end())
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

				if (add)
				{
					freeDeque.push_back(block);
				}
			}

			static std::deque<PoolMemBlock> s_poolMem;
			static std::map<u32, std::deque<PoolFreeBlock>> s_poolFree;
		};
		template<typename T> std::deque<typename PoolAllocator<T>::PoolMemBlock> PoolAllocator<T>::s_poolMem;
		template<typename T> std::map<u32, std::deque<typename PoolAllocator<T>::PoolFreeBlock>> PoolAllocator<T>::s_poolFree;
	}
}

#endif
