#pragma once
#include <cstdint>

// Signed base types.
typedef	std::int8_t				int8;		// 8-bit  signed.
typedef std::int16_t			int16;		// 16-bit signed.
typedef std::int32_t			int32;		// 32-bit signed.
typedef std::int64_t			int64;		// 64-bit signed.

// Unsigned base types.
typedef std::uint8_t 			uint8;		// 8-bit  unsigned.
typedef std::uint16_t			uint16;		// 16-bit unsigned.
typedef std::uint32_t			uint32;		// 32-bit unsigned.
typedef std::uint64_t			uint64;		// 64-bit unsigned.

// Character types.
typedef char					ANSICHAR;	// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
typedef wchar_t					WIDECHAR;	// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
typedef std::uint8_t			CHAR8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef std::uint16_t			CHAR16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef std::uint32_t			CHAR32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef WIDECHAR				TCHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.

