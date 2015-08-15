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

#ifndef _MUON_ILOGGABLE_H_INCLUDED
#define _MUON_ILOGGABLE_H_INCLUDED

#include "Muon/Core/Typedef.hpp"

/*!
* @file ILoggable.hpp
*/
namespace muon
{
	namespace system
	{
		class ILogImpl;
		/*!
		* @brief Interface used to log a custom object
		*
		* You can avoid writting the whole prototype with 
		* MUON_ILOGGABLE_DECL and MUON_ILOGGABLE_IMPL macros.
		* @see MUON_ILOGGABLE_DECL
		* @see MUON_ILOGGABLE_IMPL
		*/
		struct MUON_API ILoggable
		{
			virtual ILogImpl& operator<<(ILogImpl& stream) const = 0;
		};
	}
}
/*!
* @file ILoggable.hpp
* @def MUON_ILOGGABLE_DECL
* Place this macro inside your class header if you just don't want to have the whole line to be written.
* @code
	class MyClass : public muon::system::ILoggable
	{
		MyClass();
		// Do not forget the ';'
		MUON_ILOGGABLE_DECL;
	};
* @endcode
*/
#define MUON_ILOGGABLE_DECL virtual muon::system::ILogImpl& operator<<(muon::system::ILogImpl& stream) const

/*!
* @file ILoggable.hpp
* @def MUON_ILOGGABLE_IMPL
* Place this macro inside your class source if you just don't want to have the whole line to be written.
* @code
	MyClass::MyClass()
	{
	}

	MUON_ILOGGABLE_IMPL(MyClass, ostream)
	{
		ostream << "Not implemented yet" << muon::endl;
	}
* @endcode
* @param Class Your class name
* @param Stream The stream instance name used in the function
* @see MUON_ILOGGABLE_DECL
*/
#define MUON_ILOGGABLE_IMPL(Class, Stream) muon::system::ILogImpl& Class::operator<<(muon::system::ILogImpl& Stream) const

#endif
