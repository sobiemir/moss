/*
 *  Moss Library >>> http://moss.aculo.pl
 *
 *     /'\_/`\                           
 *    /\      \    ___     ____    ____  
 *    \ \ \__\ \  / __`\  /',__\  /',__\ 
 *     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *      \ \_\\ \_\ \____/\/\____/\/\____/
 *       \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 *  Source file for "Hash" module, FNV algorithms [Public Domain].
 *  See LICENSE file for copyright information.
 */

#include "../../inc/hash.h"

/* ================================================================================================================== */

uint32_t ms_hash_32_fnv1( const void *data, size_t length )
{
    uint32_t    hash = 2166136261u;
    const char *cdat = data;

    assert( data );

    for( ; length; --length )
        hash += (hash << 1) + (hash << 4) + (hash << 7) +
                (hash << 8) + (hash << 24),
        hash ^= *cdat++;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_fnv1a( const void *data, size_t length )
{
    uint32_t    hash = 2166136261u;
    const char *cdat = data;

    assert( data );

    for( ; length; --length )
        hash ^= *cdat++,
        hash += (hash << 1) + (hash << 4) + (hash << 7) +
                (hash << 8) + (hash << 24);

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_64_fnv1( const void *data, size_t length )
{
    uint64_t    hash = 14695981039346656037ull;
    const char *cdat = data;

    assert( data );

    for( ; length > 0; --length )
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
        hash ^= *cdat++;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_64_fnv1a( const void *data, size_t length )
{
    uint64_t    hash = 14695981039346656037ull;
    const char *cdat = data;

    assert( data );

    for( ; length > 0; --length )
        hash ^= *cdat++,
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull);

    return hash;
}

#ifdef MSD_HASH_MBS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_fnv1( const char *data )
{
    uint32_t hash = 2166136261u;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash += (hash << 1) + (hash << 4) + (hash << 7) +
                (hash << 8) + (hash << 24),
        hash ^= c;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_fnv1a( const char *data )
{
    uint32_t hash = 2166136261u;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash ^= c,
        hash += (hash << 1) + (hash << 4) + (hash << 7) +
                (hash << 8) + (hash << 24);

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_mbs_64_fnv1( const char *data )
{
    uint64_t hash = 14695981039346656037ull;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
        hash ^= c;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_mbs_64_fnv1a( const char *data )
{
    uint64_t hash = 14695981039346656037ull;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash ^= c,
        hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                (hash << 7ull) + (hash << 8ull) + (hash << 40ull);

    return hash;
}

#endif
#ifdef MSD_HASH_WCS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_fnv1( const wchar_t *data )
{
    uint32_t hash = 2166136261u;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0x00FF),
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF00) >> 8;

    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0x000000FF),
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0x0000FF00) >> 8,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0x00FF0000) >> 16,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF000000) >> 24;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_fnv1a( const wchar_t *data )
{
    uint32_t hash = 2166136261u;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash ^= (uint32_t)(c & 0x00FF),
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF00) >> 8,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24);

    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash ^= (uint32_t)(c & 0x00FF),
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF00) >> 8,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF00) >> 16,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24),
            hash ^= (uint32_t)(c & 0xFF00) >> 24,
            hash += (hash << 1) + (hash << 4) + (hash << 7) +
                    (hash << 8) + (hash << 24);

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_wcs_64_fnv1( const wchar_t *data )
{
    uint64_t hash = 14695981039346656037ull;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x00FF),
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0xFF00) >> 8;

    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x000000FF),
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x0000FF00) >> 8,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x00FF0000) >> 16,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0xFF000000) >> 24;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_wcs_64_fnv1a( const wchar_t *data )
{
    uint64_t hash = 14695981039346656037ull;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash ^= (uint32_t)(c & 0x00FF),
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0xFF00) >> 8,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull);

    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash ^= (uint32_t)(c & 0x000000FF),
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x0000FF00) >> 8,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0x00FF0000) >> 16,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull),
            hash ^= (uint32_t)(c & 0xFF000000) >> 24,
            hash += (hash << 1ull) + (hash << 4ull) + (hash << 5ull) +
                    (hash << 7ull) + (hash << 8ull) + (hash << 40ull);

    return hash;
}

#endif
