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

#ifndef INCLUDE_MUON_REFLECT_FUNCTION_HPP
#define INCLUDE_MUON_REFLECT_FUNCTION_HPP

#include "Muon/Helper/NonCopyable.hpp"
#include "Muon/Reflect/ArgContainer.hpp"

namespace m
{
	namespace reflect
	{
		class MUON_API Function
		{
		public:
			template<typename Ret, typename...Args>
			Function(const String& name);

			Function(const String& name);
			virtual ~Function();

			template<typename Ret, typename...Args>
			static Function& declare(const String& name, Ret(*fptr)(Args...));
			static void undeclare(const String& name);
			static Function& retrieve(const String& name);

			String name() const;
			u32 getArgsCount() const;
			eType getReturnType() const;
			eType getArgType(u32 i) const;

			Value call(const ArgContainer& args = ArgContainer::EMPTY);
			Value call(const UserObject& obj, const ArgContainer& args = ArgContainer::EMPTY);

		protected:
			virtual Value execute(const UserObject& obj, const ArgContainer& args) const = 0;
			Function(const String& name, eType retType, const std::vector<eType>& argTypes);

		private:
			String m_name;
			eType m_retType;
			std::vector<eType> m_argTypes;
		};
	}
}

MUON_TRAITS_DECL_ABSTRACT(m::reflect::Function);

template<typename Ret, typename...Args>
m::reflect::Function::Function(const String& name)
	: m_name(name)
{
}

#include "Muon/Reflect/Detail/FunctionImpl.hpp"

#endif
