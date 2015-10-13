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

#include <iostream>
#include <vector>
//#include <cstdio>
//#include <cstring>
//#include <cstdarg>

#include "Muon/String.hpp"
#include "Muon/System/Log.hpp"

namespace
{
	// Code is in ILogImpl.cpp
	class DefaultLogImpl : public muon::system::ILogImpl
	{
	public:
		DefaultLogImpl()
		{
		}
		virtual ~DefaultLogImpl()
		{
		}

		virtual ILogImpl& endl()
		{
			std::cout << std::endl; return *this;
		}

		virtual void operator()(muon::LogLevel level)
		{
			m_level = level;
		}

		virtual ILogImpl& operator<<(const char* pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::u64 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::u32 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::u16 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::u8 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::i64 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::i32 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::i16 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::i8 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::f64 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(muon::f32 pod)
		{
			std::cout << pod; return *this;
		}
		virtual ILogImpl& operator<<(bool pod)
		{
			std::cout << pod; return *this;
		}
	};
	DefaultLogImpl m_defaultLogImpl;

	std::vector<muon::system::ILogImpl*>& privGetLogImpl()
	{
		static std::vector<muon::system::ILogImpl*> m_logImpl;
		return m_logImpl;
	}
	;
#if defined(MUON_DEBUG)
	muon::LogLevel m_logLevel = muon::LOG_DEBUG;
#else
	muon::LogLevel m_logLevel = muon::LOG_INFO;
#endif

	muon::String m_openFilename;
	muon::String m_filename = "log_output";
	bool m_opened = false;

	template<typename T> bool notInVector(std::vector<T>& v, T f)
	{
		for (auto it = v.begin(); it != v.end(); ++it)
		{
			if (*it == f)
			{
				return false;
			}
		}
		return true;
	}
}

namespace muon
{
	/*extern*/ Endl endl;

	namespace system
	{
		std::vector<ILogImpl*>& Log::getLogImpl()
		{
			return privGetLogImpl();
		}

		bool Log::addLogImpl(ILogImpl* impl)
		{
			getLogImpl().push_back(impl);
			if (m_opened)
			{
				impl->open(m_openFilename);
			}
			//No real reason to return false
			return true;
		}

		bool Log::checkLogImpl(ILogImpl* impl)
		{
			if (impl == NULL)
			{
				Log("ILogImpl", LOG_ERROR)
						<< "Couldn't allocate ILogImpl!"
						<< muon::endl;
				return false;
			}
			return true;
		}

		Log::Log(const String& tag, LogLevel level)
			: m_level(level)
			, m_tagDisplayed(false)
		{
			if (tag.empty())
			{
				m_tag = " ";
			}
			else
			{
				m_tag = " [" + tag + "] ";
			}

			(*this)(m_level);
		}

		Log::Log(LogLevel level)
			: m_level(level)
			, m_tagDisplayed(false)
		{
			(*this)(m_level);
		}

		void Log::displayTag()
		{
			if (!m_tagDisplayed)
			{
				m_tagDisplayed = true;
				auto& logImpl = getLogImpl();

				for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
				{
					**it << m_tag;
				}
			}
		}

		Log::~Log()
		{
		}

		u32 Log::open(const String& filename)
		{
			u32 count = 0;
			auto& logImpl = getLogImpl();
			for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
			{
				if ((*it)->open(filename))
				{
					++count;
				}
			}
			m_openFilename = filename;
			m_opened = true;
			return count;
		}

		u32 Log::close()
		{
			u32 count = 0;
			auto& logImpl = getLogImpl();
			for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
			{
				if ((*it)->close())
				{
					++count;
				}
			}
			m_openFilename.clear();
			m_opened = false;
			return count;
		}

		bool Log::registerLogImpl(ILogImpl* logImpl)
		{
			if (checkLogImpl(logImpl))
			{
				return addLogImpl(logImpl);
			}
			return false;
		}

		bool Log::registerDefaultLogImpl()
		{
			return registerLogImpl(&m_defaultLogImpl);
		}

		bool Log::unregisterLogImpl(ILogImpl* logImpl)
		{
			auto& logImplList = privGetLogImpl();
			for (u32 i = 0; i < logImplList.size(); ++i)
			{
				if (logImplList[i] == logImpl)
				{
					logImplList.erase(logImplList.begin() + i);
					return true;
				}
			}
			return false;
		}

		void Log::clearLogImpl()
		{
			privGetLogImpl().clear();
		}

		Log& Log::operator()(LogLevel level)
		{
			auto& logImpl = getLogImpl();
			m_level = (level == LOG_INTERNAL ? m_level : level);
			for (auto it = logImpl.begin(); it != logImpl.end(); ++it)
			{
				(*it)->operator()(m_level);
			}
			m_tagDisplayed = false;
			return *this;
		}

		void Log::setLevel(LogLevel level)
		{
			m_logLevel = level;
		}

		LogLevel Log::getLevel()
		{
			return m_logLevel;
		}
	}
}

std::ostream& operator<<(std::ostream& stream, const muon::Endl& endl)
{
	return (stream << std::endl);
}
