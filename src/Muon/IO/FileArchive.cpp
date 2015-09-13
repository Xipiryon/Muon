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

#include "Muon/System/Assert.hpp"
#include "Muon/System/Log.hpp"
#include "Muon/IO/FileArchive.hpp"

namespace muon
{
	namespace io
	{
		FileArchive::FileArchive()
			: IStream()
			, _file(new std::fstream())
			, _filename("")
		{
		}

		FileArchive::FileArchive(const String& filename)
			: IStream()
			, _file(new std::fstream())
			, _filename(filename)
		{
			open(filename);
		}

		FileArchive::~FileArchive()
		{
			if (_file != NULL)
			{
				_file->close();
				delete _file;
				_file = NULL;
			}
		}

		bool FileArchive::open(const String& filename)
		{
			MUON_ASSERT(!_file->is_open(), "[ARCHIVE] FileArchive already open at \"%s\"\n", _filename.cStr());

			if (!_file->is_open())
			{
				_filename = filename;

				//Try open file
				_file->open(_filename.cStr()
							, std::ios_base::in
							| std::ios_base::out
							| std::ios_base::binary);

				//If not, create the file
				if (!_file->is_open())
				{
					_file->close();
					_file->open(_filename.cStr()
								, std::ios_base::in
								| std::ios_base::out
								| std::ios_base::binary
								| std::ios_base::trunc);

					if (!_file->is_open())
					{
						system::Log("[FileArchive]", LOG_ERROR) << "Could not load \"" << _filename << "\" archive" << endl;
						return false;
					}
					return true;
				}
				return true;
			}
			return false;
		}

		bool FileArchive::isOpen() const
		{
			return (_file != NULL && _file->is_open());
		}

		void FileArchive::close()
		{
			_file->close();
		}

		// WRITE
		// ****************************
		FileArchive& FileArchive::operator<<(const char* pod)
		{
			_file->write(pod, ::strlen(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(u64 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(u32 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(u16 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(u8 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(i64 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(i32 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(i16 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(i8 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(f64 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(f32 pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator<<(bool pod)
		{
			_file->write((char*)&pod, sizeof(pod));
			return *this;
		}

		// READ
		// ****************************
		FileArchive& FileArchive::operator>>(u64& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(u32& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(u16& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(u8& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(i64& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(i32& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(i16& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(i8& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(f64& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(f32& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}

		FileArchive& FileArchive::operator>>(bool& pod)
		{
			_file->read((char*)&pod, sizeof(pod));
			return *this;
		}
	}
}
