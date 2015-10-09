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

#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>

#include "Muon/Memory/Allocator.hpp"
#include "Muon/System/Assert.hpp"
#include "Muon/System/Log.hpp"
#include "Muon/System/KeyValue.hpp"

namespace
{
	enum KeyValueType
	{
		STRING =	(1 << 0),
		INT =		(1 << 1),
		FLOAT =		(1 << 2),
		BOOL =		(1 << 3),
		UNKNOWN,
	};
	const char* KeyValueTypeToString(KeyValueType kvt)
	{
		switch (kvt)
		{
			case STRING:
				return "STRING";
			case INT:
				return "INT";
			case FLOAT:
				return "FLOAT";
			case BOOL:
				return "BOOL";
			default:
				return "?UNKNOWN?";
		};
	}

	struct KeyValueInfo
	{
		KeyValueInfo(KeyValueType t = KeyValueType::UNKNOWN)
			: typeInfo(t)
			, string("")
			, integer(0)
		{
		}

		muon::String nameInfo;
		KeyValueType typeInfo;

		muon::String string;
		union
		{
			muon::u64 integer;
			muon::f64 floating;
			bool boolean;
		};
	};

	struct StaticDataInstance
	{
		~StaticDataInstance()
		{
			for(auto it = v.begin(); it != v.end(); ++it)
			{
				MUON_DELETE(*it);
			}
		}
		std::vector<KeyValueInfo*> v;
	};
	static StaticDataInstance m_data;

	std::vector<KeyValueInfo*>::iterator find(const muon::String& name)
	{
		for (auto it = m_data.v.begin(); it != m_data.v.end(); ++it)
		{
			if ((*it)->nameInfo == name)
			{
				return it;
			}
		}
		return m_data.v.end();
	}


	template<typename T>
	bool storeT(T val, const muon::String& name, KeyValueType type)
	{
		auto it = find(name);
		//Already existing: update
		if (it != m_data.v.end())
		{
			KeyValueInfo* kvi = *it;
			MUON_ASSERT(kvi->typeInfo == type
						, "Store: Wrong type for \"%s\": Trying with \"%s\", type is \"%s\"\n"
						, name.cStr()
						, KeyValueTypeToString(type)
						, KeyValueTypeToString(kvi->typeInfo));

			if (kvi->typeInfo == type)
			{
				switch (type)
				{
					case BOOL:
						kvi->boolean = (*((bool*)&val) ? true : false);
						return true;
					case INT:
						kvi->integer = *((muon::u64*)&val);
						return true;
					case FLOAT:
						kvi->floating = *((muon::f64*)&val);
						return true;
					case STRING:
						kvi->string = *((muon::String*)&val);
						return true;
					default:
						MUON_ERROR("Type doesn't match!");
						return false;
				};
			}
			return false;
		}

		//Not existing: adding
		KeyValueInfo* kvi = MUON_CNEW(KeyValueInfo, type);
		kvi->nameInfo = name;
		switch (type)
		{
			case BOOL:
				kvi->boolean = (*((bool*)&val) ? true : false);
				break;
			case INT:
				kvi->integer = *((muon::u64*)&val);
				break;
			case FLOAT:
				kvi->floating = *((muon::f64*)&val);
				break;
			case STRING:
				kvi->string = *((muon::String*)&val);
				break;
			default:
				MUON_ERROR("Can't store unknow type!");
				break;
		};
		m_data.v.push_back(kvi);
		muon::system::Log("KEYVALUE", muon::LOG_DEBUG)
				<< "Added \"" << name
				<< "\" with type \"" << KeyValueTypeToString(type) << "\""
				<< muon::endl;
		return true;
	}

