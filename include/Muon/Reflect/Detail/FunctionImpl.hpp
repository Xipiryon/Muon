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
#include "Muon/Reflect/Detail/ValueMapper.hpp"
#include "Muon/Reflect/Detail/FunctionDatabase.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			template<typename T> class CFunction;
			template<typename T> struct CallHelper;
			template<typename T, typename = void> struct ArgExtractor;

			// Extract the n'th element and cast it to
			// the expected type
			// *******************************
			template<typename T>
			struct ArgExtractor<T, typename std::enable_if<!std::is_pointer<T>::value>::type>
			{
				static T extract(const ArgContainer& args, u32 index)
				{
					MUON_ASSERT_BREAK(args[index].check<T>()
									  , "Argument does not match: expected '%s', got '%s'"
									  , traits::TypeTraits<typename traits::RawType<T>::type>::name()
									  , args[index].name().cStr());
					return args[index].as<typename traits::RawType<T>::type>();
				}
			};

			// Pointers (not const char*)
			template<typename T>
			struct ArgExtractor<T, typename std::enable_if<
				std::is_pointer<T>::value
				&& !std::is_same<typename traits::RawType<T>::type, const char*>::value
			>::type>
			{
				static T extract(const ArgContainer& args, u32 index)
				{
					MUON_ASSERT_BREAK(args[index].check<T>()
									  , "Argument does not match: expected '%s*', got '%s'"
									  , traits::TypeTraits<typename traits::RawType<T>::type>::name()
									  , args[index].name().cStr());
					return &(args[index].as<typename std::remove_pointer<T>::type>());
				}
			};

			// Specific case for const char*
			template<typename T>
			struct ArgExtractor<T, typename std::enable_if<
				std::is_pointer<T>::value
				&&
				std::is_same<typename traits::RawType<T>::type, const char*>::value
			>::type>
			{
				static T extract(const ArgContainer& args, u32 index)
				{
					MUON_ASSERT_BREAK(args[index].check<T>()
									  , "Argument does not match: expected '%s', got '%s'"
									  , traits::TypeTraits<typename traits::RawType<T>::type>::name()
									  , args[index].name().cStr());
					return args[index].as<String>().cStr();
				}
			};

			// Helper caller, extract and call the function
			// *******************************
			template<typename Ret>
			struct CallHelper
			{
				template<typename Func, typename...Args>
				static Object call(const Func& function, const ArgContainer& args)
				{
					return callImpl<Func, Args...>(function, args, helper::make_index_sequence<sizeof...(Args)>());
				}

			protected:
				template<typename Func, typename...Args, m::u32...Indexes>
				static Object callImpl(const Func& function, const ArgContainer& args, helper::index_sequence<Indexes...>)
				{
					return function(ArgExtractor<Args>::extract(args, Indexes)...);
				}
			};

			// Specialization for void
			// ------------------
			template<>
			struct CallHelper<void>
			{
				template<typename Func, typename...Args>
				static Object call(const Func& function, const ArgContainer& args)
				{
					return callImpl<Func, Args...>(function, args, helper::make_index_sequence<sizeof...(Args)>());
				}

			protected:
				template<typename Func, typename...Args, m::u32...Indexes>
				static Object callImpl(const Func& function, const ArgContainer& args, helper::index_sequence<Indexes...>)
				{
					function(ArgExtractor<Args>::extract(args, Indexes)...);
					return Object();
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
				virtual Object execute(const ArgContainer& args) const
				{
					MUON_ASSERT_BREAK(args.count() == sizeof...(Args)
									  , "Parameter count not matching for '%s': expected %d, got %d!"
									  , name().cStr(), sizeof...(Args), args.count());

					if (args.count() == sizeof...(Args))
					{
						return CallHelper<Ret>::template call<decltype(m_function), Args...>(m_function, args);
					}
					return Object();
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
				virtual Object execute(const ArgContainer& args) const
				{
					MUON_ASSERT_BREAK(args.count() == 0
									  , "Parameter count not matching for '%s': expected %d, got %d!"
									  , name().cStr(), 0, args.count());

					if (args.count() == 0)
					{
						return CallHelper<Ret>::template call(m_function, args);
					}
					return Object();
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
