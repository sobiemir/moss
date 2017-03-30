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
 * Source file for "Hash" module...
 */
#include "../inc/hash.h"

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
        ch4b *= 0xcc9e2d51,
        ch4b  = ROTL32(ch4b, 15),
        ch4b *= 0x1b873593,
        hash ^= ch4b,
        hash  = ROTL32(hash, 13),
        hash += (hash << 2) + 0xe6546b64;

    adat = (const uint8_t*)cdat;
    ch4b = 0;

    // konsumpcja pozostałych wartości
    switch( length & 3 )
    {
        case 3: ch4b |= adat[2] << 16;
        case 2: ch4b |= adat[1] << 8;
        case 1: ch4b |= adat[0];
                ch4b *= 0xcc9e2d51;
                ch4b  = ROTL32(ch4b, 15);
                ch4b *= 0x1b873593;
                hash ^= ch4b;
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
        ch4b *= 0x5bd1e995,
        ch4b ^= ch4b >> 24,
        ch4b *= 0x5bd1e995,
        hash *= 0x5bd1e995,
        hash ^= ch4b;
    
    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 3: hash ^= adat[2] << 16;
        case 2: hash ^= adat[1] << 8;
        case 1: hash ^= adat[0];
                hash *= 0x5bd1e995;
    }

    hash ^= hash >> 13;
    hash *= 0x5bd1e995;
    hash ^= hash >> 15;
    
    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur( const void *data, size_t length )
{
    uint32_t hash = MSD_HASH_SEED ^ (length * 0xc6a4a793),
             ch4b;
    
    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    for( ; length > 3; length -= 4 )
        hash += *cdat++,
        hash *= 0xc6a4a793,
        hash ^= hash >> 16;

    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 3: hash += adat[2] << 16;
        case 2: hash += adat[1] << 8;
        case 1: hash += adat[0];
                hash *= 0xc6a4a793;
                hash ^= hash >> 16;
    }
    
    hash *= 0xc6a4a793;
    hash ^= hash >> 10;
    hash *= 0xc6a4a793;
    hash ^= hash >> 17;

    return hash;
}

/* ================================================================================================================== */

#define MSH_32XXH_PRIME1 2654435761u
#define MSH_32XXH_PRIME2 2246822519u
#define MSH_32XXH_PRIME3 3266489917u
#define MSH_32XXH_PRIME4 668265263u
#define MSH_32XXH_PRIME5 374761393u

