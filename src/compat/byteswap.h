// Copyright (c) 2014-2016 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_COMPAT_BYTESWAP_H
#define FLEETCREDITS_COMPAT_BYTESWAP_H

#if defined(HAVE_CONFIG_H)
#include "config/fleetcredits-config.h"
#endif

#include <stdint.h>

// On Linux with glibc, <endian.h> provides bswap functions via <byteswap.h>
// We need to be careful about include order to avoid conflicts
#if defined(HAVE_BYTESWAP_H) && !defined(HAVE_ENDIAN_H) && !defined(HAVE_SYS_ENDIAN_H)
#include <byteswap.h>
#endif

#if defined(__APPLE__)

#if !defined(bswap_16)

// Mac OS X / Darwin features; we include a check for bswap_16 because if it is already defined, protobuf has
// defined these macros for us already; if it isn't, we do it ourselves. In either case, we get the exact same
// result regardless which path was taken
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#endif // !defined(bswap_16)

#else
// Non-Mac OS X / non-Darwin

// On systems with <endian.h>, use __builtin_bswap* directly to avoid conflicts
#if defined(HAVE_ENDIAN_H) || defined(HAVE_SYS_ENDIAN_H)
// <endian.h> includes <bits/byteswap.h> which defines __bswap_* but not bswap_*
// Use __builtin_bswap* directly to avoid conflicts
#if !defined(bswap_16)
inline uint16_t bswap_16(uint16_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap16(x);
#else
    return (x >> 8) | ((x & 0x00ff) << 8);
#endif
}
#endif // !defined(bswap_16)

#if !defined(bswap_32)
inline uint32_t bswap_32(uint32_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap32(x);
#else
    return (((x & 0xff000000U) >> 24) | ((x & 0x00ff0000U) >>  8) |
            ((x & 0x0000ff00U) <<  8) | ((x & 0x000000ffU) << 24));
#endif
}
#endif // !defined(bswap_32)

#if !defined(bswap_64)
inline uint64_t bswap_64(uint64_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap64(x);
#else
     return (((x & 0xff00000000000000ull) >> 56)
          | ((x & 0x00ff000000000000ull) >> 40)
          | ((x & 0x0000ff0000000000ull) >> 24)
          | ((x & 0x000000ff00000000ull) >> 8)
          | ((x & 0x00000000ff000000ull) << 8)
          | ((x & 0x0000000000ff0000ull) << 24)
          | ((x & 0x000000000000ff00ull) << 40)
          | ((x & 0x00000000000000ffull) << 56));
#endif
}
#endif // !defined(bswap_64)
#else
// Systems without <endian.h>
#if !defined(bswap_16) && HAVE_DECL_BSWAP_16 == 0
inline uint16_t bswap_16(uint16_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap16(x);
#else
    return (x >> 8) | ((x & 0x00ff) << 8);
#endif
}
#endif // !defined(bswap_16) && HAVE_DECL_BSWAP16

#if !defined(bswap_32) && HAVE_DECL_BSWAP_32 == 0
inline uint32_t bswap_32(uint32_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap32(x);
#else
    return (((x & 0xff000000U) >> 24) | ((x & 0x00ff0000U) >>  8) |
            ((x & 0x0000ff00U) <<  8) | ((x & 0x000000ffU) << 24));
#endif
}
#endif // !defined(bswap_32) && HAVE_DECL_BSWAP32

#if !defined(bswap_64) && HAVE_DECL_BSWAP_64 == 0
inline uint64_t bswap_64(uint64_t x)
{
#if defined(__GNUC__)
    return __builtin_bswap64(x);
#else
     return (((x & 0xff00000000000000ull) >> 56)
          | ((x & 0x00ff000000000000ull) >> 40)
          | ((x & 0x0000ff0000000000ull) >> 24)
          | ((x & 0x000000ff00000000ull) >> 8)
          | ((x & 0x00000000ff000000ull) << 8)
          | ((x & 0x0000000000ff0000ull) << 24)
          | ((x & 0x000000000000ff00ull) << 40)
          | ((x & 0x00000000000000ffull) << 56));
#endif
}
#endif // !defined(bswap_64) && HAVE_DECL_BSWAP64
#endif // HAVE_ENDIAN_H || HAVE_SYS_ENDIAN_H

#endif // defined(__APPLE__)

#endif // FLEETCREDITS_COMPAT_BYTESWAP_H
