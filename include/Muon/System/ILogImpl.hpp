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

#ifndef INCLUDE_MUON_ILOGIMPL_HPP
#define INCLUDE_MUON_ILOGIMPL_HPP

#include "Muon/Core/Typedef.hpp"

/*!
* @file ILogImpl.hpp
*/
namespace muon
{
	/*!
	* @file ILogImpl.hpp
	* @enum LogLevel
	* @brief Minimum Level required for Log to be printed
	* @see system::Log::setLevel
	* @see system::Log::getLevel
	*/
	enum LogLevel
	{
		LOG_DEBUG       = 0x1,
		LOG_INFO        = 0x2,
		LOG_WARNING     = 0x4,
		LOG_ERROR       = 0x8,

		LOG_INTERNAL    = 0			//!< Internal, should not be used directly
	};

	class String;
	namespace system
	{
		/*!
		* @brief Custom Log Interface
		*
		* In order to create an new log implementation, you must 
		* inherit from this class and then register it.
		* An example of custom log implementation could be:
		* - Output in a HTML format
		* - Remote log to a distant server
		* - Add color when logging to console
		* - etc...
		*/
		class MUON_API ILogImpl
		{
		public:
			ILogImpl();
			virtual ~ILogImpl();

			/*!
			* @brief Open the stream
			*
			* Override this method if you need a 
			* particular way to open your stream.
			* 
			* @param filename The name given to system::Log::open
			* @return true if stream has been correctly opened
			*/
			virtual bool open(const String& filename);

			/*!
			* @brief Close the stream
			*
			* Override this method if you need a 
			* particular way to close your stream.
			*
			* @return true if stream has been correctly closed
			*/
			virtual bool close();

			/*!
			* @brief Set the LogLevel
			*
			* This function is used internaly by the Log class.
			* You can override it if you want a specific behaviour when code
			* like this is used:
			* @code{.cpp}
				// This will call the function with LOG_INTERNAL
				log() << "Hello" << muon::endl;
				// This will call the function with same parameter as for log call
				log(LOG_DEBUG) << "World" << muon::endl;
			* @endcode
			*/
			virtual void operator()(LogLevel level);

			/*!
			* @brief Override the endl behavior
			*
			* Override this method to handle specific case of end line.
			* @return ILogImpl The instance of the ILogImpl class
			*/
			virtual ILogImpl& endl() = 0;

			//! Behavior on const char* parameter
			virtual ILogImpl& operator<<(const char* pod) = 0;
			//! Behavior on u64 parameter
			virtual ILogImpl& operator<<(u64 pod) = 0;
			//! Behavior on u32 parameter
			virtual ILogImpl& operator<<(u32 pod) = 0;
			//! Behavior on u16 parameter
			virtual ILogImpl& operator<<(u16 pod) = 0;
			//! Behavior on u8 parameter
			virtual ILogImpl& operator<<(u8 pod) = 0;
			//! Behavior on i64 parameter
			virtual ILogImpl& operator<<(i64 pod) = 0;
			//! Behavior on i32 parameter
			virtual ILogImpl& operator<<(i32 pod) = 0;
			//! Behavior on i16 parameter
			virtual ILogImpl& operator<<(i16 pod) = 0;
			//! Behavior on i8 parameter
			virtual ILogImpl& operator<<(i8 pod) = 0;
			//! Behavior on f64 parameter
			virtual ILogImpl& operator<<(f64 pod) = 0;
			//! Behavior on f32 parameter
			virtual ILogImpl& operator<<(f32 pod) = 0;
			//! Behavior on bool parameter
			virtual ILogImpl& operator<<(bool pod) = 0;

		protected:
			LogLevel m_level;
		};
	}
}

#endif