uint32_t ms_hash_32_xxhash( const void *data, size_t length )
{
    uint32_t hash = 0;

    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    /* jeżeli wartość jest większa niż 15 */
    if( length > 0xF )
    {
        uint32_t val1 = MSD_HASH_SEED + MSH_32XXH_PRIME1 + MSH_32XXH_PRIME2,
                 val2 = MSD_HASH_SEED + MSH_32XXH_PRIME2,
                 val3 = MSD_HASH_SEED,
                 val4 = MSD_HASH_SEED - MSH_32XXH_PRIME1;
        size_t   iter = length >> 4; /* dziel na 16, będą pobierane 4 porcje po 4 bajty */

        do
            (val1 += *cdat++ * MSH_32XXH_PRIME2), (val1 = ROTL32(val1, 13)), (val1 *= MSH_32XXH_PRIME1),
            (val2 += *cdat++ * MSH_32XXH_PRIME2), (val2 = ROTL32(val2, 13)), (val2 *= MSH_32XXH_PRIME1),
            (val3 += *cdat++ * MSH_32XXH_PRIME2), (val3 = ROTL32(val3, 13)), (val3 *= MSH_32XXH_PRIME1),
            (val4 += *cdat++ * MSH_32XXH_PRIME2), (val4 = ROTL32(val4, 13)), (val4 *= MSH_32XXH_PRIME1);
        while( --iter );

        hash = ROTL32( val1, 1 ) + ROTL32( val2, 7 ) + ROTL32( val3, 12 ) + ROTL32( val4, 18 );
    }
    else
        hash = MSD_HASH_SEED + MSH_32XXH_PRIME5;
    
    hash   += length;
    length &= 0xF;

    /* przejmuj odpadki */
    if( length > 11 )
        hash   += *cdat++ * MSH_32XXH_PRIME3,
        hash    = ROTL32( hash, 17 ) * MSH_32XXH_PRIME4,
        length -= 4;
    if( length > 7 )
        hash   += *cdat++ * MSH_32XXH_PRIME3,
        hash    = ROTL32( hash, 17 ) * MSH_32XXH_PRIME4,
        length -= 4;
    if( length > 3 )
        hash   += *cdat++ * MSH_32XXH_PRIME3,
        hash    = ROTL32( hash, 17 ) * MSH_32XXH_PRIME4,
        length -= 4;

    // length &= 0x3;
    adat = (const uint8_t*)cdat;

    switch( length )
    {
        case 3:
            hash += *adat++ * MSH_32XXH_PRIME5;
            hash  = ROTL32( hash, 11 ) * MSH_32XXH_PRIME1;
        case 2:
            hash += *adat++ * MSH_32XXH_PRIME5;
            hash  = ROTL32( hash, 11 ) * MSH_32XXH_PRIME1;
        case 1:
            hash += *adat++ * MSH_32XXH_PRIME5;
            hash  = ROTL32( hash, 11 ) * MSH_32XXH_PRIME1;
    }

    hash ^= hash >> 15;
    hash *= MSH_32XXH_PRIME2;
    hash ^= hash >> 13;
    hash *= MSH_32XXH_PRIME3;
    hash ^= hash >> 16;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_64_murmur2( const void *data, size_t length )
{
    uint64_t hash = MSD_HASH_SEED ^ (length * 0xc6a4a7935bd1e995ull),
             ch4b;
    
    const uint64_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    for( ; length > 7; length -= 8 )
        ch4b  = *cdat++,
        ch4b *= 0xc6a4a7935bd1e995ull,
        ch4b ^= ch4b >> 47ull,
        ch4b *= 0xc6a4a7935bd1e995ull,
        hash ^= ch4b,
        hash *= 0xc6a4a7935bd1e995ull;

    adat = (const uint8_t*)cdat;
    switch( length )
    {
        case 7: hash ^= (uint64_t)adat[6] << 48ull;
        case 6: hash ^= (uint64_t)adat[5] << 40ull;
        case 5: hash ^= (uint64_t)adat[4] << 32ull;
        case 4: hash ^= (uint64_t)adat[3] << 24ull;
        case 3: hash ^= (uint64_t)adat[2] << 16ull;
        case 2: hash ^= (uint64_t)adat[1] << 8ull;
        case 1: hash ^= (uint64_t)adat[0];
                hash *= 0xc6a4a7935bd1e995ull;
    }
    
    hash ^= hash >> 47ull;
    hash *= 0xc6a4a7935bd1e995ull;
    hash ^= hash >> 47ull;

    return hash;
}

/* ================================================================================================================== */

#define MSH_64XXH_PRIME1 11400714785074694791ull
#define MSH_64XXH_PRIME2 14029467366897019727ull
#define MSH_64XXH_PRIME3 1609587929392839161ull
#define MSH_64XXH_PRIME4 9650029242287828579ull
#define MSH_64XXH_PRIME5 2870177450012600261ull

uint64_t ms_hash_64_xxhash( const void *data, size_t length )
{

    uint64_t hash = 0,
             ch8b;

    const uint64_t *cdat = data;
    const uint8_t  *adat;
    const uint32_t *vdat;

    assert( data );

    /* jeżeli wartość jest większa niż 31 */
    if( length > 0x1F )
    {
        uint64_t val1 = MSD_HASH_SEED + MSH_64XXH_PRIME1 + MSH_64XXH_PRIME2,
                 val2 = MSD_HASH_SEED + MSH_64XXH_PRIME2,
                 val3 = MSD_HASH_SEED,
                 val4 = MSD_HASH_SEED - MSH_64XXH_PRIME1;
        size_t   iter = length >> 5; /* dziel na 32, będą pobierane 4 porcje po 8 bajtów */

        do
            val1 += *cdat++ * MSH_64XXH_PRIME2, val1 = ROTL64(val1, 31), val1 *= MSH_64XXH_PRIME1,
            val2 += *cdat++ * MSH_64XXH_PRIME2, val2 = ROTL64(val2, 31), val2 *= MSH_64XXH_PRIME1,
            val3 += *cdat++ * MSH_64XXH_PRIME2, val3 = ROTL64(val3, 31), val3 *= MSH_64XXH_PRIME1,
            val4 += *cdat++ * MSH_64XXH_PRIME2, val4 = ROTL64(val4, 31), val4 *= MSH_64XXH_PRIME1;
        while( --iter );

        hash  = ROTL64( val1, 1 ) + ROTL64( val2, 7 ) + ROTL64( val3, 12 ) + ROTL64( val4, 18 );
        val1 *= MSH_64XXH_PRIME2;
        val1  = ROTL64(val1, 31);
        val1 *= MSH_64XXH_PRIME1;
        hash ^= val1;
        hash  = hash * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4;
        val2 *= MSH_64XXH_PRIME2;
        val2  = ROTL64(val2, 31);
        val2 *= MSH_64XXH_PRIME1;
        hash ^= val2;
        hash  = hash * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4;
        val3 *= MSH_64XXH_PRIME2;
        val3  = ROTL64(val3, 31);
        val3 *= MSH_64XXH_PRIME1;
        hash ^= val3;
        hash  = hash * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4;
        val4 *= MSH_64XXH_PRIME2;
        val4  = ROTL64(val4, 31);
        val4 *= MSH_64XXH_PRIME1;
        hash ^= val4;
        hash  = hash * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4;
    }
    else
        hash = MSD_HASH_SEED + MSH_64XXH_PRIME5;
    
    hash   += length;
    length &= 0x1F;

    /* przejmuj odpadki */
    if( length > 23 )
        ch8b    = *cdat++ * MSH_64XXH_PRIME2,
        ch8b    = ROTL64( ch8b, 31 ),
        ch8b   *= MSH_64XXH_PRIME1,
        hash   ^= ch8b,
        hash    = ROTL64( hash, 27 ) * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4,
        length -= 8;
    if( length > 15 )
        ch8b    = *cdat++ * MSH_64XXH_PRIME2,
        ch8b    = ROTL64( ch8b, 31 ),
        ch8b   *= MSH_64XXH_PRIME1,
        hash   ^= ch8b,
        hash    = ROTL64( hash, 27 ) * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4,
        length -= 8;
    if( length > 7 )
        ch8b    = *cdat++ * MSH_64XXH_PRIME2,
        ch8b    = ROTL64( ch8b, 31 ),
        ch8b   *= MSH_64XXH_PRIME1,
        hash   ^= ch8b,
        hash    = ROTL64( hash, 27 ) * MSH_64XXH_PRIME1 + MSH_64XXH_PRIME4,
        length -= 8;

    vdat = (const uint32_t*)cdat;
    if( length > 3 )
        hash   ^= *vdat++ * MSH_64XXH_PRIME1,
        hash    = ROTL64( hash, 23 ) * MSH_64XXH_PRIME2 + MSH_64XXH_PRIME3,
        length -= 4;

    // length &= 0x7;
    adat = (const uint8_t*)vdat;

    switch( length )
    {
        case 3:
            hash ^= *adat++ * MSH_64XXH_PRIME5;
            hash  = ROTL64( hash, 11 ) * MSH_64XXH_PRIME1;
        case 2:
            hash ^= *adat++ * MSH_64XXH_PRIME5;
            hash  = ROTL64( hash, 11 ) * MSH_64XXH_PRIME1;
        case 1:
            hash ^= *adat++ * MSH_64XXH_PRIME5;
            hash  = ROTL64( hash, 11 ) * MSH_64XXH_PRIME1;
    }

    hash ^= hash >> 33;
    hash *= MSH_64XXH_PRIME2;
    hash ^= hash >> 29;
    hash *= MSH_64XXH_PRIME3;
    hash ^= hash >> 32;

    return hash;
}
