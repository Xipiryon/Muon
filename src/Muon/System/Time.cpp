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

#include "Muon/System/Time.hpp"

namespace muon
{
	namespace system
	{
		////////////////////////////
		// TIME
		////////////////////////////
		void Time::start()
		{
			m_start = clock();
		}

		f32 Time::now()
		{
			return ((f32)clock() - m_start) / CLOCKS_PER_SEC;
		}

		////////////////////////////
		// TIMER
		////////////////////////////
		void Timer::start(f32 delay)
		{
			m_delay = delay <= 0.f ? 0.f : delay;
			restart();
		}

		void Timer::restart()
		{
			m_start = clock();
			m_elapsed = false;
		}

		f32 Timer::timeLeft(bool normalized)
		{
			f32 current = m_delay - (((f32)clock() - m_start) / CLOCKS_PER_SEC);
			if (normalized)
			{
				return current / m_delay;
			}
			return (current <= 0.f ? 0.f : current);
		}

		bool Timer::isElapsed()
		{
			m_elapsed = (timeLeft(false) <= 0.f);
			return m_elapsed;
		}
	}
}

