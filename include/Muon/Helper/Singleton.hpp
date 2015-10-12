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

#ifndef INCLUDE_MUON_SINGLETON_HPP
#define INCLUDE_MUON_SINGLETON_HPP

#include "Muon/Helper/NonCopyable.hpp"

/*
* @file Sinleton.hpp
*/
namespace muon
{
	namespace helper
	{
		/*!
		* @brief Singleton pattern class
		*
		* This class is a helper to create Singleton classes.
		* For a class to be used as such, it must have the following requirements:
		* - Constructor must not have any argument
		* - There must be a first call to instantiate() before any get()
		* - Class must both inherit from Singleton<T> and declare it as friend
		* @code{cpp}
			class A : public muon::modifier::Singleton<A>
			{
				friend class muon::modifier::Singleton<A>;
				public:
					// ...
				private:
					A() {}
					~A() {}
			}
		* @endcode
		*/
		template<typename T>
		class Singleton : NonCopyable
		{
		public:

			/*!
			* @brief Create the unique instance of the class T
			* @return the unique instance of T
			*/
			static T& instantiate()
			{
				MUON_ASSERT(!s_instance, "Singleton instance already created!");
				static T gInstance;
				s_instance = &gInstance;
				return get();
			}

			/*!
			* @brief Return the unique instance of T
			*/
			static T& get()
			{
				MUON_ASSERT_BREAK(s_instance, "Singleton instance has not been created!");
				return *s_instance;
			}

			/*!
			* @brief Return true if the singleton is instantiated, false otherwhise
			*/
			static bool isInstantiated()
			{
				return s_instance != NULL;
			}

		protected:

			Singleton()
				: NonCopyable()
			{
			}

		private:
			static T* s_instance;
		};

		template<typename T> T* Singleton<T>::s_instance = NULL;
	}
}

#endif
