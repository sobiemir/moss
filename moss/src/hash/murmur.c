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
 * Source file for "Hash" module, MURMUR section...
 */

#include "../../inc/hash.h"

/**
 * Algorytm Murmur  - Public Domain
 * Algorytm Murmur2 - Public Domain
 * Algorytm Murmur3 - Public Domain
 */

#define MSN_H32M3_1 0xCC9E2D51
#define MSN_H32M3_2 0x1B873593
#define MSN_H32M3_3 0xE6546B64

/* ================================================================================================================== */

inline static size_t msf_hash_find_32_0byte( uint32_t bits )
{
    if( !(bits & 0x000000FF) )
        return 1;
    if( !(bits & 0x0000FF00) )
        return 2;
    if( !(bits & 0x00FF0000) )
        return 3;
    if( !(bits & 0xFF000000) )
        return 4;

    return 0;
}

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
    for( iter = length >> 2; iter; --iter )
        ch4b  = *cdat++,
        ch4b *= MSN_H32M3_1,
        ch4b  = ROTL32(ch4b, 15),
        ch4b *= MSN_H32M3_2,
        hash ^= ch4b,
        hash  = ROTL32(hash, 13),
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
                ch4b  = ROTL32(ch4b, 15);
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

uint32_t ms_hash_32_murmur3_mbs( const char *data )
{
    uint32_t hash = MSD_HASH_SEED,
             ch4b;
    size_t   nbit,
             slen;

    const uint32_t *cdat = (const uint32_t*)data;

    assert( data );

    while( !(nbit = msf_hash_find_32_0byte((ch4b = *cdat++))) )
        ch4b *= MSN_H32M3_1,
        ch4b  = ROTL32( ch4b, 15 ),
        ch4b *= MSN_H32M3_2,
        hash ^= ch4b,
        hash  = ROTL32( hash, 13 ),
        hash += (hash << 2) + MSN_H32M3_3,
        slen += 4;

    // jako że ostatnie 4 bajty zostały już pobrane, nałóż na nie odpowiednią maskę
    if( nbit > 1 )
        ch4b &= 0xFFFFFFFF >> ((5 - nbit) << 3),
        ch4b *= MSN_H32M3_1,
        ch4b  = ROTL32( ch4b, 15 ),
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

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur3_wcs( const wchar_t *data )
{
    uint32_t hash = MSD_HASH_SEED,
             ch4b;
    size_t   slen;

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
                ch4b  = ROTL32( ch4b, 15 );
                ch4b *= MSN_H32M3_2;
                hash ^= ch4b;
                slen += 2;
                break;
            }
            // rób swoje
            ch4b *= MSN_H32M3_1;
            ch4b  = ROTL32( ch4b, 15 );
            ch4b *= MSN_H32M3_2;
            hash ^= ch4b;
            hash  = ROTL32( hash, 13 );
            hash += (hash << 2) + MSN_H32M3_3;
            slen += 4;
        }
    // wchar_t jest 4 bajtowy, zajmuje całą porcję
    else if( sizeof(wchar_t) == 4 )
        while( (ch4b = *cdat++) )
            ch4b *= MSN_H32M3_1,
            ch4b  = ROTL32( ch4b, 15 ),
            ch4b *= MSN_H32M3_2,
            hash ^= ch4b,
            hash  = ROTL32( hash, 13 ),
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
