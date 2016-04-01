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

#ifndef INCLUDE_MUON_STRING_HPP
#define INCLUDE_MUON_STRING_HPP

#include <functional>
#include <ostream>
#include "Muon/Core/Constant.hpp"
#include "Muon/Traits/TypeTraits.hpp"
#include "Muon/IO/ISerializable.hpp"
#include "Muon/System/ILogImpl.hpp"

/*!
* @file String.hpp
*/
namespace m
{
	/*!
	* @brief
	* @param value
	* @param buffer
	*/
	void MUON_API itoa(u64 value, char* buffer);

	/*!
	* @brief
	* @param value
	* @param buffer
	*/
	void MUON_API itoa(i64 value, char* buffer);

	/*!
	* @brief
	* @param value
	* @param buffer
	*/
	void MUON_API ftoa(f64 value, char* buffer);

	/*!
	* @brief Store and manipulate array of characters
	*
	* Every String instances have their internal representation null-terminated,
	* so you don't have to worry about it.
	* @see String.hpp
	*/
	class MUON_API String
	{
	public:
		/*!
		* @brief Construct an empty String
		*/
		String();

		/*!
		* @brief Create a new string from another one
		* @param str The other string
		*/
		String(const String& str);

		/*!
		* @brief Create a String instance based on a char*
		* @param str Null terminated char*
		*/
		String(const char* str);

		/*!
		* @brief Preallocate a new String with a default character
		* @param size The size (in character number) of the new String
		* @param c Default character.
		* @note If default character is 0 ('\0'), the string will be considered as empty
		*/
		String(u32 size, char c);

		/*!
		* @brief Destroy the string and its internal char*
		*/
		~String();

		/*!
		* @brief Update the String from a null-terminated char*
		* @param other The other string
		* @return The updated String instance
		*/
		String& operator=(const char* other);

		/*!
		* @brief Update the String from another String
		* @param str The other string
		* @return The updated String instance
		*/
		String& operator=(const String& other);

		/*!
		* @brief Access the nth character of the String as read-only
		* @param i The index of the character
		* @return The character at the index given as parameter
		*/
		char operator[](u32 i) const;

		/*!
		* @brief Access the nth character of the String as read-write
		* @param i The index of the character
		* @return The character at the index given as parameter
		*/
		char& operator[](u32 i);

		m::String& operator+=(const char* other);
		m::String& operator+=(const m::String& other);
		m::String& operator+=(const char c);

		/*!
		* @brief Return a raw character array
		* @return The raw null terminated character array
		*/
		const char* cStr() const;

		/*!
		* @brief Check for empty string
		* @return true if the string is empty
		* @note A String is considered empty when its internal representation contains '\0' as first character,
		* or when its size is 0.
		* For exemple, resizing a String to more than 1 and setting the first character to '\0' will make it "empty".
		*/
		bool empty() const;

		/*!
		* @brief Retrieve the number of characters
		* @return Return the number of characters stored inside the String
		*/
		u32 size() const;

		/*!
		* @brief Resize the String
		* @param size The new size in character numbers
		*/
		void resize(u32 size);

		/*!
		* @brief Clear the String
		*/
		void clear();

		/*!
		* @brief Find a character
		* @param c The character you want to find
		* @param pos Character index to start at
		* @return Index at which the substring has been found, or INVALID_INDEX
		*/
		u32 find(char c, u32 pos = 0) const;

		/*!
		* @brief Find a string
		* @param A raw null terminated string character array
		* @param pos Character index to start at
		* @return Index at which the substring has been found, or INVALID_INDEX
		*/
		u32 find(const char* s, u32 pos = 0) const;

		/*!
		* @brief Find a string
		* @param str String to find
		* @param pos Character index to start at
		* @return Index at which the substring has been found, or INVALID_INDEX
		*/
		u32 find(const String& str, u32 pos = 0) const;

		/*!
		* @brief Return a specific substring
		* @param pos Character index to start at (character at this index is included in the substring)
		* @param len Number of character you want to retrieve from starting position. 0 means "everything until the end of the string"
		* @return A new String instance containing only the specified substring, or an empty one if outside boundaries
		* @code
			String hello = "Hello World";
			hello.substr(0, 5); // "Hello"
		* @endcode
		*/
		String substr(u32 pos, u32 len = 0) const;

