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
#include <Muon/System/Time.hpp>
#include <Muon/Variant.hpp>

#include "tinyxml2.h"

class LogFile : public m::system::ILogImpl
{
public:
	LogFile() {}
	virtual ~LogFile() {}

	virtual bool open(const m::String& filename) { m_file.open(filename.cStr()); return true;}
	virtual bool close() { m_file.close(); return true;}

	virtual LogFile& endl() { m_file << "\n"; return *this;}

	//! Behavior on const char* parameter
	virtual LogFile& operator<<(const char* pod) { m_file << pod; return *this; }
	//! Behavior on u64 parameter
	virtual LogFile& operator<<(m::u64 pod) { m_file << pod; return *this;}
	//! Behavior on u32 parameter
	virtual LogFile& operator<<(m::u32 pod) { m_file << pod; return *this;}
	//! Behavior on u16 parameter
	virtual LogFile& operator<<(m::u16 pod) { m_file << pod; return *this;}
	//! Behavior on u8 parameter
	virtual LogFile& operator<<(m::u8 pod) { m_file << pod; return *this;}
	//! Behavior on i64 parameter
	virtual LogFile& operator<<(m::i64 pod) { m_file << pod; return *this;}
	//! Behavior on i32 parameter
	virtual LogFile& operator<<(m::i32 pod) { m_file << pod; return *this;}
	//! Behavior on i16 parameter
	virtual LogFile& operator<<(m::i16 pod) { m_file << pod; return *this;}
	//! Behavior on i8 parameter
	virtual LogFile& operator<<(m::i8 pod) { m_file << pod; return *this;}
	//! Behavior on f64 parameter
	virtual LogFile& operator<<(m::f64 pod) { m_file << pod; return *this;}
	//! Behavior on f32 parameter
	virtual LogFile& operator<<(m::f32 pod) { m_file << pod; return *this;}
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
	static const m::u32 ConstructedValue = 64;
	static const m::u32 DestructedValue = 42;
	m::u32 value;

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
	m::system::Log::registerDefaultLogImpl();
	m::system::Log::registerLogImpl(&logFile);

	m::system::Log::open("unit_test.log");
	m::system::Log mainLog("Main", m::LOG_INFO);

	mainLog() << "Number of arguments: " << argc << m::endl;
	for (m::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << m::endl;
	}

