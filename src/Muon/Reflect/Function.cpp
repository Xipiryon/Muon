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

namespace m
{
	namespace reflect
	{
		Function::Function(const String& name)
			: m_name(name)
			, m_retType(eType::NONE)
		{
		}

		Function::Function(const String& name, eType retType, const std::vector<eType>& argTypes)
			: m_name(name)
			, m_retType(retType)
			, m_argTypes(argTypes)
		{
		}

		Function::~Function()
		{
		}

		const Function& Function::retrieve(const String& name)
		{
			return detail::FunctionDatabase::getInstance().get(name);
		}

		String Function::name() const
		{
			return m_name;
		}

		u32 Function::getArgsCount() const
		{
			return m_argTypes.size();
		}

		eType Function::getReturnType() const
		{
			return m_retType;
		}

		eType Function::getArgType(u32 i) const
		{
			return m_argTypes[i];
		}

		Object Function::call(const ArgContainer& args)
		{
			return execute(args);
		}
	}
}