		/*!
		* @brief Return a string with some content replaced
		* @param src The content to be replaced
		* @param dst The new content
		* @return A new String instance where 'src' has been replaced by 'dst'
		* @code
		String hello = "Hello Whale";
		hello.replace("hale", "orld"); // "Hello World"
		* @endcode
		*/
		String replace(const String& src, const String& dst) const;

		/*!
		* @brief Return a string with some content replaced
		* @param src The content to be replaced
		* @param dst The new content
		* @return A new String instance where 'src' has been replaced by 'dst'
		* @code
		String hello = "Hello Whale";
		hello.replace("hale", "orld"); // "Hello World"
		* @endcode
		*/
		String replace(const char* src, const char* dst) const;

		/*!
		* @brief Check that current stored string is UTF8.
		* @return True if every characters are encoded as UTF8.
		*/
		bool checkUTF8() const;

		/*!
		* @brief Return a hash based on the String content
		* @return A 64-bit value
		*/
		u64 hash() const;

		//! Serialize a String parameter
		String& operator<<(const String& value);
		//! Serialize a const char* parameter
		String& operator<<(const char* value);
		//! Serialize an u64 parameter
		String& operator<<(u64 value);
		//! Serialize an u32 parameter
		String& operator<<(u32 value);
		//! Serialize an u16 parameter
		String& operator<<(u16 value);
		//! Serialize an u8 parameter
		String& operator<<(u8 value);
		//! Serialize an i64 parameter
		String& operator<<(i64 value);
		//! Serialize an i32 parameter
		String& operator<<(i32 value);
		//! Serialize an i16 parameter
		String& operator<<(i16 value);
		//! Serialize an i8 parameter
		String& operator<<(i8 value);
		//! Serialize a f64 parameter
		String& operator<<(f64 value);
		//! Serialize a f32 parameter
		String& operator<<(f32 value);
		//! Serialize a bool parameter
		String& operator<<(bool value);

		/*!
		* @brief Construct a String from multiple parameters
		* It requires that every argument overload at least String operator<<()
		* @param Args every arguments
		*/
		template<typename...Args>
		static String join(Args...args)
		{
			String str;
			str._join(args...);
			return str;
		}

	private:
		template<typename T>
		void _join(T value)
		{
			*this << value;
		}

		template<typename T, typename...Args>
		void _join(T value, Args...args)
		{
			*this << value;
			_join(args...);
		}

		u32 m_charcount;
		u32 m_memsize;
		char* m_str;
	};

	/*!
	* @brief Create a new String from a String instance and a single character
	* @param str Base String
	* @param other Single character to be appended
	* @return A new String
	* @code
		String a = "hell";
		char b = 'o';
		String c = a + b; // "hello"
	* @endcode
	*/
	MUON_API String operator+(const String& str, const char other);

	/*!
	* @brief Create a new String from a String instance and a raw character array
	* @param str Base String
	* @param other Raw array of characters to be appended
	* @return A new String
	* @code
		String a = "he";
		char b[] = "llo";
		String c = a + b; // "hello"
	* @endcode
	*/
	MUON_API String operator+(const String& str, const char* other);

	/*!
	* @brief Create a new String from two String instances
	* @param str Base String
	* @param other Other String instance
	* @return A new String
	* @code
		String a = "he";
		String b = "llo";
		String c = a + b; // "hello"
	* @endcode
	*/
	MUON_API String operator+(const String& str, const String& other);

	/*!
	* @brief Create a new String from a raw character array and a String instance
	* @param str Base raw character array
	* @param other String instance to be appended
	* @return A new String
	* @code
		char a[] = "he";
		String b = "llo";
		String c = a + b; // "hello"
	* @endcode
	*/
	MUON_API String operator+(const char* str, const String& other);


	MUON_API bool operator==(const String& str, const String& other);
	MUON_API bool operator!=(const String& str, const String& other);
	MUON_API bool operator<(const String& str, const String& other);
	MUON_API bool operator<=(const String& str, const String& other);
	MUON_API bool operator>(const String& str, const String& other);
	MUON_API bool operator>=(const String& str, const String& other);


	MUON_API system::ILogImpl& operator<<(system::ILogImpl& log, const String& str);
}

MUON_TRAITS_DECL(m::String)
MUON_META_USEPOINTER(m::String);

namespace std
{
	//Return address of first char
	template<> struct hash<m::String>
	{
		m::u32 operator()(const m::String& str) const
		{
			return (m::u32)str.hash();
		}
	};
}

#endif
