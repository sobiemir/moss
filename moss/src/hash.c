/*
 * Moss Library >> http://moss.aculo.pl
 *
 *    /'\_/`\                           
 *   /\      \    ___     ____    ____  
 *   \ \ \__\ \  / __`\  /',__\  /',__\ 
 *    \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *     \ \_\\ \_\ \____/\/\____/\/\____/
 *      \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 * Template file for "Hash" module...
 */
#include "../inc/hash.h"

#define MSD_HASH_LOOP_UNROLL 4
#define MSD_HASH_MURMUR_SEED 1234

#define MSX_LOOP_UNROLL_ONELINE_MACRONUM(macro, count) MSX_LOOP_UNROLL_ONELINE(macro, count)
#define MSX_LOOP_UNROLL_ONELINE(macro, count) __MSX_LOOP_UNROLL_ONELINE_ ## count ( macro )

#define __MSX_LOOP_UNROLL_ONELINE_16(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_15(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_15(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_14(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_14(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_13(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_13(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_12(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_12(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_11(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_11(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_10(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_10(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_9 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_9(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_8 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_8(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_7 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_7(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_6 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_6(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_5 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_5(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_4 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_4(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_3 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_3(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_2 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_2(_Macro)  _Macro, _Macro

#define MSX_CASE_UNROLL_NOBREAK_MACRONUM(macro, count) MSX_CASE_UNROLL_NOBREAK(macro, count)
#define MSX_CASE_UNROLL_NOBREAK(macro, count) __MSX_CASE_UNROLL_NOBREAK_ ## count ( macro )

#define __MSX_CASE_UNROLL_NOBREAK_16(_Macro) case 15: _Macro; __MSX_CASE_UNROLL_NOBREAK_15(_Macro )
#define __MSX_CASE_UNROLL_NOBREAK_15(_Macro) case 14: _Macro; __MSX_CASE_UNROLL_NOBREAK_14(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_14(_Macro) case 13: _Macro; __MSX_CASE_UNROLL_NOBREAK_13(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_13(_Macro) case 12: _Macro; __MSX_CASE_UNROLL_NOBREAK_12(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_12(_Macro) case 11: _Macro; __MSX_CASE_UNROLL_NOBREAK_11(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_11(_Macro) case 10: _Macro; __MSX_CASE_UNROLL_NOBREAK_10(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_10(_Macro) case 9:  _Macro; __MSX_CASE_UNROLL_NOBREAK_9(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_9(_Macro)  case 8:  _Macro; __MSX_CASE_UNROLL_NOBREAK_8(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_8(_Macro)  case 7:  _Macro; __MSX_CASE_UNROLL_NOBREAK_7(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_7(_Macro)  case 6:  _Macro; __MSX_CASE_UNROLL_NOBREAK_6(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_6(_Macro)  case 5:  _Macro; __MSX_CASE_UNROLL_NOBREAK_5(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_5(_Macro)  case 4:  _Macro; __MSX_CASE_UNROLL_NOBREAK_4(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_4(_Macro)  case 3:  _Macro; __MSX_CASE_UNROLL_NOBREAK_3(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_3(_Macro)  case 2:  _Macro; __MSX_CASE_UNROLL_NOBREAK_2(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_2(_Macro)  case 1:  _Macro

/**
 * DJB2 K33
 * http://www.cse.yorku.ca/~oz/hash.html#djb2
 */
/* ================================================================================================================== */

uint32_t ms_hash_32_djb2( const void *data, size_t length )
{
    uint32_t    hash = 5381;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash = ((hash << 5) + hash) + *cdat++;

    return hash;
}

/**
 * DJB2A K33
 * http://www.cse.yorku.ca/~oz/hash.html#djb2
 */
/* ================================================================================================================== */

uint32_t ms_hash_32_djb2a( const void *data, size_t length )
{
    uint32_t    hash = 5381;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash = ((hash << 5) + hash) ^ (uint32_t)*cdat++;

    return hash;
}

/**
 * SDBM
 * http://www.cse.yorku.ca/~oz/hash.html#sdbm
 */
/* ================================================================================================================== */

uint32_t ms_hash_32_sdbm( const void *data, size_t length )
{
	uint32_t    hash = 0;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash = *cdat++ + (hash << 6) + (hash << 16) - hash;

	return hash;
}

/**
 * Jenkins One At Time
 * https://en.wikipedia.org/wiki/Jenkins_hash_function
 */
/* ================================================================================================================== */

uint32_t ms_hash_32_joaat( const void *data, size_t length )
{
	uint32_t    hash = 0;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash += *cdat++, hash += hash << 10, hash ^= hash >> 6;

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

    return hash;
}

/**
 * FNV-1 32BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
 */
 /* ================================================================================================================== */

uint32_t ms_hash_32_fnv1( const void *data, size_t length )
{
    uint32_t    hash = 2166136261u;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash += (hash << 1u) + (hash << 4u) + (hash << 7u) +
                (hash << 8u) + (hash << 24u),
        hash ^= *cdat++;

    return hash;
}

/**
 * FNV-1 64BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
 */
 /* ================================================================================================================== */

uint64_t ms_hash_64_fnv1( const void *data, size_t length )
{
    uint64_t    hash = 14695981039346656037ull;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
        hash ^= *cdat++;

    return hash;
}

/**
 * FNV-1A 32BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
 */
 /* ================================================================================================================== */

uint32_t ms_hash_32_fnv1a( const void *data, size_t length )
{
    uint32_t    hash = 2166136261u;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash ^= *cdat++,
        hash += (hash << 1u) + (hash << 4u) + (hash << 7u) +
                (hash << 8u) + (hash << 24u);

    return hash;
}

/**
 * FNV-1A 64BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
 */
 /* ================================================================================================================== */

uint64_t ms_hash_64_fnv1a( const void *data, size_t length )
{
    uint64_t    hash = 14695981039346656037ull;
    const char *cdat = data;

    for( ; length > 0; --length )
        hash ^= *cdat++,
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull);

    return hash;
}

/**
 * MurmurHash3
 * https://en.wikipedia.org/wiki/MurmurHash
 */
 /* ================================================================================================================== */

uint32_t ms_hash_32_murmur3( const void *data, size_t length )
{
    uint32_t hash = MSD_HASH_MURMUR_SEED,
             ch4b;
    size_t   iter;

    const uint32_t *cdat = data;
    const char     *adat;

    // murmur3 pobiera porcjami 4 bajtowymi
    for( iter = length >> 2; iter > 0; --iter )
        ch4b  = *cdat++ * 0xcc9e2d51,
        hash ^= (ROTL32(ch4b, 15)) * 0x1b873593,
        hash  = ROTL32(hash, 13),
        hash += (hash << 2) + 0xe6546b64;

    adat = (const char*)cdat;
    ch4b = 0;

    // konsumpcja pozostałych wartości
    switch( length & 3 )
    {
        case 3: ch4b |= adat[2] << 16;
        case 2: ch4b |= adat[1] << 8;
        case 1: ch4b |= adat[0],
                ch4b *= 0xcc9e2d51,
                hash ^= (ROTL32(ch4b, 15)) * 0x1b873593;
    }

    // trochę magii
    hash ^= length;
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;

    // i wynik gotowy
    return hash;
}
