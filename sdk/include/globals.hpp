#ifndef WPP_GLOBAL_H
#define WPP_GLOBAL_H

#include "wppstdint.hpp"

namespace wpp {
namespace how {

const int tMaxInt = 0x7FFFFFFF;
const int tMinInt = - tMaxInt - 1;
const uint32_t tMaxUInt32 = 0xFFFFFFFFu;

const int KB = 1024;
const int MB = KB * KB;
const int GB = KB * KB * KB;

const int tCharSize     = sizeof(char);     
const int tShortSize    = sizeof(short);    
const int tIntSize      = sizeof(int);       
const int tDoubleSize   = sizeof(double);    
const int tIntptrSize   = sizeof(intptr_t);  
const int tPointerSize  = sizeof(void*);    

#if WPP_HOST_ARCH_64_BIT
const int tPointerSizeLog2 = 3;
const intptr_t tIntptrSignBit = V8_INT64_C(0x8000000000000000);
const uintptr_t tUintptrAllBitsSet = V8_UINT64_C(0xFFFFFFFFFFFFFFFF);
#else
const int tPointerSizeLog2 = 2;
const intptr_t tIntptrSignBit = 0x80000000;
const uintptr_t tUintptrAllBitsSet = 0xFFFFFFFFu;
#endif

const int tBitsPerByte = 8;
const int tBitsPerByteLog2 = 3;
const int tBitsPerPointer = tPointerSize * tBitsPerByte;
const int tBitsPerInt = tIntSize * tBitsPerByte;

// IEEE 754 single precision floating point number bit layout.
const uint32_t tBinary32SignMask = 0x80000000u;
const uint32_t tBinary32ExponentMask = 0x7f800000u;
const uint32_t tBinary32MantissaMask = 0x007fffffu;
const int tBinary32ExponentBias = 127;
const int tBinary32MaxExponent  = 0xFE;
const int tBinary32MinExponent  = 0x01;
const int tBinary32MantissaBits = 23;
const int tBinary32ExponentShift = 23;

// ASCII/UC16 constants
// Code-point values in Unicode 4.0 are 21 bits wide.
typedef uint16_t uc16;
typedef int32_t uc32;
const int tASCIISize    = tCharSize;
const int tUC16Size     = sizeof(uc16);      
const uc32 tMaxAsciiCharCode = 0x7f;
const uint32_t tMaxAsciiCharCodeU = 0x7fu;

}} //namespace wpp::how

#endif //WPP_GLOBAL_H

