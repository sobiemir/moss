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
 * Source file for "Hash" module, Murmur family...
 * License: MIT, see LICENSE file for details
 *
 * Algorithms:
 * - Murmur  [Public Domain]
 * - Murmur2 [Public Domain]
 * - Murmur3 [Public Domain]
 */

#include "../../inc/hash.h"

#define MSN_H32M3_1 0xCC9E2D51
#define MSN_H32M3_2 0x1B873593
#define MSN_H32M3_3 0xE6546B64
#define MSN_H32M2   0x5BD1E995
#define MSN_H64M2   0xC6A4A7935BD1E995ull
#define MSN_H32M1   0xC6A4A793

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur3( const void *data, size_t length )
{
    uint32_t hash = MSD_HASH_SEED,
             ch4b;
    size_t   iter;

    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    // murmur3 pobiera porcjami 4 bajtowymi
    for( iter = length >> 2; iter--; )
        ch4b  = *cdat++,
        ch4b *= MSN_H32M3_1,
        ch4b  = MSX_ROTL32(ch4b, 15),
        ch4b *= MSN_H32M3_2,
        hash ^= ch4b,
        hash  = MSX_ROTL32(hash, 13),
        hash += (hash << 2) + MSN_H32M3_3;

    adat = (const uint8_t*)cdat;
    ch4b = 0;

    // konsumpcja pozostałych wartości
    switch( length & 3 )
    {
        case 3: ch4b |= adat[2] << 16;
        case 2: ch4b |= adat[1] << 8;
        case 1: ch4b |= adat[0];
                ch4b *= MSN_H32M3_1;
                ch4b  = MSX_ROTL32(ch4b, 15);
                ch4b *= MSN_H32M3_2;
                hash ^= ch4b;
    }

    // trochę magii
    hash ^= length;
    hash ^= hash >> 16;
    hash *= 0x85EBCA6B;
    hash ^= hash >> 13;
    hash *= 0xC2B2AE35;
    hash ^= hash >> 16;

    // i wynik gotowy
    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur2( const void *data, size_t length )
{
    uint32_t hash = MSD_HASH_SEED ^ length,
             ch4b;

    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    for( ; length > 3; length -= 4 )
        ch4b  = *cdat++,
        ch4b *= MSN_H32M2,
        ch4b ^= ch4b >> 24,
        ch4b *= MSN_H32M2,
        hash *= MSN_H32M2,
        hash ^= ch4b;
    
    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 3: hash ^= adat[2] << 16;
        case 2: hash ^= adat[1] << 8;
        case 1: hash ^= adat[0];
                hash *= MSN_H32M2;
    }

    hash ^= hash >> 13;
    hash *= MSN_H32M2;
    hash ^= hash >> 15;
    
    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur( const void *data, size_t length )
{
    uint32_t hash = MSD_HASH_SEED ^ (length * MSN_H32M1),
             ch4b;
    
    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    for( ; length > 3; length -= 4 )
        hash += *cdat++,
        hash *= MSN_H32M1,
        hash ^= hash >> 16;

    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 3: hash += adat[2] << 16;
        case 2: hash += adat[1] << 8;
        case 1: hash += adat[0];
                hash *= MSN_H32M1;
                hash ^= hash >> 16;
    }
    
    hash *= MSN_H32M1;
    hash ^= hash >> 10;
    hash *= MSN_H32M1;
    hash ^= hash >> 17;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_64_murmur2( const void *data, size_t length )
{
    uint64_t hash = MSD_HASH_SEED ^ (length * MSN_H64M2),
             ch4b;
    
    const uint64_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    for( ; length > 7; length -= 8 )
        ch4b  = *cdat++,
        ch4b *= MSN_H64M2,
        ch4b ^= ch4b >> 47,
        ch4b *= MSN_H64M2,
        hash ^= ch4b,
        hash *= MSN_H64M2;

    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 7: hash ^= (uint64_t)adat[6] << 48;
        case 6: hash ^= (uint64_t)adat[5] << 40;
        case 5: hash ^= (uint64_t)adat[4] << 32;
        case 4: hash ^= (uint64_t)adat[3] << 24;
        case 3: hash ^= (uint64_t)adat[2] << 16;
        case 2: hash ^= (uint64_t)adat[1] << 8;
        case 1: hash ^= (uint64_t)adat[0];
                hash *= MSN_H64M2;
    }
    
    hash ^= hash >> 47;
    hash *= MSN_H64M2;
    hash ^= hash >> 47;

    return hash;
}