	template<typename T>
	bool retrieveT(T& val, const muon::String& name, KeyValueType type)
	{
		auto it = find(name);
		if (it != m_data.v.end())
		{
			KeyValueInfo* kvi = *it;
			MUON_ASSERT(kvi->typeInfo == type
						, "Retrieve: Wrong type for \"%s\": Trying with \"%s\", type is \"%s\"\n"
						, name.cStr()
						, KeyValueTypeToString(type)
						, KeyValueTypeToString(kvi->typeInfo));

			if (kvi->typeInfo == type)
			{
				switch (type)
				{
					case BOOL:
						val = *((T*)&kvi->boolean);
						break;
					case INT:
						val = *((T*)&kvi->integer);
						break;
					case FLOAT:
						val = *((T*)&kvi->floating);
						break;
					case STRING:
						val = *((T*)&kvi->string);
						break;
					default:
						MUON_ERROR("Type doesn't match!");
						break;
				};
				muon::system::Log("KEYVALUE", muon::LOG_DEBUG)
						<< "Retrieving \"" << name
						<< "\" with type \"" << KeyValueTypeToString(type) << "\""
						<< muon::endl;
				return true;
			}
			return false;
		}
		muon::system::Log("KEYVALUE", muon::LOG_DEBUG)
				<< "Retrieve failed, couldn't find \"" << name << "\""
				<< muon::endl;
		return false;
	}
}

namespace muon
{
	namespace system
	{
		bool KeyValue::exists(const String& name)
		{
			return (find(name) != m_data.v.end());
		}

		bool KeyValue::store(const String& name, const String& val)
		{
			return storeT<String>(val, name, STRING);
		}

		bool KeyValue::store(const String& name, const char* val)
		{
			return storeT<String>(String(val), name, STRING);
		}

		bool KeyValue::store(const String& name, u64 val)
		{
			return storeT<u64>(val, name, INT);
		}

		bool KeyValue::store(const String& name, u32 val)
		{
			return storeT<u32>(val, name, INT);
		}

		bool KeyValue::store(const String& name, u16 val)
		{
			return storeT<u16>(val, name, INT);
		}

		bool KeyValue::store(const String& name, u8 val)
		{
			return storeT<u8>(val, name, INT);
		}

		bool KeyValue::store(const String& name, i64 val)
		{
			return storeT<i64>(val, name, INT);
		}

		bool KeyValue::store(const String& name, i32 val)
		{
			return storeT<i32>(val, name, INT);
		}

		bool KeyValue::store(const String& name, i16 val)
		{
			return storeT<i16>(val, name, INT);
		}

		bool KeyValue::store(const String& name, i8 val)
		{
			return storeT<i8>(val, name, INT);
		}

		bool KeyValue::store(const String& name, f64 val)
		{
			return storeT<f64>(val, name, FLOAT);
		}

		bool KeyValue::store(const String& name, f32 val)
		{
			return storeT<f32>(val, name, FLOAT);
		}

		bool KeyValue::store(const String& name, bool val)
		{
			return storeT<bool>(val, name, BOOL);
		}

		bool KeyValue::retrieve(const String& name, String& val)
		{
			return retrieveT<String>(val, name, STRING);
		}

		bool KeyValue::retrieve(const String& name, u64& val)
		{
			return retrieveT<u64>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, u32& val)
		{
			return retrieveT<u32>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, u16& val)
		{
			return retrieveT<u16>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, u8& val)
		{
			return retrieveT<u8>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, i64& val)
		{
			return retrieveT<i64>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, i32& val)
		{
			return retrieveT<i32>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, i16& val)
		{
			return retrieveT<i16>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, i8& val)
		{
			return retrieveT<i8>(val, name, INT);
		}

		bool KeyValue::retrieve(const String& name, f64& val)
		{
			return retrieveT<f64>(val, name, FLOAT);
		}

		bool KeyValue::retrieve(const String& name, f32& val)
		{
			return retrieveT<f32>(val, name, FLOAT);
		}

		bool KeyValue::retrieve(const String& name, bool& val)
		{
			return retrieveT<bool>(val, name, BOOL);
		}

		bool KeyValue::erase(const String& name)
		{
			auto it = find(name);
			if (it != m_data.v.end())
			{
				KeyValueInfo* kvi = *it;
				MUON_DELETE(kvi);
				m_data.v.erase(it);
				Log("KEYVALUE", LOG_DEBUG) << "Successfuly erased \"" << name << "\"" << muon::endl;
				return true;
			}

			Log("KEYVALUE", LOG_DEBUG) << "Erase failed, couldn't find \"" << name << "\"" << muon::endl;
			return false;
		}
	}
}
