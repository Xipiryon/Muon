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
#include <Muon/Meta/TypeInfo.hpp>
#include <Muon/Meta/MetaDatabase.hpp>
#include <Muon/Type/Variant.hpp>
#include <Muon/Type/String.hpp>

struct C { virtual void* foo() const = 0; private: char c[128]; };

struct B : C { virtual void* foo() const { return (void*) this; } };

struct A : B
{
	A() { std::cout << "A()" << std::endl; }
	~A() { std::cout << "~A()" << std::endl; }
	int i;
	float f;

private:
	char c;
};

struct Z
{
	A* a;
};

MUON_TRAITS(A);
MUON_TRAITS(Z);
MUON_USEPOINTER(Z);

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

	muon::system::Log::open("output.log");
	muon::system::Log mainLog("Main", muon::LOG_DEBUG);

	muon::meta::MetaDatabase::instantiate();

	mainLog() << "Number of arguments: " << argc << muon::endl;
	for (muon::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << muon::endl;
	}
#if 0
	// META
	{
		//*
		muon::system::Log metaLog("Meta", muon::LOG_DEBUG);
		// "#NULL#"
		muon::meta::MetaData* m = NULL;

#define PRINT_META_T(Value) m = MUON_META(Value); PRINT_META(Value)
#define PRINT_META_O(Value) m = MUON_META_OBJECT(Value); PRINT_META(Value)
#define PRINT_META_N(Value) m = MUON_META_NAME(Value); PRINT_META(Value)

#define PRINT_META(Value) \
	if(m) metaLog() << "\"" << #Value << "\" \n\t==> \"" << m->name() << "\"\t" << m->id() << "\t(" << m->size() << ")" << muon::endl << muon::endl; \
	else metaLog() << "#NULL#" << muon::endl << muon::endl; //

		// ok
		PRINT_META_T(void);
		PRINT_META_T(unsigned int);
		PRINT_META_T(muon::u32);

		muon::String s;
		muon::String***** sptr = NULL;
		muon::String sarray[2];
		// ok
		PRINT_META_T(muon::String);
		PRINT_META_O((const muon::String&)s);
		PRINT_META_O(sptr);
		PRINT_META_O(sptr);
		PRINT_META_N("muon::String");
		// "#NULL#" because there is no such registered type
		PRINT_META_N("String");

		PRINT_META_O(metaLog);
		//*/
	}
#endif
#if 0
	{
		//*
		muon::system::Log memberLog("Member", muon::LOG_DEBUG);
#define META_MEMBER_PRINT(Member) memberLog() << "meta::offsetof: " << #Member << " = " << muon::meta::offset(Member)<< muon::endl

		META_MEMBER_PRINT(&A::i);
		META_MEMBER_PRINT(&A::f);

		A a;
		char* obj = (char*)&a;
		muon::u64 o = muon::meta::offset(&A::f);
		float* pf = (float*)(obj + o);
		*pf = 7.4f;

		memberLog() << "a.f = " << a.f << " | " << pf << " " << &a.f<< muon::endl;
		//*/
	}
#endif
#if 0
	{
		//*
#define PRINT_VARIANT_O(Value) v = Value; m = v.getMeta(); PRINT_VARIANT(Value)
#define PRINT_VARIANT(Value) \
	if(m) variantLog() << "\"" << #Value << "\" \n\t==> \"" << m->name() << "\"\t" << m->id() << "\t(" << m->size() << ")" << muon::endl << muon::endl; \
	else variantLog() << "#NULL#" << muon::endl << muon::endl; //

		muon::meta::MetaData* m = NULL;
		muon::system::Log variantLog("Variant", muon::LOG_DEBUG);

		muon::meta::Variant v;
		PRINT_VARIANT_O(12);
		PRINT_VARIANT_O(4.7);
		PRINT_VARIANT_O(v);
		//*/
	}
#endif
#if 0
	{
		//*
		muon::system::Log log("MetaMember");
		MUON_META_REGISTER(A);
		//muon::meta::Variant v;
		muon::meta::MetaData* custom = MUON_META_CREATE("Custom");
		muon::meta::MetaData* m = custom;
		m = MUON_META_NAME("Custom");
		log() <<  " ==> \"" << m->name() << "\"\t" << m->id() << "\t(" << m->size() << ")" << muon::endl;

		m->addAttribute<int>("i");
		m->addAttribute<float>("f");
		m->addAttribute<char>("i");

		muon::meta::MetaMember mb = custom->getAttribute("i");
		m = mb.meta();
		log() <<  " ==> MetaMember\"\t" << mb.name() << "\"\t" << mb.offset() << muon::endl;
		log() <<  " ===> MetaData\"\t" << m->name() << "\"\t" << m->id() << "\t(" << m->size() << ")" << muon::endl;

		mb = custom->getAttribute(4);
		m = mb.meta();
		log() <<  " ==> MetaMember\"\t" << mb.name() << "\"\t" << mb.offset() << muon::endl;
		log() <<  " ===> MetaData\"\t" << m->name() << "\"\t" << m->id() << "\t(" << m->size() << ")" << muon::endl;
	}
#endif
#if 0
	{
		muon::system::Log log("StringReplace");
		muon::String hello = "Hello haleWhale Whale";
		log() << hello.replace("hale", "orld") << muon::endl;;

		hello = "Hello Whale Whalehale";
		log() << hello.replace("hale", "orld_!_:D") << muon::endl;;

		hello = "hale Whale Whale hale";
		log() << hello.replace("hale", "3") << muon::endl;

		hello = "hale Whale Whale hale things more";
		log() << hello.replace("hale", "\n") << muon::endl;

		muon::String hale="hale";
		muon::String n = "\\n";
		log() << hello.replace(hale, n) << muon::endl;
	}
#endif
#if 0
	{
		muon::system::Log log;
		muon::String world = "World";
		A a;
		a.i = 42;
		log << "Hello " << world << ", A a = " << a << muon::endl;
	}
#endif
#if 1
	{
		MUON_META_REGISTER(A);
		MUON_META_REGISTER(Z);
		std::cout	<< " MemCopy == " << std::boolalpha << muon::meta::MemCopyable<A>::value
					<< "\t| NonMemCop == " << std::boolalpha << muon::meta::NonMemCopyable<A>::value
					<< "\t| UsePointer == " << std::boolalpha << muon::meta::UsePointer<A>::value
					<< "\t| UseReference == " << std::boolalpha << muon::meta::UseReference<A>::value
					<< std::endl;
		std::cout	<< " MemCopy == " << std::boolalpha << muon::meta::MemCopyable<Z>::value
					<< "\t| NonMemCop == " << std::boolalpha << muon::meta::NonMemCopyable<Z>::value
					<< "\t| UsePointer == " << std::boolalpha << muon::meta::UsePointer<Z>::value
					<< "\t| UseReference == " << std::boolalpha << muon::meta::UseReference<Z>::value
					<< std::endl;
		A a;
		a.f = 0.42f;
		a.i = 64;
		Z z;
		z.a = &a;
		// Should operate on two different implementation
		muon::Variant v;
		v = a;
		v = z;

		std::cout << "Z: " << v.get<Z>().a << " (A)=> " << ((A*)v.get<Z>().a)->f << std::endl;
	}
#endif

	muon::system::Log::close();
#ifdef MUON_PLATFORM_WINDOWS
	::system("PAUSE");
#endif
	return 0;
}

muon::system::ILogImpl& operator<<(muon::system::ILogImpl& log, const A& a)
{
	log << a.i << " " << (muon::u64)a.foo();
	return log;
}
