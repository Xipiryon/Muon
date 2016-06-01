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

#ifndef INCLUDE_MUON_SHAREDPOINTER_HPP
#define INCLUDE_MUON_SHAREDPOINTER_HPP

#include "Muon/Memory/Utils.hpp"

namespace m
{
	namespace memory
	{
		template<typename T>
		class SharedPointer
		{
			class RefCounter
			{
			public:
				i32 add()
				{
					return ++m_count;
				}

				i32 dec()
				{
					return --m_count;
				}

			private:
				i32 m_count = 0;
			};

		public:

			template<typename... Args>
			explicit SharedPointer(Args&&... args)
				: m_refCount(MUON_NEW(RefCounter))
				, m_data(MUON_NEW(T, args...))
			{
				m_refCount->add();
			}

			SharedPointer(T* ptr)
				: m_refCount(MUON_NEW(RefCounter))
				, m_data(ptr)
			{
				m_refCount->add();
			}

			SharedPointer(const SharedPointer<T>& o)
				: m_refCount(o.m_refCount)
				, m_data(o.m_data)
			{
				m_refCount->add();
			}

			~SharedPointer()
			{
				if (m_refCount->dec() == 0)
				{
					MUON_DELETE(m_data);
					MUON_DELETE(m_refCount);
				}
			}

			SharedPointer& operator=(const SharedPointer<T>& o)
			{
				if (&o != this)
				{
					if (m_refCount->dec() == 0)
					{
						MUON_DELETE(m_data);
						MUON_DELETE(m_refCount);
					}

					m_data = o.m_data;
					m_refCount = o.m_refCount;
					m_refCount->add();
				}
				return *this;
			}

			T& operator*()
			{
				return *m_data;
			}

			T* operator->()
			{
				return m_data;
			}

		private:
			RefCounter *m_refCount;
			T* m_data;
		};
	}
}
#endif
