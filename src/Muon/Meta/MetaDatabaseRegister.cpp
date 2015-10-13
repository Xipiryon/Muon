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

#include "Muon/Meta/MetaDatabase.hpp"
#include "Muon/System/Log.hpp"
#include "Muon/System/Time.hpp"
#include "Muon/String.hpp"
#include "Muon/Variant.hpp"

namespace muon
{
	namespace meta
	{
		void MetaDatabase::registerInternal()
		{
			registerMeta<void>();
			registerMeta<bool>();
			registerMeta<const char*>();
			registerMeta<char*>();

			registerMeta<u8>();
			registerMeta<u16>();
			registerMeta<u32>();
			registerMeta<u64>();

			registerMeta<i8>();
			registerMeta<i16>();
			registerMeta<i32>();
			registerMeta<i64>();

			registerMeta<f32>();
			registerMeta<f64>();

			registerMeta<RawPointer>();

			registerMeta<system::Log>();
			registerMeta<system::Time>();
			registerMeta<system::Timer>();

			registerMeta<muon::String>();
			registerMeta<muon::Variant>();
		}
	}
}
