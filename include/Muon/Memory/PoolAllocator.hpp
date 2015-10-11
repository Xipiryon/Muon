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
#include "Muon/Memory/Allocator.hpp"

#ifndef POOLALLOCATOR_SIZE
#	define POOLALLOCATOR_SIZE 4096
#endif

static_assert(POOLALLOCATOR_SIZE != 0, "POOLALLOCATOR_SIZE cannot be defined as 0!");

/*
* @file PoolAllocator.hpp
*/
namespace muon
{
	namespace memory
	{

		namespace priv
		{

			struct MUON_API PoolMemBlock
			{
				explicit PoolMemBlock(u32 size);

				u32* data = NULL;
			};

			struct MUON_API PoolFreeBlock
			{
				PoolFreeBlock(u32* startIndex, u32 freeSize);

				u32 size = POOLALLOCATOR_SIZE;
				u32* start = NULL;
			};

			extern MUON_API std::deque<PoolMemBlock> s_poolMem;
			extern MUON_API std::map<u32, std::deque<PoolFreeBlock>> s_poolFree;
		}

		/*!
		* @brief
		*
		*/
		class MUON_API PoolAllocator : public helper::NonInstantiable
		{
		public:

			template<typename T>
			static T* allocate(u32 count)
			{
				const u32 memBlock = count*sizeof(T);
				MUON_ASSERT_BREAK(memBlock <= POOLALLOCATOR_SIZE
								  , "Cannot allocate a single element which size (%d*%d=%d) is bigger than POOLALLOCATOR_SIZE (%d)"
								  , count, sizeof(T), memBlock, POOLALLOCATOR_SIZE);
				T* mem = NULL;
				u32 poolId = 0;
				u32 freeId = 0;
				while (!mem)
				{
					// Allocate a whole new pool, create a new FreeList
					if (poolId == priv::s_poolMem.size())
					{
						priv::s_poolMem.push_back(priv::PoolMemBlock(POOLALLOCATOR_SIZE));
						priv::s_poolFree[poolId].push_back(priv::PoolFreeBlock(priv::s_poolMem.back().data, POOLALLOCATOR_SIZE));
					}

					// Search a free block big enough
					auto& freeDeque = priv::s_poolFree[poolId];
					while (!mem && freeId < freeDeque.size())
					{
						// We've got a free list in this pool
						priv::PoolFreeBlock& freeBlock = freeDeque[freeId];
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

			template<typename T, typename... Args>
			static T* construct(u32 count, T* ptr, Args... args)
			{
				for (u32 c = 0; c < count; ++c)
				{
					new (ptr + c) T(args...);
				}
				return ptr;
			}

			template<typename T>
			static void destroy(u32 count, T* ptr)
			{
				for (u32 c = 0; c < count; ++c)
				{
					(ptr + c)->~T();
				}
			}

			template<typename T>
			static void deallocate(u32 count, T* ptr)
			{
				bool assert = true;
				for (u32 poolId = 0; poolId < priv::s_poolMem.size(); ++poolId)
				{
					priv::PoolMemBlock pool = priv::s_poolMem.at(poolId);
					u32* start = pool.data;
					u32* end = pool.data + POOLALLOCATOR_SIZE;
					u32* uPtr = (u32*)ptr;
					if (uPtr >= start && uPtr <= end)
					{
						u32 memSize = sizeof(T)*count;
						if (uPtr + memSize > end)
						{
							const u32 memLeft = (u32)((u64)end - (u64)uPtr);
							MUON_UNUSED(memLeft);	// Remove warnings if MUON_ERROR_BREAK is not compiled
							MUON_ERROR_BREAK("Trying to deallocate too much memory (%u bytes, Pool size: %u bytes)", memSize, memLeft);
						}

						// Merge free blocks and stop the function (so we don't hit the MUON_ERROR)
						mergeFreeBlock(poolId, priv::PoolFreeBlock(start, memSize));
						assert = false;
						break;
					}
				}

				if (assert)
				{
					MUON_ERROR_BREAK("Object %p has not been allocated with muon::memory::PoolAllocator!", ptr);
				}
			}

		private:

			static void mergeFreeBlock(u32 poolId, priv::PoolFreeBlock freeBlock);
		};
	}
}

#endif
