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

#include <iostream>
#include <Muon/System/Log.hpp>
#include <Muon/Meta/MetaDatabase.hpp>
#include <Muon/Meta/Variant.hpp>
#include <Muon/Type/String.hpp>

struct C { virtual void* foo() = 0; private: char c[128]; };

struct B : C { virtual void* foo() { return (void*) this; } };

struct A : B
{
	A() { std::cout << "A()" << std::endl; }
	~A() { std::cout << "~A()" << std::endl; }
	int i;
	float f;


private:
	char c;
};
MUON_TRAITS(A);

int main(int argc, char** argv)
{
	muon::system::Log::registerDefaultLogImpl();
	muon::system::Log mainLog("Main", muon::LOG_DEBUG);

	mainLog() << "Number of arguments: " << argc << muon::endl;
	for (muon::i32 i = 0; i < argc; ++i)
	{
		mainLog() << "\t: " << argv[i] << muon::endl;
	}

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
#ifdef MUON_PLATFORM_WINDOWS
	::system("PAUSE");
#endif
	return 0;
}
