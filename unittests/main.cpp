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
#include <Muon/Memory/PoolAllocator.hpp>

class LogFile : public muon::system::ILogImpl
{
public:
	LogFile() {}
	virtual ~LogFile() {}

	virtual bool open(const muon::String& filename) { m_file.open(filename.cStr()); return true;}
	virtual bool close() { m_file.close(); return true;}

	virtual LogFile& endl() { m_file << "\n"; return *this;}

	//! Behavior on const char* parameter
	virtual LogFile& operator<<(const char* pod) { m_file << pod; return *this; }
	//! Behavior on u64 parameter
	virtual LogFile& operator<<(muon::u64 pod) { m_file << pod; return *this;}
	//! Behavior on u32 parameter
	virtual LogFile& operator<<(muon::u32 pod) { m_file << pod; return *this;}
	//! Behavior on u16 parameter
	virtual LogFile& operator<<(muon::u16 pod) { m_file << pod; return *this;}
	//! Behavior on u8 parameter
	virtual LogFile& operator<<(muon::u8 pod) { m_file << pod; return *this;}
	//! Behavior on i64 parameter
	virtual LogFile& operator<<(muon::i64 pod) { m_file << pod; return *this;}
	//! Behavior on i32 parameter
	virtual LogFile& operator<<(muon::i32 pod) { m_file << pod; return *this;}
	//! Behavior on i16 parameter
	virtual LogFile& operator<<(muon::i16 pod) { m_file << pod; return *this;}
	//! Behavior on i8 parameter
	virtual LogFile& operator<<(muon::i8 pod) { m_file << pod; return *this;}
	//! Behavior on f64 parameter
	virtual LogFile& operator<<(muon::f64 pod) { m_file << pod; return *this;}
	//! Behavior on f32 parameter
	virtual LogFile& operator<<(muon::f32 pod) { m_file << pod; return *this;}
	//! Behavior on bool parameter
	virtual LogFile& operator<<(bool pod) { m_file << pod; return *this;}

	protected:
		std::ofstream m_file;
};

struct UnitTestObject
{
	static bool destroyed;
	static const muon::u32 ConstructedValue = 64;
	static const muon::u32 DestructedValue = 42;
	muon::u32 value;

	UnitTestObject()
	{
		value = ConstructedValue;
		destroyed = false;
	}

	~UnitTestObject()
	{
		value = DestructedValue;
		destroyed = true;
	}
};
bool UnitTestObject::destroyed = false;

int main(int argc, char** argv)
{
	LogFile logFile;
	muon::system::Log::registerDefaultLogImpl();
	muon::system::Log::registerLogImpl(&logFile);

	muon::system::Log::open("unit_test.log");
	muon::system::Log mainLog("Main", muon::LOG_INFO);

	muon::meta::MetaDatabase::instantiate();

	mainLog() << "Number of arguments: " << argc << muon::endl;
	for (muon::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << muon::endl;
	}

	muon::u32 errorCount = 0;

#define MUON_TITLE(msg) mainLog() << msg << muon::endl
#if defined(MUON_PLATFORM_WINDOWS)
#	define MUON_CHECK(cond, err, ...) if(!(cond)) {++errorCount; MUON_ERROR("\t-> " err, __VA_ARGS__);}
#else
#	define MUON_CHECK(cond, err, args...) if(!(cond)) {++errorCount; MUON_ERROR("\t-> " err, ##args);}
#endif

	// ***************
	// BEGIN UNIT TEST
	muon::RawPointer pointer = NULL;
	UnitTestObject* pUTO = NULL;

	// Check that PoolAllocator correctly allocate, construct, destruct and free memory
	// Check also that for allocated object A and B, if A is freed, then next will be allocated at A last place
	MUON_TITLE(" ** Checking muon::memory::PoolAllocator ** ");
	pUTO = muon::memory::PoolAllocator::allocate<UnitTestObject>(1);
	MUON_CHECK(pUTO, "Could not allocate with PoolAllocator!");
	pointer = muon::memory::PoolAllocator::construct(1, pUTO);
	MUON_CHECK(pointer == pUTO, "PoolAllocator::construct returned a different address: %p != %p", pointer, pUTO);
	MUON_CHECK(pUTO->value == UnitTestObject::ConstructedValue, "UnitTestObject not correctly constructed: %d != %d", pUTO->value, UnitTestObject::ConstructedValue);
	muon::memory::PoolAllocator::destroy(1, pUTO);
	MUON_CHECK(UnitTestObject::destroyed == true, "UnitTestObject not correctly destroyed: %d != %d", pUTO->value, UnitTestObject::DestructedValue);
	muon::memory::PoolAllocator::deallocate(1, pUTO);
	{
		UnitTestObject* newUTO = muon::memory::PoolAllocator::allocate<UnitTestObject>(1);
		MUON_CHECK(newUTO == pUTO, "Allocated object is not using previously freed space!");
		muon::memory::PoolAllocator::deallocate(1, newUTO);
	}

	// END UNIT TEST
	// ***************

	mainLog(errorCount == 0 ? muon::LOG_INFO : muon::LOG_ERROR) << "Error Count: " << errorCount << muon::endl;
	muon::system::Log::close();
#ifdef MUON_PLATFORM_WINDOWS
	::system("PAUSE");
#endif
	return -((muon::i32)errorCount);
}
