#ifndef WPP_WPPSTDINT_H
#define WPP_WPPSTDINT_H

#include <stdio.h>

#define TYPE_(ty, oty) typedef oty ty;
#if defined(_WIN32) && !defined(__MINGW32__)

TYPE_(int8_t,	signed char)
TYPE_(uint8_t,	unsigned char)
TYPE_(int16_t,	short)
TYPE_(uint16_t,	unsigned short)
TYPE_(int32_t,	int)
TYPE_(uint32_t,	unsigned int)
TYPE_(int64_t,	__int64)
TYPE_(uint64_t,	unsigned __int64)

#else

#include <stdint.h>

#endif

#endif  // WPP_WPPSTDINT_H