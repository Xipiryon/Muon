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

// IMPORTANT
// This file is used only as testing/debugging.
// It is in no way used in finals products, and thus,
// could be modified in any way to ensure new features
// can be used / are working / pass unit tests / etc...

#include <fstream>
#include <iostream>
#include <Muon/System/Log.hpp>
#include <Muon/Meta/MetaDatabase.hpp>

class LogFile : public muon::system::ILogImpl
{
public:
	LogFile() {}
	virtual ~LogFile() {}

	virtual bool open(const muon::String& filename) { _file.open(filename.cStr()); return true;}
	virtual bool close() { _file.close(); return true;}

	virtual LogFile& endl() { _file << "\n"; return *this;}

	//! Behavior on const char* parameter
	virtual LogFile& operator<<(const char* pod) { _file << pod; return *this; }
	//! Behavior on u64 parameter
	virtual LogFile& operator<<(muon::u64 pod) { _file << pod; return *this;}
	//! Behavior on u32 parameter
	virtual LogFile& operator<<(muon::u32 pod) { _file << pod; return *this;}
	//! Behavior on u16 parameter
	virtual LogFile& operator<<(muon::u16 pod) { _file << pod; return *this;}
	//! Behavior on u8 parameter
	virtual LogFile& operator<<(muon::u8 pod) { _file << pod; return *this;}
	//! Behavior on i64 parameter
	virtual LogFile& operator<<(muon::i64 pod) { _file << pod; return *this;}
	//! Behavior on i32 parameter
	virtual LogFile& operator<<(muon::i32 pod) { _file << pod; return *this;}
	//! Behavior on i16 parameter
	virtual LogFile& operator<<(muon::i16 pod) { _file << pod; return *this;}
	//! Behavior on i8 parameter
	virtual LogFile& operator<<(muon::i8 pod) { _file << pod; return *this;}
	//! Behavior on f64 parameter
	virtual LogFile& operator<<(muon::f64 pod) { _file << pod; return *this;}
	//! Behavior on f32 parameter
	virtual LogFile& operator<<(muon::f32 pod) { _file << pod; return *this;}
	//! Behavior on bool parameter
	virtual LogFile& operator<<(bool pod) { _file << pod; return *this;}

	protected:
		std::ofstream _file;
};

int main(int argc, char** argv)
{
	LogFile logFile;
	muon::system::Log::registerDefaultLogImpl();
	muon::system::Log::registerLogImpl(&logFile);

	muon::system::Log::open("unit_test.log");
	muon::system::Log mainLog("Main", muon::LOG_DEBUG);

	muon::meta::MetaDatabase::instantiate();

	mainLog() << "Number of arguments: " << argc << muon::endl;
	for (muon::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << muon::endl;
	}

	// ***************
	// BEGIN UNIT TEST

	// END UNIT TEST
	// ***************

	muon::system::Log::close();
#ifdef MUON_PLATFORM_WINDOWS
	::system("PAUSE");
#endif
	return 0;
}
