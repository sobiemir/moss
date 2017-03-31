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
 * Source file for "Hash" module, DJB family...
 * License: MIT, see LICENSE file for details
 *
 * Algorithms:
 * - DJB2  [Public Domain]
 * - DJB2A [Public Domain]
 */

#include "../../inc/hash.h"

/* ================================================================================================================== */

uint32_t ms_hash_32_djb2( const void *data, size_t length )
{
    uint32_t       hash = 5381;
    const uint8_t *cdat = data;

    assert( data );

    for( ; length; --length )
        hash = ((hash << 5) + hash) + *cdat++;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_djb2a( const void *data, size_t length )
{
    uint32_t       hash = 5381;
    const uint8_t *cdat = data;

    assert( data );

    for( ; length; --length )
        hash = ((hash << 5) + hash) ^ *cdat++;

    return hash;
}

#ifdef MSD_HASH_MBS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_djb2( const char *data )
{
    uint32_t hash = 5381;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash = ((hash << 5) + hash) + c;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_djb2a( const char *data )
{
    uint32_t hash = 5381;
    int      c;

    assert( data );

    while( (c = (uint8_t)*data++) )
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

#endif
#ifdef MSD_HASH_WCS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_djb2( const wchar_t *data )
{
    uint32_t hash = 5381;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0x00FF)     ),
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0xFF00) >> 8);
    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0x000000FF)      ),
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0x0000FF00) >> 8 ),
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0x00FF0000) >> 16),
            hash = ((hash << 5) + hash) + ((uint32_t)(c & 0xFF000000) >> 24);

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_djb2a( const wchar_t *data )
{
    uint32_t hash = 5381;
    wint_t   c;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (c = (uint16_t)*data++) )
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0x00FF)     ),
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0xFF00) >> 8);
    else if( sizeof(wchar_t) == 4 )
        while( (c = (uint32_t)*data++) )
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0x000000FF)      ),
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0x0000FF00) >> 8 ),
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0x00FF0000) >> 16),
            hash = ((hash << 5) + hash) ^ ((uint32_t)(c & 0xFF000000) >> 24);

    return hash;
}

#endif