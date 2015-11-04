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
#include <Muon/Variant.hpp>

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

struct SimpleObject
{
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

namespace test
{
	struct TestObject {};
}

MUON_TRAITS_META_REGISTER(UnitTestObject);
MUON_TRAITS_META_REGISTER(SimpleObject);
MUON_TRAITS_META_REGISTER(test::TestObject);

int main(int argc, char** argv)
{
	LogFile logFile;
	muon::system::Log::registerDefaultLogImpl();
	muon::system::Log::registerLogImpl(&logFile);

	muon::system::Log::open("unit_test.log");
	muon::system::Log mainLog("Main", muon::LOG_INFO);

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

	// Check that PoolAllocator correctly allocate, construct, destruct and free memory
	// Check also that for allocated object A and B, if A is freed, then next will be allocated at A last place
	{
		UnitTestObject* pUTO = NULL;
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
	}

	// Check the MetaData system, and register all the type we need
	{
		MUON_TITLE(" ** Checking muon::meta::MetaDatabase ** ");
		MUON_CHECK(muon::meta::MetaDatabase::isInstantiated(), "MetaDatabase is not already instantiated!");
		UnitTestObject uto;
		muon::meta::MetaData* data = MUON_META(UnitTestObject);
		muon::meta::MetaData* dataName = MUON_META_NAME("UnitTestObject");
		muon::meta::MetaData* dataObject = MUON_META_OBJECT(uto);

		muon::u32 eCount = errorCount;
		MUON_CHECK(data, "MUON_META returned a null MetaData!");
		MUON_CHECK(dataName, "MUON_META_NAME returned a null MetaData!");
		MUON_CHECK(dataObject, "MUON_META_OBJECT returned a null MetaData!");
		// Skip following test if there is errors from last steps
		if(eCount == errorCount)
		{
			MUON_CHECK(data->id() == muon::traits::TypeTraits<UnitTestObject>::id(), "ID retrieved from MUON_META and TypeTraits does not match! (%lu != %lu)", data->id(), muon::traits::TypeTraits<UnitTestObject>::id());
			MUON_CHECK(data->id() == dataName->id(), "ID retrieved from MUON_META and MUON_META_NAME does not match! (%lu != %lu)", data->id(), dataName->id());
			MUON_CHECK(data->id() == dataObject->id(), "ID retrieved from MUON_META and MUON_META_OBJECT does not match! (%lu != %lu)", data->id(), dataObject->id());
		}
	}

	// Check that variant can correctly alternate between types without memory losses
	{
		MUON_TITLE(" ** Checking muon::Variant ** ");
		muon::Variant v;
		v = 45;
		muon::u32 r = v.get<muon::u32>();
		MUON_CHECK(r == 45, "Variant stored POD does not match! %d != %d", r, 45);
		// Create a object destroyed after affectations
		// It should still match as memcopied in Variant
		{
			UnitTestObject uto;
			uto.value = 1024;
			v = uto;
		}
		r = v.get<UnitTestObject>().value;
		MUON_CHECK(r == 1024, "Variant memcopied class does not match! %d != %d", r, 1024);
		// Do the same with a non memcopiable object
		{
			muon::String s;
			s = "Hello World!";
			v = s;
		}
		{
			muon::String s = v.get<muon::String>();
			MUON_CHECK(s == "Hello World!", "Variant non-memcopyable class does not match! %s != %s", s.cStr(), "Hello World!");
		}
		// Assure that a Variant copy of a non-memcopyable object is not the same
		{
			muon::Variant v2;
			v2 = v;
			MUON_CHECK(v.getMeta()->id() == v2.getMeta()->id(), "Copied Variant with non-memcopyable object have not the same type id! %d != %d", v.getMeta()->id(), v2.getMeta()->id());
			MUON_CHECK(&(v.get<muon::String>()) != &(v2.get<muon::String>()), "Copied Variant with non-memcopyable object have the same address!");
		}

	}

	// END UNIT TEST
	// ***************

	mainLog(errorCount == 0 ? muon::LOG_INFO : muon::LOG_ERROR) << "Error Count: " << errorCount << muon::endl;

	muon::system::Log::close();
#if defined(MUON_PLATFORM_WINDOWS) && defined(MUON_DEBUG)
	::system("PAUSE");
#endif
	return -((muon::i32)errorCount);
}
