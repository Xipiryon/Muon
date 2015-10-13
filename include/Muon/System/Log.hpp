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

#ifndef INCLUDE_MUON_LOG_HPP
#define INCLUDE_MUON_LOG_HPP

#include <vector>
#include "Muon/IO/IStream.hpp"
#include "Muon/System/ILogImpl.hpp"
#include "Muon/String.hpp"

namespace muon
{
	struct Endl {};

	/*!
	* @brief End line structure
	*
	* This static instance of the (internal) Endl structure is designed to
	* be used by Log classes, and its behaviour is overriden in ILogImpl classes.
	* The system::ILogImpl::endl function will be called (for each registered impls)
	* when a muon::endl instance is given to the Log class.
	* @code
		struct CustomILogImpl : public muon::system::ILogImpl
		{
			ILogImpl& endl()
			{
				return (*this << "\n");
			}
			// Other overrides are skipped for code simplicity
		}

		// ...
		// Skipping the registering and Log class instantiation part
		// ...
		log() << "Hello" << muon::endl;	// Will output "Hello\n"
	* @endcode
	* @see system::ILogImpl
	*/
	extern MUON_API Endl endl;

	//! System classes are used to check, assert or log data
	namespace system
	{
		/*! 
		* @brief Create a Log instance to display message or informations
		*
		*/
		class MUON_API Log
		{
			friend class ILogImpl;
			friend struct Endl;

		public:
			/*!
			* @brief Construct a Log instance
			*
			* The constructor can take a String parameter which allow
			* the ouptut to be prefixed with a tag.
			*
			* @param tag A string that will be prepended to any instance's log call
			* @param level Level of Log output
			*/
			Log(const String& tag, LogLevel level = LOG_INFO);

			/*!
			* @brief Construct a Log instance
			*
			* @param level Level of Log output
			*/
			Log(LogLevel level = LOG_INFO);

			~Log();

			/*!
			* @brief Register a new ILogImpl
			* This method will register a Log implementation,
			* which will be used to redirect output stream.
			* @return true if the implementation has been added, false if already added or couldn't be added
			* @note The Engine won't deep-copy the implementation. 
			* Deleting it while still registered will likely cause the engine to crash.
			*/
			static bool registerLogImpl(ILogImpl* logImpl);

			/*!
			* @brief Register default ILogImpl
			* Register default log implementation, acting as the default
			* standard output stream std::cout.
			* @return true if the implementation has been added, false if already added or couldn't be added
			*/
			static bool registerDefaultLogImpl();

			/*!
			* @brief Clear a specific registered ILogImpl
			* @param logImpl Pointer to the registered ILogImpl instance
			* @return true if the implementation has been removed, false if already removed or not registered
			* @note ILogImpl instances will not be deleted by the engine.
			* You always should keep a pointer to your custom implementation, 
			* and deleting it after a call to unregisterLogImpl() or clearLogImpl().
			*/
			static bool unregisterLogImpl(ILogImpl* logImpl);

			/*!
			* @brief Clear all ILogImpl registered
			* If for any reason you have to clear every registered
			* ILogImpl classes, then uses this method.
			* @note ILogImpl instances will not be deleted by the engine.
			* You always should keep a pointer to your custom implementation, 
			* and deleting it after a call to unregisterLogImpl() or clearLogImpl().
			*/
			static void clearLogImpl();

			/*!
			* @brief Open file in log implementation
			* This will dispatch for every registered log implementation
			* the file parameter to their open() method.
			* You don't have to specify any file extension, as it should be
			* handled by any register log implementation.
			* @param filename Path (relative to the executable) to open
			* @return Number of successfully opened log
			*/
			static u32 open(const String& filename);

			/*!
			* @brief Close every opened log
			* @return Number of successfully closed log
			*/
			static u32 close();

			/*!
			* @brief Set the minimum Log level required to print.
			* This setting is used by every instances of Log in the application.
			* @param level Level Specify the lowest level of output used by the Log
			*/
			static void setLevel(LogLevel level);

			/*!
			* @brief Get the minimum Log level required to print.
			*/
			static LogLevel getLevel();

			/*!
			* @brief Log an object
			* Dispatch the object to all registererd ILogImpl.
			* @tparam Tref A reference to the object to be outputed
			* @return Instance to the Log class
			*/
			template<typename Tref> MUON_INLINE Log& operator<<(const Tref& obj)
			{
				if (m_level >= getLevel())
				{
					displayTag();
					auto& logImpl = getLogImpl();
					for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
					{
						**it << obj;
					}
				}
				return *this;
			}

			/*!
			* @brief Log an object address
			* Dispatch the address to all registererd ILogImpl.
			* The pointer will be converted into hexadecimal value before being dispatched.
			* @tparam Tptr A pointer to the object address to be outputed
			* @return Instance to the Log class
			*/
			template<typename Tptr> MUON_INLINE Log& operator<<(Tptr* ptr)
			{
				//Cast pointer to readable address
				String buffer(32, 0);	// use 0 as char to not memset
				char* dest = (char*)buffer.cStr();
				sprintf(dest, "0x%llx", (u64)ptr);
				return *this << buffer;
			}

			/*
			* @brief Change the stored LogLevel of the Log instance
			* If the 'level' parameter is left as defined (LOG_INTERNAL),
			* the default behavior is to use the last level set.
			* @param level New level required to print
			*/
			virtual Log& operator()(LogLevel level = LOG_INTERNAL);

		protected:
			String m_tag;
			LogLevel m_level;

		private:
			static std::vector<ILogImpl*>& getLogImpl();
			static bool checkLogImpl(muon::system::ILogImpl*);
			static bool addLogImpl(muon::system::ILogImpl*);

			void displayTag();
			bool m_tagDisplayed;
		};

		template<> MUON_INLINE Log& Log::operator<<(const char* str)
		{
			if (m_level >= getLevel())
			{
				displayTag();
				auto& logImpl = getLogImpl();
				for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
				{
					**it << str;
				}
			}
			return *this;
		}

		template<> MUON_INLINE Log& Log::operator<<(char* str)
		{
			return (*this << (const char*)str);
		}

		template<> MUON_INLINE Log& Log::operator<<(const Endl& endl)
		{
			auto& logImpl = getLogImpl();
			for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
			{
				(*it)->endl();
			}
			return *this;
		}
	}
}

MUON_TRAITS(muon::system::Log)

MUON_API std::ostream& operator<<(std::ostream& stream, const muon::Endl& endl);

#endif	//INCLUDE_MUON_LOG_HPP
