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
 * Source file for "Hash" module, FNV1 section...
 */

#include "../../inc/hash.h"

/**
 * Algorytm FNV-1  - Public Domain
 * Algorytm FNV-1a - Public Domain
 */

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

uint32_t ms_hash_32_fnv1_mbs( const char *data )
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

uint32_t ms_hash_32_fnv1_wcs( const wchar_t *data )
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

uint32_t ms_hash_32_fnv1a_mbs( const char *data )
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

uint32_t ms_hash_32_fnv1a_wcs( const wchar_t *data )
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

uint64_t ms_hash_64_fnv1_mbs( const char *data )
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

uint64_t ms_hash_64_fnv1_wcs( const wchar_t *data )
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

/* ================================================================================================================== */

uint64_t ms_hash_64_fnv1a_mbs( const char *data )
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

/* ================================================================================================================== */

uint64_t ms_hash_64_fnv1a_wcs( const wchar_t *data )
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