	// Required variables
	m::system::Time clockTest;
	m::String title;
	m::u32 errorCount = 0;
	m::u32 totalTests = 0;
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLElement* xmlRoot = xmlDoc.NewElement("testsuite");

#define MUON_TITLE(msg) do { mainLog() << msg << m::endl; title = msg; } while(false);
#define MUON_NODE_BEGIN(cond)	++totalTests; tinyxml2::XMLElement* xmlNode = xmlDoc.NewElement("testcase"); \
									xmlNode->SetAttribute("name", #cond); \
									xmlNode->SetAttribute("classname", title.cStr()); \
									xmlNode->SetAttribute("time", clockTest.now()*1000);
#define MUON_NODE_ERR(err)		++errorCount; tinyxml2::XMLElement* xmlErr = xmlDoc.NewElement("failure"); xmlErr->SetText(err); xmlNode->InsertEndChild(xmlErr);
#define MUON_NODE_END			xmlRoot->InsertEndChild(xmlNode); clockTest.start();
#if defined(MUON_PLATFORM_WINDOWS)
#	define MUON_CHECK(cond, err, ...)  do { MUON_NODE_BEGIN(cond) if(!(cond)) { MUON_NODE_ERR(err); MUON_ERROR("\t-> " err, __VA_ARGS__);} MUON_NODE_END } while(false);
#else
#	define MUON_CHECK(cond, err, args...) do { MUON_NODE_BEGIN(cond) if(!(cond)) { MUON_NODE_ERR(err); MUON_ERROR("\t-> " err, ##args);} MUON_NODE_END } while(false);
#endif

	// ***************
	// BEGIN UNIT TEST
	m::RawPointer pointer = NULL;
	clockTest.start();
	// Check that PoolAllocator correctly allocate, construct, destruct and free memory
	// Check also that for allocated object A and B, if A is freed, then next will be allocated at A last place
	{
		UnitTestObject* pUTO = NULL;
		MUON_TITLE("m::memory::PoolAllocator");
		pUTO = m::memory::PoolAllocator::allocate<UnitTestObject>(1);
		MUON_CHECK(pUTO, "Could not allocate with PoolAllocator!");
		pointer = m::memory::PoolAllocator::construct(1, pUTO);
		MUON_CHECK(pointer == pUTO, "PoolAllocator::construct returned a different address: %p != %p", pointer, pUTO);
		MUON_CHECK(pUTO->value == UnitTestObject::ConstructedValue, "UnitTestObject not correctly constructed: %d != %d", pUTO->value, UnitTestObject::ConstructedValue);
		m::memory::PoolAllocator::destroy(1, pUTO);
		MUON_CHECK(UnitTestObject::destroyed == true, "UnitTestObject not correctly destroyed: %d != %d", pUTO->value, UnitTestObject::DestructedValue);
		m::memory::PoolAllocator::deallocate(1, pUTO);
		{
			UnitTestObject* newUTO = m::memory::PoolAllocator::allocate<UnitTestObject>(1);
			MUON_CHECK(newUTO == pUTO, "Allocated object is not using previously freed space!");
			m::memory::PoolAllocator::deallocate(1, newUTO);
		}
	}

	// Check the MetaData system, and register all the type we need
	{
		MUON_TITLE("m::meta::MetaDatabase");
		MUON_CHECK(m::meta::MetaDatabase::isInstantiated(), "MetaDatabase is not already instantiated!");
		UnitTestObject uto;
		m::meta::MetaData* data = MUON_META(UnitTestObject);
		m::meta::MetaData* dataName = MUON_META_NAME("UnitTestObject");
		m::meta::MetaData* dataObject = MUON_META_OBJECT(uto);

		m::u32 eCount = errorCount;
		MUON_CHECK(data, "MUON_META returned a null MetaData!");
		MUON_CHECK(dataName, "MUON_META_NAME returned a null MetaData!");
		MUON_CHECK(dataObject, "MUON_META_OBJECT returned a null MetaData!");
		// Skip following test if there is errors from last steps
		if(eCount == errorCount)
		{
			MUON_CHECK(data->id() == m::traits::TypeTraits<UnitTestObject>::id(), "ID retrieved from MUON_META and TypeTraits does not match! (%lu != %lu)", data->id(), m::traits::TypeTraits<UnitTestObject>::id());
			MUON_CHECK(data->id() == dataName->id(), "ID retrieved from MUON_META and MUON_META_NAME does not match! (%lu != %lu)", data->id(), dataName->id());
			MUON_CHECK(data->id() == dataObject->id(), "ID retrieved from MUON_META and MUON_META_OBJECT does not match! (%lu != %lu)", data->id(), dataObject->id());
		}
	}

	// Check that variant can correctly alternate between types without memory losses
	{
		MUON_TITLE("m::Variant");
		m::Variant v;
		v = 45;
		m::u32 r = v.get<m::u32>();
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
			m::String s;
			s = "Hello World!";
			v = s;
		}
		{
			m::String s = v.get<m::String>();
			MUON_CHECK(s == "Hello World!", "Variant non-memcopyable class does not match! %s != %s", s.cStr(), "Hello World!");
		}
		// Assure that a Variant copy of a non-memcopyable object is not the same
		{
			m::Variant v2;
			v2 = v;
			MUON_CHECK(v.getMeta()->id() == v2.getMeta()->id(), "Copied Variant with non-memcopyable object have not the same type id! %d != %d", v.getMeta()->id(), v2.getMeta()->id());
			MUON_CHECK(&(v.get<m::String>()) != &(v2.get<m::String>()), "Copied Variant with non-memcopyable object have the same address!");
		}

		// Check that String are correctly used
		{
			MUON_TITLE("m::String");

			m::String str(5, 'H');
			MUON_CHECK(str == "HHHHH", "String(size, char) constructor failed! (%s)", str.cStr());
			str = "Hello";
			MUON_CHECK(str == "Hello", "Assignment operator failed! (%s)", str.cStr());
			str += ' ';
			MUON_CHECK(str == "Hello ", "Appending char failed (%s)", str.cStr());

			m::String tmp_str("World");
			MUON_CHECK(tmp_str == "World", "Creating String with const char* failed (%s)", tmp_str.cStr());

			str += tmp_str;
			MUON_CHECK(str == "Hello World", "Appending another String failed (%s)", str.cStr());

			m::u32 index = str.find("or");
			MUON_CHECK(index == 7, "Find existing text in string didn't returned correct index (%s | %d)", str.cStr(), index);
			index = str.find("ez");
			MUON_CHECK(index == m::INVALID_INDEX, "Find non-existing text in String didn't returned INVALID_INDEX  (%s | %d)", str.cStr(), index);

			m::String new_str = str.replace("World", "Banana");
			MUON_CHECK(new_str == "Hello Banana", "Replace function failed (%s)", new_str.cStr());

			str.resize(5);
			MUON_CHECK(str == "Hello", "Resize to with lower bound failed (%s)", str.cStr());
			str.resize(15);
			MUON_CHECK(str == "Hello", "Resize to with higher bound failed (%s)", str.cStr());

			str.clear();
			MUON_CHECK(str.empty(), "Clear failed (%s)", str.cStr());

			int i = -6424;
			str << i;
			MUON_CHECK(str == "-6424", "Using operator<< with integer value failed (%s)", str.cStr());
			str.clear();

			// Because of float and decimal approximation, this test could be
			// considerer as "false positive"
			float f = 42.85210037f;
			str << f;
			MUON_CHECK(str == "42.85210037", "Using operator<< with floating value failed (%s)", str.cStr());

			str.clear();
			bool b = false;
			str << b;
			MUON_CHECK(str == "false", "Using operator<< with boolean value failed (%s)", str.cStr());

			str.clear();
			str << 84765 << " " << -0.420 << " " << true;
			MUON_CHECK(str == "84765 -0.42 true", "Chaining operator<< with different value failed (%s)", str.cStr());

			str.clear();
			{
				m::String other = "abcdef";
				str = "abcd";
				MUON_CHECK(str < other, "String::operator< returned a wrong value (%s < %s)", str.cStr(), other.cStr());

				str = "abcdefg";
				MUON_CHECK(str > other, "String::operator> returned a wrong value (%s > %s)", str.cStr(), other.cStr());

				str = "aBcdefgh";
				MUON_CHECK(str < "abcdef", "String::operator< returned a wrong value (%s < %s)", str.cStr(), other.cStr());
			}
		}
	}

	// END UNIT TEST
	// ***************
	mainLog(errorCount == 0 ? m::LOG_INFO : m::LOG_ERROR) << "Error Count: " << errorCount << m::endl;

	xmlRoot->SetAttribute("tests", totalTests);
	xmlDoc.InsertFirstChild(xmlRoot);
	xmlDoc.SaveFile("unittests.xml");

	m::system::Log::close();
#if defined(MUON_PLATFORM_WINDOWS) && defined(MUON_DEBUG)
	::system("PAUSE");
#endif

	return 100 - (m::i32)(errorCount / (m::f32)totalTests) * 100;
}
