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

#ifndef INCLUDE_MUON_ISTREAM_HPP
#define INCLUDE_MUON_ISTREAM_HPP

#include "Muon/Core/Typedef.hpp"
#include "Muon/IO/ISerializable.hpp"
#include "Muon/String.hpp"

namespace muon
{
	namespace io
	{
		class MUON_API IStream
		{
		public:
			IStream() {}
			virtual ~IStream() {}

			// Write
			virtual IStream& operator<<(u64 pod) = 0;
			virtual IStream& operator<<(u32 pod) = 0;
			virtual IStream& operator<<(u16 pod) = 0;
			virtual IStream& operator<<(u8 pod) = 0;
			virtual IStream& operator<<(i64 pod) = 0;
			virtual IStream& operator<<(i32 pod) = 0;
			virtual IStream& operator<<(i16 pod) = 0;
			virtual IStream& operator<<(i8 pod) = 0;
			virtual IStream& operator<<(f64 pod) = 0;
			virtual IStream& operator<<(f32 pod) = 0;
			virtual IStream& operator<<(bool pod) = 0;

			// Read
			virtual IStream& operator>>(u64& pod) = 0;
			virtual IStream& operator>>(u32& pod) = 0;
			virtual IStream& operator>>(u16& pod) = 0;
			virtual IStream& operator>>(u8& pod) = 0;
			virtual IStream& operator>>(i64& pod) = 0;
			virtual IStream& operator>>(i32& pod) = 0;
			virtual IStream& operator>>(i16& pod) = 0;
			virtual IStream& operator>>(i8& pod) = 0;
			virtual IStream& operator>>(f64& pod) = 0;
			virtual IStream& operator>>(f32& pod) = 0;
			virtual IStream& operator>>(bool& pod) = 0;
		};
	}
}

#endif
