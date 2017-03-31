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
 * Source file for "Hash" module, XXHASH family...
 * License: MIT, see LICENSE file for details
 *
 * Algorithms:
 * - XXHASH [BSD 2-Clause License]
 */

/*
 *  Functions based on xxHash implementation on BSD license.
 *  Original license:
 *
 *  xxHash - Fast Hash algorithm
 *  Copyright (C) 2012-2016, Yann Collet
 *
 *  BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *  * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following disclaimer
 *  in the documentation and/or other materials provided with the
 *  distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You can contact the author at :
 *  - xxHash homepage: http://www.xxhash.com
 *  - xxHash source repository : https://github.com/Cyan4973/xxHash
 */

#include "../../inc/hash.h"

#define MSN_H32XX_1 2654435761u
#define MSN_H32XX_2 2246822519u
#define MSN_H32XX_3 3266489917u
#define MSN_H32XX_4 668265263u
#define MSN_H32XX_5 374761393u
#define MSN_H64XX_1 11400714785074694791ull
#define MSN_H64XX_2 14029467366897019727ull
#define MSN_H64XX_3 1609587929392839161ull
#define MSN_H64XX_4 9650029242287828579ull
#define MSN_H64XX_5 2870177450012600261ull

/* ================================================================================================================== */

inline static uint64_t msf_hash_mix64_value( uint64_t hash, uint64_t value )
{
    value *= MSN_H64XX_2;
    value  = MSX_ROTL64( value, 31 );
    value *= MSN_H64XX_1;
    hash  ^= value;

    return hash * MSN_H64XX_1 + MSN_H64XX_4;
}

/* ================================================================================================================== */

