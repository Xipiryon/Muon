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

#ifndef INCLUDE_MUON_REFLECT_CLASS_HPP
#define INCLUDE_MUON_REFLECT_CLASS_HPP

#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/Reflect/Type.hpp"
#include "Muon/Reflect/Object.hpp"

namespace m
{
	namespace reflect
	{
		class Class;

		class MUON_API ClassBuilder
		{
		public:
			ClassBuilder(Class&);
			ClassBuilder& value(const String& name, i32 value);

		private:
			Class& m_class;
		};

		class MUON_API Class
		{
			friend class ClassBuilder;
		public:
			Class();

			u64 id() const;
			u32 size() const;
			const String& name() const;

		private:
			u64 m_id;
			u32 m_size;
			String m_name;
		};
	}
}

#endif
