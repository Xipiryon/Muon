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

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "Muon/System/Log.hpp"
#include "Muon/System/Assert.hpp"

namespace muon
{
	namespace system
	{
		void assertMessage(const char* file, const char* func, u32 line, const char* format, ...)
		{
			// Reserve twice the length of format
			i32 size = (strlen(format) + 1) * 2;
			i32 finalSize = 0;
			va_list args;
			char* buffer;
			do
			{
				if (finalSize < 0 || finalSize >= size)
				{
					size += abs(finalSize - size + 1);
				}

				buffer = (char*)malloc(size);
				strncpy(buffer, format, size);
				va_start(args, format);
				finalSize = vsnprintf(buffer, size, format, args);
				va_end(args);
			}
			while (finalSize < 0 || finalSize >= size);

			//Output it
			Log log("ASSERT", LOG_ERROR);
			log() << file << muon::endl;
			log() << "[" << func << ":" << line << "]" << muon::endl;
			log() << buffer << muon::endl;
			free(buffer);
		}
	}
}
