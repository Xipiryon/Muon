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

#ifndef INCLUDE_MUON_INTEGERSEQUENCE_HPP
#define INCLUDE_MUON_INTEGERSEQUENCE_HPP

#include "Muon/Core/Typedef.hpp"

namespace m
{
	namespace helper
	{
		// From http://stackoverflow.com/a/32223343/2087136

		template <u64... Ints>
		struct index_sequence
		{
			using type = index_sequence;
			using value_type = u64;
			static MUON_CONSTEXPR u64 size()
			{
				return sizeof...(Ints);
			}
		};

		template <class Sequence1, class Sequence2>
		struct _merge_and_renumber;

		template <u64... I1, u64... I2>
		struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
			: index_sequence<I1..., (sizeof...(I1)+I2)...>
		{};

		template <u64 N>
		struct make_index_sequence
			: _merge_and_renumber<typename make_index_sequence<N / 2>::type,
			typename make_index_sequence<N - N / 2>::type>
		{
		};

		template<> struct make_index_sequence<0> : index_sequence<>{};
		template<> struct make_index_sequence<1> : index_sequence<0>{};
	}
}

#endif
