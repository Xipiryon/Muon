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

#ifndef INCLUDE_MUON_REFLECT_FUNCTIONIMPL_HPP
#define INCLUDE_MUON_REFLECT_FUNCTIONIMPL_HPP

#include <vector>
#include "Muon/Helper/IndexSequence.hpp"
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Function.hpp"
#include "Muon/Reflect/Detail/FunctionDatabase.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			template<typename T> class CFunction;
			template<typename T> struct CallHelper;

			// Extract the n'th element and cast it to
			// the expected type
			// *******************************
			template<typename T>
			T extractArg(const ArgContainer& args, u32 index)
			{
				MUON_ASSERT_BREAK(args[index].id() == traits::TypeTraits<T>::id()
								  , "Argument does not match expected type: '%s' != '%s'"
								  , args[index].name().cStr(), traits::TypeTraits<T>::name());
				return args[index].get<T>();
			}

			// Helper caller, extract and call the function
			// *******************************
			template<typename Ret>
			struct CallHelper
			{
				template<typename Func, typename...Args>
				static Value call(const Func& function, const ArgContainer& args)
				{
					return callImpl<Func, Args...>(function, args, helper::make_index_sequence<sizeof...(Args)>());
				}

			protected:
				template<typename Func, typename...Args, m::u32...Indexes>
				static Value callImpl(const Func& function, const ArgContainer& args, helper::index_sequence<Indexes...>)
				{
					return function(extractArg<Args>(args, Indexes)...);
				}
			};

			// Specialization for void
			// ------------------
			template<>
			struct CallHelper<void>
			{
				template<typename Func, typename...Args>
				static Value call(const Func& function, const ArgContainer& args)
				{
					return callImpl<Func, Args...>(function, args, helper::make_index_sequence<sizeof...(Args)>());
				}

			protected:
				template<typename Func, typename...Args, m::u32...Indexes>
				static Value callImpl(const Func& function, const ArgContainer& args, helper::index_sequence<Indexes...>)
				{
					function(extractArg<Args>(args, Indexes)...);
					return None();
				}
			};

			// C-style function with arguments
			// *******************************
			template<typename Ret, typename...Args>
			class CFunction<Ret(Args...)> : public Function
			{
			public:
				CFunction(const String& name, Ret(*function)(Args...))
					: Function(name, detail::mapToEnum<Ret>(), { mapToEnum<Args>()... })
					, m_function(function)
				{
				}

			protected:
				virtual Value execute(const UserObject& obj, const ArgContainer& args) const
				{
					MUON_ASSERT_BREAK(args.count() == sizeof...(Args)
									  , "Parameter count not matching for '%s': expected %d, got %d!"
									  , name().cStr(), args.count(), sizeof...(Args));

					if (args.count() == sizeof...(Args))
					{
						return CallHelper<Ret>::call<decltype(m_function), Args...>(m_function, args);
					}
					return None();
				}
			private:
				std::function<Ret(Args...)> m_function;
			};

			// Specialization with no arguments
			// MSVC can't handle this with an empty initializer list for Args
			// ------------------
			template<typename Ret>
			class CFunction<Ret()> : public Function
			{
			public:
				CFunction(const String& name, Ret(*function)())
					: Function(name, detail::mapToEnum<Ret>(), {})
					, m_function(function)
				{
				}

			protected:
				virtual Value execute(const UserObject& obj, const ArgContainer& args) const
				{
					MUON_ASSERT_BREAK(args.count() == 0
									  , "Parameter count not matching for '%s': expected %d, got %d!"
									  , name().cStr(), 0, args.count());

					if (args.count() == 0)
					{
						return CallHelper<Ret>::call(m_function, args);
					}
					return None();
				}
			private:
				std::function<Ret()> m_function;
			};
		}
	}
}

template<typename Ret, typename...Args>
m::reflect::Function& m::reflect::Function::declare(const m::String& name, Ret(*fptr)(Args...))
{
	Function* f = MUON_NEW(detail::CFunction<Ret(Args...)>, name, fptr);
	return m::reflect::detail::FunctionDatabase::getInstance().add(name, f);
}

#endif