inline static uint64_t msf_hash_mix64_char64( uint64_t hash, uint64_t c64 )
{
    c64  *= MSN_H64XX_2;
    c64   = MSX_ROTL64( c64, 31 );
    c64  *= MSN_H64XX_1;
    hash ^= c64;

    return MSX_ROTL64( hash, 27 ) * MSN_H64XX_1 + MSN_H64XX_4;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_32s8b0_check( const uint32_t *data )
{
    size_t nbit;

    if( (nbit = MSX_FIND8B0IN32B(data[0])) )
        return nbit;
    if( (nbit = MSX_FIND8B0IN32B(data[1])) )
        return 4 + nbit;
    if( (nbit = MSX_FIND8B0IN32B(data[2])) )
        return 8 + nbit;
    if( (nbit = MSX_FIND8B0IN32B(data[3])) )
        return 12 + nbit;
    
    return 0;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_64s8b0_check( const uint64_t *data )
{
    size_t nbit;

    if( (nbit = MSX_FIND8B0IN64B(data[0])) )
        return nbit;
    if( (nbit = MSX_FIND8B0IN64B(data[1])) )
        return 8 + nbit;
    if( (nbit = MSX_FIND8B0IN64B(data[2])) )
        return 16 + nbit;
    if( (nbit = MSX_FIND8B0IN64B(data[3])) )
        return 24 + nbit;

    return 0;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_64s16b0_check( const uint64_t *data )
{
    size_t nbit;

    if( (nbit = MSX_FIND16B0IN64B(data[0])) )
        return nbit;
    if( (nbit = MSX_FIND16B0IN64B(data[1])) )
        return 4 + nbit;
    if( (nbit = MSX_FIND16B0IN64B(data[2])) )
        return 8 + nbit;
    if( (nbit = MSX_FIND16B0IN64B(data[3])) )
        return 12 + nbit;

    return 0;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_64s32b0_check( const uint64_t *data )
{
    size_t nbit;

    if( (nbit = MSX_FIND32B0IN64B(data[0])) )
        return nbit;
    if( (nbit = MSX_FIND32B0IN64B(data[1])) )
        return 2 + nbit;
    if( (nbit = MSX_FIND32B0IN64B(data[2])) )
        return 4 + nbit;
    if( (nbit = MSX_FIND32B0IN64B(data[3])) )
        return 6 + nbit;

    return 0;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_32s16b0_check( const uint32_t *data )
{
    size_t nbit;

    if( (nbit = MSX_FIND16B0IN32B(data[0])) )
        return nbit;
    if( (nbit = MSX_FIND16B0IN32B(data[1])) )
        return 2 + nbit;
    if( (nbit = MSX_FIND16B0IN32B(data[2])) )
        return 4 + nbit;
    if( (nbit = MSX_FIND16B0IN32B(data[3])) )
        return 6 + nbit;
    
    return 0;
}

/* ================================================================================================================== */

inline static size_t msf_hash_mbs_32s32b0_check( const uint32_t *data )
{
    if( !data[0] )
        return 1;
    if( !data[1] )
        return 2;
    if( !data[2] )
        return 3;
    if( !data[3] )
        return 4;
    
    return 0;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_xxhash( const void *data, size_t length )
{
    uint32_t hash = 0;

    const uint32_t *cdat = data;
    const uint8_t  *adat;

    assert( data );

    /* długość ciągu znaków w tym przypadku musi być większa niż 15 */
    if( length > 0xF )
    {
        uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2,
                 val2 = MSD_HASH_SEED + MSN_H32XX_2,
                 val3 = MSD_HASH_SEED,
                 val4 = MSD_HASH_SEED - MSN_H32XX_1;
        size_t   iter = length >> 4; /* div by 16, dziel na 16, będą pobierane 4 porcje po 4 bajty */

        do
            (val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
            (val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
            (val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
            (val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1);
        while( --iter );

        hash = MSX_ROTL32( val1, 1 ) + MSX_ROTL32( val2, 7 ) + MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
    }
    else
        hash = MSD_HASH_SEED + MSN_H32XX_5;

    hash   += length;
    length &= 0xF;

    /* przejmuj odpadki */
    if( length > 11 )
        hash   += *cdat++ * MSN_H32XX_3,
        hash    = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        length -= 4;
    if( length > 7 )
        hash   += *cdat++ * MSN_H32XX_3,
        hash    = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        length -= 4;
    if( length > 3 )
        hash   += *cdat++ * MSN_H32XX_3,
        hash    = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        length -= 4;

    /* pozostałości mniejsze niż 4 bajty */
    adat = (const uint8_t*)cdat;

    switch( length )
    {
        case 3:
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
        case 2:
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
        case 1:
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
    }

    hash ^= hash >> 15;
    hash *= MSN_H32XX_2;
    hash ^= hash >> 13;
    hash *= MSN_H32XX_3;
    hash ^= hash >> 16;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_xxhash( const char *data )
{
    uint32_t hash = 0;
    size_t   nbit,
             slen = 0;

    const uint32_t *cdat = (const uint32_t*)data;
    const uint8_t  *adat;

    // wartości przy optymalizacji i tak już powinny być znane...
    uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2,
             val2 = MSD_HASH_SEED + MSN_H32XX_2,
             val3 = MSD_HASH_SEED,
             val4 = MSD_HASH_SEED - MSN_H32XX_1;

    assert( data );

    // szukaj zera jeszcze przed rozpoczęciem pętli
    nbit = msf_hash_mbs_32s8b0_check( cdat );

    if( !nbit )
    {
        // przetwarzaj, dopóki kod poniżej nie znajdzie 0 w ciągu
        while( !nbit )
            // po każdej pętli dodawaj do wartości 16 - ilość sprawdzonych znaków w których nie ma zera
            slen += 16,

            // przetwarzaj elementy bez zera
            (val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
            (val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
            (val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
            (val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

            // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
            // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
            nbit = msf_hash_mbs_32s8b0_check( cdat );
        
        hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
               MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
    }
    else
        hash = MSD_HASH_SEED + MSN_H32XX_5;

    // długość już jest znana
    slen += nbit - 1;
    hash += slen;

    // dalej jest to samo co w głównej funkcji...
    // jedyną różnicą są liczby podniesione o 1 z racji nbit i zwracanej wartości
    if( nbit > 12 )
        hash += *cdat++ * MSN_H32XX_3,
        hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        nbit -= 4;
    if( nbit > 8 )
        hash += *cdat++ * MSN_H32XX_3,
        hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        nbit -= 4;
    if( nbit > 3 )
        hash += *cdat++ * MSN_H32XX_3,
        hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
        nbit -= 4;

    adat = (const uint8_t*)cdat;

    switch( nbit )
    {
        case 4: 
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
        case 3:
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
        case 2:
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
    }

    hash ^= hash >> 15;
    hash *= MSN_H32XX_2;
    hash ^= hash >> 13;
    hash *= MSN_H32XX_3;
    hash ^= hash >> 16;

    return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_xxhash( const wchar_t *data )
{
    uint32_t hash = 0;
    size_t   nbit,
             slen = 0;

    const uint32_t *cdat = (const uint32_t*)data;

    // wartości przy optymalizacji i tak już powinny być znane...
    uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2,
             val2 = MSD_HASH_SEED + MSN_H32XX_2,
             val3 = MSD_HASH_SEED,
             val4 = MSD_HASH_SEED - MSN_H32XX_1;

    assert( data );

    if( sizeof(wchar_t) == 2 )
    {
        const uint8_t *adat;

        // szukaj zera jeszcze przed rozpoczęciem pętli
        nbit = msf_hash_mbs_32s16b0_check( cdat );

        if( !nbit )
        {
            while( !nbit )
                // po każdej pętli dodawaj do wartości 16 - ilość sprawdzonych znaków w których nie ma zera
                slen += 16,

                // przetwarzaj elementy bez zera
                (val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
                (val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
                (val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
                (val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

                // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
                // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
                nbit = msf_hash_mbs_32s16b0_check( cdat );

            hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
                   MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
        }
        else
            hash = MSD_HASH_SEED + MSN_H32XX_5;

        // długość już jest znana
        slen += nbit ? (nbit - 1) << 1 : 0;
        hash += slen;

        // dalej jest to samo co w głównej funkcji...
        // jedyną różnicą są liczby podniesione o 1 z racji nbit i zwracanej wartości
        if( nbit > 6 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
            nbit -= 2;
        if( nbit > 4 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
            nbit -= 2;
        if( nbit > 2 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
            nbit -= 2;

        adat = (const uint8_t*)cdat;

        if( nbit == 2 )
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
            hash += *adat++ * MSN_H32XX_5;
            hash  = MSX_ROTL32( hash, 11 ) * MSN_H32XX_1;
    }
    else if( sizeof(wchar_t) == 4 )
    {
        // szukaj zera jeszcze przed rozpoczęciem pętli
        nbit = msf_hash_mbs_32s32b0_check( cdat );

        if( !nbit )
        {
            while( !nbit )
                // po każdej pętli dodawaj do wartości 16 - ilość sprawdzonych znaków w których nie ma zera
                slen += 16,

                // przetwarzaj elementy bez zera
                (val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
                (val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
                (val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
                (val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

                // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
                // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
                nbit = msf_hash_mbs_32s32b0_check( cdat );

            hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
                   MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
        }
        else
            hash = MSD_HASH_SEED + MSN_H32XX_5;

        // długość już jest znana
        slen += nbit ? (nbit - 1) << 2 : 0;
        hash += slen;

        // dalej jest to samo co w głównej funkcji...
        // jedyną różnicą są liczby podniesione o 1 z racji nbit i zwracanej wartości
        if( nbit > 3 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
            nbit -= 1;
        if( nbit > 2 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4,
            nbit -= 1;
        if( nbit > 1 )
            hash += *cdat++ * MSN_H32XX_3,
            hash  = MSX_ROTL32( hash, 17 ) * MSN_H32XX_4;
    }

    hash ^= hash >> 15;
    hash *= MSN_H32XX_2;
    hash ^= hash >> 13;
    hash *= MSN_H32XX_3;
    hash ^= hash >> 16;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_64_xxhash( const void *data, size_t length )
{
    uint64_t hash = 0;

    const uint64_t *cdat = data;
    const uint8_t  *adat;
    const uint32_t *vdat;

    assert( data );

    /* jeżeli wartość jest większa niż 31 */
    if( length > 0x1F )
    {
        uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2,
                 val2 = MSD_HASH_SEED + MSN_H64XX_2,
                 val3 = MSD_HASH_SEED,
                 val4 = MSD_HASH_SEED - MSN_H64XX_1;
        size_t   iter = length >> 5; /* dziel na 32, będą pobierane 4 porcje po 8 bajtów */

        do
            val1 += *cdat++ * MSN_H64XX_2, val1 = MSX_ROTL64(val1, 31), val1 *= MSN_H64XX_1,
            val2 += *cdat++ * MSN_H64XX_2, val2 = MSX_ROTL64(val2, 31), val2 *= MSN_H64XX_1,
            val3 += *cdat++ * MSN_H64XX_2, val3 = MSX_ROTL64(val3, 31), val3 *= MSN_H64XX_1,
            val4 += *cdat++ * MSN_H64XX_2, val4 = MSX_ROTL64(val4, 31), val4 *= MSN_H64XX_1;
        while( --iter );

        hash = MSX_ROTL64( val1, 1  ) + MSX_ROTL64( val2, 7  ) +
               MSX_ROTL64( val3, 12 ) + MSX_ROTL64( val4, 18 );

        hash = msf_hash_mix64_value( hash, val1 );
        hash = msf_hash_mix64_value( hash, val2 );
        hash = msf_hash_mix64_value( hash, val3 );
        hash = msf_hash_mix64_value( hash, val4 );
    }
    else
        hash = MSD_HASH_SEED + MSN_H64XX_5;
    
    hash   += length;
    length &= 0x1F;

    /* przejmuj odpadki */
    if( length > 23 )
        hash = msf_hash_mix64_char64( hash, *cdat++ ),
        length -= 8;
    if( length > 15 )
        hash = msf_hash_mix64_char64( hash, *cdat++ ),
        length -= 8;
    if( length > 7 )
        hash = msf_hash_mix64_char64( hash, *cdat++ ),
        length -= 8;

    vdat = (const uint32_t*)cdat;
    if( length > 3 )
        hash   ^= *vdat++ * MSN_H64XX_1,
        hash    = MSX_ROTL64( hash, 23 ) * MSN_H64XX_2 + MSN_H64XX_3,
        length -= 4;

    adat = (const uint8_t*)vdat;
    switch( length )
    {
        case 3: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
        case 2: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
        case 1: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
    }

    hash ^= hash >> 33;
    hash *= MSN_H64XX_2;
    hash ^= hash >> 29;
    hash *= MSN_H64XX_3;
    hash ^= hash >> 32;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_mbs_64_xxhash( const char *data )
{
    uint64_t hash = 0;
    size_t   slen = 0,
             nbit;

    const uint64_t *cdat = (const uint64_t*)data;
    const uint8_t  *adat;
    const uint32_t *vdat;

    uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2,
             val2 = MSD_HASH_SEED + MSN_H64XX_2,
             val3 = MSD_HASH_SEED,
             val4 = MSD_HASH_SEED - MSN_H64XX_1;

    assert( data );

    // szukaj zera jeszcze przed rozpoczęciem pętli
    nbit = msf_hash_mbs_64s8b0_check( cdat );

    /* jeżeli wartość jest większa niż 31 */
    if( !nbit )
    {
        // przetwarzaj, dopóki kod poniżej nie znajdzie 0 w ciągu
        while( !nbit )
            // po każdej pętli dodawaj do wartości 32 - ilość sprawdzonych znaków w których nie ma zera
            slen += 32,

            // przetwarzaj elementy bez zera
            (val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
            (val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
            (val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
            (val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

            // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
            // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
            nbit = msf_hash_mbs_64s8b0_check( cdat );
        
        hash = MSX_ROTL64( val1, 1  ) + MSX_ROTL64( val2, 7  ) +
               MSX_ROTL64( val3, 12 ) + MSX_ROTL64( val4, 18 );

        hash = msf_hash_mix64_value( hash, val1 );
        hash = msf_hash_mix64_value( hash, val2 );
        hash = msf_hash_mix64_value( hash, val3 );
        hash = msf_hash_mix64_value( hash, val4 );
    }
    else
        hash = MSD_HASH_SEED + MSN_H64XX_5;

    // długość już jest znana
    slen += nbit - 1;
    hash += slen;

    if( nbit > 24 )
        hash  = msf_hash_mix64_char64( hash, *cdat++ ),
        nbit -= 8;
    else if( nbit > 16 )
        hash  = msf_hash_mix64_char64( hash, *cdat++ ),
        nbit -= 8;
    else if( nbit > 8 )
        hash  = msf_hash_mix64_char64( hash, *cdat++ ),
        nbit -= 8;

    vdat = (const uint32_t*)cdat;
    if( nbit > 4 )
        hash ^= *vdat++ * MSN_H64XX_1,
        hash  = MSX_ROTL64( hash, 23 ) * MSN_H64XX_2 + MSN_H64XX_3,
        nbit -= 4;

    adat = (const uint8_t*)vdat;
    switch( nbit )
    {
        case 4: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
        case 3: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
        case 2: (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
    }

    hash ^= hash >> 33;
    hash *= MSN_H64XX_2;
    hash ^= hash >> 29;
    hash *= MSN_H64XX_3;
    hash ^= hash >> 32;

    return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_wcs_64_xxhash( const wchar_t *data )
{
    uint64_t hash = 0;
    size_t   slen = 0,
             nbit;

    const uint64_t *cdat = (const uint64_t*)data;
    const uint32_t *vdat;

    uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2,
             val2 = MSD_HASH_SEED + MSN_H64XX_2,
             val3 = MSD_HASH_SEED,
             val4 = MSD_HASH_SEED - MSN_H64XX_1;

    assert( data );

    if( sizeof(wchar_t) == 2 )
    {
        const uint8_t  *adat;

        // szukaj zera jeszcze przed rozpoczęciem pętli
        nbit = msf_hash_mbs_64s16b0_check( cdat );

        /* jeżeli wartość jest większa niż 31 */
        if( !nbit )
        {
            // przetwarzaj, dopóki kod poniżej nie znajdzie 0 w ciągu
            while( !nbit )
                // po każdej pętli dodawaj do wartości 32 - ilość sprawdzonych znaków w których nie ma zera
                slen += 32,

                // przetwarzaj elementy bez zera
                (val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
                (val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
                (val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
                (val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

                // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
                // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
                nbit = msf_hash_mbs_64s16b0_check( cdat );
            
            hash = MSX_ROTL64( val1, 1  ) + MSX_ROTL64( val2, 7  ) +
                   MSX_ROTL64( val3, 12 ) + MSX_ROTL64( val4, 18 );

            hash = msf_hash_mix64_value( hash, val1 );
            hash = msf_hash_mix64_value( hash, val2 );
            hash = msf_hash_mix64_value( hash, val3 );
            hash = msf_hash_mix64_value( hash, val4 );
        }
        else
            hash = MSD_HASH_SEED + MSN_H64XX_5;

        // długość już jest znana
        slen += (nbit - 1) << 1;
        hash += slen;

        if( nbit > 12 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 4;
        if( nbit > 8 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 4;
            if( nbit > 4 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 4;

        vdat = (const uint32_t*)cdat;
        if( nbit > 2 )
            hash ^= *vdat++ * MSN_H64XX_1,
            hash  = MSX_ROTL64( hash, 23 ) * MSN_H64XX_2 + MSN_H64XX_3,
            nbit -= 2;

        adat = (const uint8_t*)vdat;
        if( nbit == 2 )
            (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1),
            (hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
    }
    else if( sizeof(wchar_t) == 4 )
    {
        // szukaj zera jeszcze przed rozpoczęciem pętli
        nbit = msf_hash_mbs_64s32b0_check( cdat );

        /* jeżeli wartość jest większa niż 31 */
        if( !nbit )
        {
            // przetwarzaj, dopóki kod poniżej nie znajdzie 0 w ciągu
            while( !nbit )
                // po każdej pętli dodawaj do wartości 32 - ilość sprawdzonych znaków w których nie ma zera
                slen += 32,

                // przetwarzaj elementy bez zera
                (val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
                (val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
                (val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
                (val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

                // jako że są róznice pomiędzy przetwarzaniem wszystkich a częściowych,
                // trzeba najpierw szukać zera we wszystkich pobieranych elementach...            
                nbit = msf_hash_mbs_64s32b0_check( cdat );
            
            hash = MSX_ROTL64( val1, 1  ) + MSX_ROTL64( val2, 7  ) +
                   MSX_ROTL64( val3, 12 ) + MSX_ROTL64( val4, 18 );

            hash = msf_hash_mix64_value( hash, val1 );
            hash = msf_hash_mix64_value( hash, val2 );
            hash = msf_hash_mix64_value( hash, val3 );
            hash = msf_hash_mix64_value( hash, val4 );
        }
        else
            hash = MSD_HASH_SEED + MSN_H64XX_5;

        // długość już jest znana
        slen += (nbit - 1) << 2;
        hash += slen;

        if( nbit > 6 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 2;
        if( nbit > 4 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 2;
        if( nbit > 2 )
            hash  = msf_hash_mix64_char64( hash, *cdat++ ),
            nbit -= 2;

        vdat = (const uint32_t*)cdat;
        if( nbit == 2 )
            hash ^= *vdat++ * MSN_H64XX_1,
            hash  = MSX_ROTL64( hash, 23 ) * MSN_H64XX_2 + MSN_H64XX_3;
    }

    hash ^= hash >> 33;
    hash *= MSN_H64XX_2;
    hash ^= hash >> 29;
    hash *= MSN_H64XX_3;
    hash ^= hash >> 32;

    return hash;
}
