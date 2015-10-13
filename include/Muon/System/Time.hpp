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

#ifndef INCLUDE_MUON_TIME_HPP
#define INCLUDE_MUON_TIME_HPP

#include <ctime>
#include "Muon/Core/Typedef.hpp"
#include "Muon/Traits/TypeTraits.hpp"

namespace muon
{
	namespace system
	{
		class MUON_API Time
		{
		public:
			void start();
			f32 now();

		private:
			clock_t m_start;
			clock_t m_end;
		};

		class MUON_API Timer
		{
		public:
			void start(f32 delay);
			void restart();
			f32 timeLeft(bool normalized = false);
			bool isElapsed();

		private:
			clock_t m_start;
			clock_t m_end;
			bool m_elapsed = false;
			f32 m_delay = 0.f;
		};
	}
}

MUON_TRAITS(muon::system::Time)
MUON_TRAITS(muon::system::Timer)
#endif
