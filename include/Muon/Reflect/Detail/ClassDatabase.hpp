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

#ifndef INCLUDE_MUON_REFLECT_CLASSDATABASE_HPP
#define INCLUDE_MUON_REFLECT_CLASSDATABASE_HPP

#include <map>
#include "Muon/String.hpp"
#include "Muon/Helper/Singleton.hpp"
#include "Muon/Reflect/Class.hpp"

namespace m
{
	namespace reflect
	{
		namespace detail
		{
			class MUON_API ClassDatabase : public helper::Singleton<ClassDatabase>
			{
				friend class helper::Singleton<ClassDatabase>;
			public:
				~ClassDatabase();

				Class& add(const String& name);
				void remove(const String& name);
				const Class& retrieve(const String& name);

				u32 size() const;
				bool exists(const String& name);

			private:
				std::map<String, Class> m_classes;
			};
		}
	}
}

#endif
