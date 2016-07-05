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

#include "Muon/Reflect/Function.hpp"
#include "Muon/Reflect/Detail/FunctionDatabase.hpp"

namespace
{
	void emptyFunctionAssert()
	{
		MUON_ERROR("Not implemented!");
	}

	m::String EMPTY_FUNCTION_NAME = "#EmptyFunction#";
	m::reflect::detail::CFunction<void()> EMPTY_FUNCTION(EMPTY_FUNCTION_NAME, &emptyFunctionAssert);
}

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			FunctionDatabase::~FunctionDatabase()
			{
				for (auto it = m_functions.begin(); it != m_functions.end(); it++)
				{
					MUON_DELETE(it->second);
				}
			}
			Function& FunctionDatabase::add(const m::String& name, Function* function)
			{
				auto it = m_functions.find(name);
				MUON_ASSERT_BREAK(it == m_functions.end(), "Function '%s' already registered!", name.cStr());

				if (it == m_functions.end())
				{
					m_functions[name] = function;
					return (*m_functions[name]);
				}

				return (*it->second);
			}

			void FunctionDatabase::remove(const String& name)
			{
				auto it = m_functions.find(name);
				MUON_ASSERT_BREAK(it != m_functions.end(), "Function '%s' not registered!", name.cStr());

				if (it != m_functions.end())
				{
					m_functions.erase(it);
				}
			}

			Function& FunctionDatabase::get(const String& name)
			{
				auto it = m_functions.find(name);
				MUON_ASSERT_BREAK(it != m_functions.end(), "Function '%s' not registered!", name.cStr());

				if (it != m_functions.end())
				{
					return (*it->second);
				}
				return EMPTY_FUNCTION;
			}

			u32 FunctionDatabase::size() const
			{
				return m_functions.size();
			}

			bool FunctionDatabase::exists(const String& name)
			{
				return (m_functions.find(name) != m_functions.end());
			}
		}
	}
}
