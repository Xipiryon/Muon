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

#ifndef INCLUDE_MUON_FILEARCHIVE_HPP
#define INCLUDE_MUON_FILEARCHIVE_HPP

#include <fstream>

#include "Muon/IO/IStream.hpp"

namespace muon
{
	namespace io
	{
		class MUON_API FileArchive : public IStream
		{
		public:
			FileArchive();
			FileArchive(const String& filename);
			virtual ~FileArchive();

			bool open(const String& filename);
			bool isOpen() const;
			void close();

			template<typename T> FileArchive& operator<<(T& obj)
			{
				MUON_ASSERT(isOpen(), "FileArchive not open!");
				return (FileArchive&)IStream::operator<<(obj);
			}

			template<typename T> FileArchive& operator>>(T& obj)
			{
				MUON_ASSERT(isOpen(), "FileArchive not open!");
				return (FileArchive&)IStream::operator>>(obj);
			}

			// POD Type
			// Write
			virtual FileArchive& operator<<(const char* pod);
			virtual FileArchive& operator<<(u64 pod);
			virtual FileArchive& operator<<(u32 pod);
			virtual FileArchive& operator<<(u16 pod);
			virtual FileArchive& operator<<(u8 pod);
			virtual FileArchive& operator<<(i64 pod);
			virtual FileArchive& operator<<(i32 pod);
			virtual FileArchive& operator<<(i16 pod);
			virtual FileArchive& operator<<(i8 pod);
			virtual FileArchive& operator<<(f64 pod);
			virtual FileArchive& operator<<(f32 pod);
			virtual FileArchive& operator<<(bool pod);

			// Read
			virtual FileArchive& operator>>(u64& pod);
			virtual FileArchive& operator>>(u32& pod);
			virtual FileArchive& operator>>(u16& pod);
			virtual FileArchive& operator>>(u8& pod);
			virtual FileArchive& operator>>(i64& pod);
			virtual FileArchive& operator>>(i32& pod);
			virtual FileArchive& operator>>(i16& pod);
			virtual FileArchive& operator>>(i8& pod);
			virtual FileArchive& operator>>(f64& pod);
			virtual FileArchive& operator>>(f32& pod);
			virtual FileArchive& operator>>(bool& pod);

		private:
			const static u32 BUFFER_SIZE = 1024;

			std::fstream *m_file;
			String m_filename;
		};
	}
}

#endif
