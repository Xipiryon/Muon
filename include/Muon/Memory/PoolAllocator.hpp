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

#ifndef _MUON_POOLALLOCATOR_H_INCLUDED
#define _MUON_POOLALLOCATOR_H_INCLUDED

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
		/*!
		* @brief
		*
		*/
		class MUON_API PoolAllocator : public helper::NonInstantiable
		{
		public:

			template<typename T>
			static T* allocate(u32 count, const AllocatorInfo& info = {})
			{
				const u32 memBlock = count*sizeof(T);
				MUON_ASSERT_BREAK(memBlock <= POOLALLOCATOR_SIZE
								  , "Cannot allocate a single element which size (%d*%d=%d) is bigger than POOLALLOCATOR_SIZE (%d)"
								  , count, sizeof(T), memBlock, POOLALLOCATOR_SIZE);
				initPool();

				T* mem = NULL;
				u32 poolId = 0;
				u32 freeId = 0;
				while (!mem)
				{
					// Allocate a whole new pool, create a new FreeList
					if (poolId == _pool->size())
					{
						_pool->push_back(MemBlock(POOLALLOCATOR_SIZE));
						(*_free)[poolId].push_back(FreeBlock(_pool->back().data));
					}

					// Search a free block big enough
					std::deque<FreeBlock>& freeDeque = (*_free)[poolId];
					while (!mem && freeId < freeDeque.size())
					{
						// We've got a free list in this pool
						FreeBlock& freeBlock = freeDeque[freeId];
						if (memBlock <= freeBlock.size)
						{
							mem = (T*)freeBlock.index;
							// Update the free block
							if (freeBlock.size != memBlock)
							{
								freeBlock.size -= memBlock;
								freeBlock.index += memBlock;
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
			static T* construct(u32 count, T* ptr, const AllocatorInfo& info, Args... args)
			{
				for (u32 c = 0; c < count; ++c)
				{
					new (ptr + c) T(args...);
				}
				return ptr;
			}

			template<typename T>
			static void destroy(T* ptr, const AllocatorInfo& info = {})
			{
				//TODO
			}

			template<typename T>
			static void deallocate(u32 count, T* ptr, const AllocatorInfo& info = {})
			{
				//TODO
			}

		private:
			struct MUON_API MemBlock
			{
				MemBlock(u32 size);
				MemBlock(const MemBlock& block);
				~MemBlock();

				u32* data = NULL;
			};

			struct MUON_API FreeBlock
			{
				FreeBlock(u32* poolIndex);
				u32 size = POOLALLOCATOR_SIZE;
				u32* index = NULL;
			};

			typedef std::deque<MemBlock> MemBlockList;
			typedef std::map<u32, std::deque<FreeBlock>> MapFreeBlockList;

			static MemBlockList* _pool;
			static MapFreeBlockList* _free;

			static void initPool()
			{
				if (_pool == NULL)
				{
					_pool = MUON_CNEW(MemBlockList);
				}

				if (_free == NULL)
				{
					_free = MUON_CNEW(MapFreeBlockList);
				}
			}

			static void mergeFreeBlock()
			{
				// TODO
			}
		};
	}
}

#endif