#ifdef MSD_HASH_MBS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_murmur3( const char *data )
{
    uint32_t hash = MSD_HASH_SEED,
             ch4b;
    size_t   nbit,
             slen = 0;

    const uint32_t *cdat = (const uint32_t*)data;

    assert( data );

    while( ch4b = *cdat++, !(nbit = MSX_FIND8B0IN32B(ch4b)) )
        ch4b *= MSN_H32M3_1,
        ch4b  = MSX_ROTL32( ch4b, 15 ),
        ch4b *= MSN_H32M3_2,
        hash ^= ch4b,
        hash  = MSX_ROTL32( hash, 13 ),
        hash += (hash << 2) + MSN_H32M3_3,
        slen += 4;

    // jako że ostatnie 4 bajty zostały już pobrane, nałóż na nie odpowiednią maskę
    if( nbit > 1 )
        ch4b &= 0xFFFFFFFF >> ((5 - nbit) << 3),
        ch4b *= MSN_H32M3_1,
        ch4b  = MSX_ROTL32( ch4b, 15 ),
        ch4b *= MSN_H32M3_2,
        hash ^= ch4b,
        slen += nbit - 1;

    // trochę magii
    hash ^= slen;
    hash ^= hash >> 16;
    hash *= 0x85EBCA6B;
    hash ^= hash >> 13;
    hash *= 0xC2B2AE35;
    hash ^= hash >> 16;

    // i wynik gotowy
    return hash;
}

#endif
#ifdef MSD_HASH_WCS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_murmur3( const wchar_t *data )
{
    uint32_t hash = MSD_HASH_SEED,
             ch4b;
    size_t   slen = 0;

    const uint32_t *cdat = (const uint32_t*)data;

    assert( data );

    if( sizeof(wchar_t) == 2 )
        while( (ch4b = *cdat++) )
        {
            // sprawdź czy jeden z indeksów będzie równy 0
            if( !(ch4b & 0x0000FFFF) )
                break;
            // jeżeli to będzie ten drugi, nałóż maskę i licz...
            if( !(ch4b & 0xFFFF0000) )
            {
                ch4b &= 0xFFFF0000 >> 16;
                ch4b *= MSN_H32M3_1;
                ch4b  = MSX_ROTL32( ch4b, 15 );
                ch4b *= MSN_H32M3_2;
                hash ^= ch4b;
                slen += 2;
                break;
            }
            // rób swoje
            ch4b *= MSN_H32M3_1;
            ch4b  = MSX_ROTL32( ch4b, 15 );
            ch4b *= MSN_H32M3_2;
            hash ^= ch4b;
            hash  = MSX_ROTL32( hash, 13 );
            hash += (hash << 2) + MSN_H32M3_3;
            slen += 4;
        }
    // wchar_t jest 4 bajtowy, zajmuje całą porcję
    else if( sizeof(wchar_t) == 4 )
        while( (ch4b = *cdat++) )
            ch4b *= MSN_H32M3_1,
            ch4b  = MSX_ROTL32( ch4b, 15 ),
            ch4b *= MSN_H32M3_2,
            hash ^= ch4b,
            hash  = MSX_ROTL32( hash, 13 ),
            hash += (hash << 2) + MSN_H32M3_3,
            slen += 4;

    // trochę magii
    hash ^= slen;
    hash ^= hash >> 16;
    hash *= 0x85EBCA6B;
    hash ^= hash >> 13;
    hash *= 0xC2B2AE35;
    hash ^= hash >> 16;

    // i wynik gotowy
    return hash;
}

#endif
