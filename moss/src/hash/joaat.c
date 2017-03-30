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
 * Source file for "Hash" module, JOAAT family...
 * License: MIT, see LICENSE file for details
 *
 * Algorithms:
 * - JOAAT [Public Domain] (Jenkins One At A Time)
 */

#include "../../inc/hash.h"

/* ================================================================================================================== */

uint32_t ms_hash_32_joaat( const void *data, size_t length )
{
	uint32_t    hash = 0;
    const char *cdat = data;

    assert( data );

    for( ; length; --length )
        hash += *cdat++,
        hash += hash << 10,
        hash ^= hash >> 6;

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

    return hash;
}

#ifdef MSD_HASH_MBS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_joaat( const char *data )
{
	uint32_t hash = 0;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash += c,
        hash += hash << 10,
        hash ^= hash >> 6;

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

    return hash;
}

#endif
#ifdef MSD_HASH_WCS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_joaat( const wchar_t *data )
{
	uint32_t hash = 0;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash += (uint32_t)(c & 0x00FF),
            hash += hash << 10,
            hash ^= hash >> 6,

            hash += (uint32_t)(c & 0xFF00) >> 8,
            hash += hash << 10,
            hash ^= hash >> 6;

    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash += (uint32_t)(c & 0x000000FF),
            hash += hash << 10,
            hash ^= hash >> 6,
            
            hash += (uint32_t)(c & 0x0000FF00) >> 8,
            hash += hash << 10,
            hash ^= hash >> 6,

            hash += (uint32_t)(c & 0x00FF0000) >> 16,
            hash += hash << 10,
            hash ^= hash >> 6,

            hash += (uint32_t)(c & 0xFF000000) >> 24,
            hash += hash << 10,
            hash ^= hash >> 6;

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

    return hash;
}

#endif
