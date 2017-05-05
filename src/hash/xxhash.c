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
 *  Source file for "Hash" module, xxHash algorithm [BSD 2-Clause License].
 *  All modifications are based on GPLv3 license.
 *  
 *  This file is part of Moss Library
 *  Copyright (c) by sobiemir <sobiemir@aculo.pl>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
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

#include <moss/hash.h>

/* wartości dla 32 bitowych funkcji */
#define MSN_H32XX_1 2654435761u
#define MSN_H32XX_2 2246822519u
#define MSN_H32XX_3 3266489917u
#define MSN_H32XX_4 668265263u
#define MSN_H32XX_5 374761393u

/* wartości dla 64 bitowych funkcji */
#define MSN_H64XX_1 11400714785074694791ull
#define MSN_H64XX_2 14029467366897019727ull
#define MSN_H64XX_3 1609587929392839161ull
#define MSN_H64XX_4 9650029242287828579ull
#define MSN_H64XX_5 2870177450012600261ull

#ifdef MSD_COMPILER_MSC
	__pragma( warning(push) )
	__pragma( warning(disable:4307))    /* integral constant overflow */
	__pragma( warning(disable:4127))    /* conditional expression is constant */
#endif

/**
 * Miesza 64 bitowy skrót danych z podaną wartością.
 * 
 * @param  hash  Aktualny skrót danych.
 * @param  value Wartość do mieszania.
 * @return       Nowy skrót danych.
 */
INLINE static uint64_t msf_hash_mix64_value( uint64_t hash, uint64_t value );

/**
 * Miesza 64 bitowy skrót danych z kolejną wartością pobraną z danych.
 * 
 * @param  hash  Aktualny skrót danych.
 * @param  value Wartość do mieszania.
 * @return       Nowy skrót danych.
 */
INLINE static uint64_t msf_hash_mix64_char64( uint64_t hash, uint64_t c64 );

/**
 * Szuka 8 bitowego zera w 4 porcjach 32 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_32s8b0_check( const uint32_t *data );

/**
 * Szuka 8 bitowego zera w 4 porcjach 64 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_64s8b0_check( const uint64_t *data );

/**
 * Szuka 16 bitowego zera w 4 porcjach 32 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_32s16b0_check( const uint32_t *data );

/**
 * Szuka 16 bitowego zera w 4 porcjach 64 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_64s16b0_check( const uint64_t *data );

/**
 * Szuka 32 bitowego zera w 4 porcjach 32 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_32s32b0_check( const uint32_t *data );

/**
 * Szuka 32 bitowego zera w 4 porcjach 64 bitowych wartości.
 * W przypadku gdy zero zostanie znalezione, funkcja zwróci jego indeks rozpoczynający się od wartości 1.
 * 
 * @param  data Dane do przeszukania.
 * @return      Indeks rozpoczynający się od 1 lub wartość 0.
 */
INLINE static size_t msf_hash_mbs_64s32b0_check( const uint64_t *data );

/* ================================================================================================================== */

uint32_t ms_hash_32_xxhash( const void *data, size_t length )
{
	uint32_t hash = 0;

	const uint32_t *cdat = data;
	const uint8_t  *adat;

	assert( data );

	if( length > 0xF )
	{
		uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2;
		uint32_t val2 = MSD_HASH_SEED + MSN_H32XX_2;
		uint32_t val3 = MSD_HASH_SEED;
		uint32_t val4 = MSD_HASH_SEED - MSN_H32XX_1;

		/* dziel na 16, pobierane będą 4 porcje po 32 bity, 32 bity => 4 bajty, 4 * 4 = 16 */
		size_t iter = length >> 4;

		do
			(val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
			(val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
			(val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
			(val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1);
		while( --iter );

		hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
		       MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
	}
	else
		hash = MSD_HASH_SEED + MSN_H32XX_5;

	/* dodaj długość do skrótu i nałóż maskę aby wykryć odpadki */
	hash   += length;
	length &= 0xF;

	/* przejmuj odpadki */
	if( length > 11 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (length -= 12);
	else if( length > 7 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (length -= 8);
	else if( length > 3 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (length -= 4);

	/* odpadki mniejsze niż 4 bajty */
	adat = (const uint8_t*)cdat;
	switch( length )
	{
		case 3: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
		case 2: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
		case 1: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
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
	const uint32_t *vdat;
	const uint8_t  *adat;

	assert( data );

	if( length > 0x1F )
	{
		uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2;
		uint64_t val2 = MSD_HASH_SEED + MSN_H64XX_2;
		uint64_t val3 = MSD_HASH_SEED;
		uint64_t val4 = MSD_HASH_SEED - MSN_H64XX_1;

		/* dziel na 32, będą pobierane 4 porcje po 64 bity, 64 bity => 8 bajtów, 4 * 8 = 32 */
		size_t iter = length >> 5;

		do
			(val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
			(val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
			(val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
			(val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1);
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
	
	/* dodaj długość do skrótu i nałóż maskę aby wykryć odpadki */
	hash   += length;
	length &= 0x1F;

	/* przejmuj odpadki */
	if( length > 23 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ), length -= 24;
	else if( length > 15 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ), length -= 16;
	else if( length > 7 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ), length -= 8;

	/* odpadki mniejsze niż 8 bajtów */
	vdat = (const uint32_t*)cdat;
	if( length > 3 )
		(hash ^= *vdat++ * MSN_H64XX_1), (hash = MSX_ROTL64(hash, 23) * MSN_H64XX_2 + MSN_H64XX_3), (length -= 4);

	/* odpadki mniejsze niż 4 bajty */
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

#ifdef MSD_HASH_MBS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_mbs_32_xxhash( const char *data )
{
	uint32_t hash = 0;
	size_t   slen = 0;
	size_t   nbit;

	const uint32_t *cdat = (const uint32_t*)data;
	const uint8_t  *adat;

	/* wartości przy optymalizacji i tak już powinny być znane... */
	uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2;
	uint32_t val2 = MSD_HASH_SEED + MSN_H32XX_2;
	uint32_t val3 = MSD_HASH_SEED;
	uint32_t val4 = MSD_HASH_SEED - MSN_H32XX_1;

	assert( data );

	/* szukaj zera jeszcze przed rozpoczęciem pętli */
	nbit = msf_hash_mbs_32s8b0_check( cdat );
	if( !nbit )
	{
		/* przetwarzaj, dopóki kod poniżej nie znajdzie 0 w ciągu */
		while( !nbit )
			/* po każdej pętli dodawaj ilość sprawdzonych bajtów w których nie ma zera */
			slen += 16,

			(val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
			(val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
			(val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
			(val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

			/* trzeba niestety szukać zer w następnych elementach przed kolejną iteracją */
			nbit = msf_hash_mbs_32s8b0_check( cdat );
		
		hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
		       MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
	}
	else
		hash = MSD_HASH_SEED + MSN_H32XX_5;

	/* długość ciągu już jest znana */
	slen += nbit - 1;
	hash += slen;

	/* przejmuj odpadki, liczba podniesiona względem oryginału ze względu wartości nbit */
	if( nbit > 12 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 12);
	else if( nbit > 8 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 8);
	else if( nbit > 4 )
		(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 4);

	adat = (const uint8_t*)cdat;
	switch( nbit )
	{
		case 4: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
		case 3: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
		case 2: (hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
	}

	hash ^= hash >> 15;
	hash *= MSN_H32XX_2;
	hash ^= hash >> 13;
	hash *= MSN_H32XX_3;
	hash ^= hash >> 16;

	return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_mbs_64_xxhash( const char *data )
{
	uint64_t hash = 0;
	size_t   slen = 0;
	size_t   nbit;

	const uint64_t *cdat = (const uint64_t*)data;
	const uint32_t *vdat;
	const uint8_t  *adat;

	uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2;
	uint64_t val2 = MSD_HASH_SEED + MSN_H64XX_2;
	uint64_t val3 = MSD_HASH_SEED;
	uint64_t val4 = MSD_HASH_SEED - MSN_H64XX_1;

	assert( data );

	nbit = msf_hash_mbs_64s8b0_check( cdat );
	if( !nbit )
	{
		while( !nbit )
			slen += 32,

			(val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
			(val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
			(val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
			(val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

			/* trzeba niestety szukać zer w następnych elementach przed kolejną iteracją */         
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

	/* długość ciągu już jest znana */
	slen += nbit - 1;
	hash += slen;

	/* odpadki 64, 32 i 8 bajtowe */
	if( nbit > 24 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 24;
	else if( nbit > 16 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ),
		hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 16;
	else if( nbit > 8 )
		hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 8;

	vdat = (const uint32_t*)cdat;
	if( nbit > 4 )
		(hash ^= *vdat++ * MSN_H64XX_1), (hash = MSX_ROTL64(hash, 23) * MSN_H64XX_2 + MSN_H64XX_3), (nbit -= 4);

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

#endif
#ifdef MSD_HASH_WCS_FUNCTIONS

/* ================================================================================================================== */

uint32_t ms_hash_wcs_32_xxhash( const wchar_t *data )
{
	uint32_t hash = 0;
	size_t   nbit,
	         slen = 0;

	const uint32_t *cdat = (const uint32_t*)data;

	uint32_t val1 = MSD_HASH_SEED + MSN_H32XX_1 + MSN_H32XX_2;
	uint32_t val2 = MSD_HASH_SEED + MSN_H32XX_2;
	uint32_t val3 = MSD_HASH_SEED;
	uint32_t val4 = MSD_HASH_SEED - MSN_H32XX_1;

	assert( data );

	/* wersja dla 2 bajtowego rozmiaru wchar_t */
	if( sizeof(wchar_t) == 2 )
	{
		const uint8_t *adat;

		nbit = msf_hash_mbs_32s16b0_check( cdat );
		if( !nbit )
		{
			while( !nbit )
				slen += 16,

				(val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
				(val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
				(val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
				(val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

				nbit = msf_hash_mbs_32s16b0_check( cdat );

			hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
			       MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
		}
		else
			hash = MSD_HASH_SEED + MSN_H32XX_5;

		/* wartość nbit trzeba pomnożyć przez 2 z racji rozmiaru wchar_t */
		slen += nbit ? (nbit - 1) << 1 : 0;
		hash += slen;

		/* liczba podzielona przez 2 względem wersji mbs z racji rozmiaru wchar_t */
		if( nbit > 6 )
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 6);
		if( nbit > 4 )
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4),
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 4);
		if( nbit > 2 )
			(hash += *cdat++ * MSN_H32XX_3), (hash = MSX_ROTL32(hash, 17) * MSN_H32XX_4), (nbit -= 2);

		/* tylko dwa odpadki 8 bitowe z racji tego, że 0 zajmuje całe 16 bitów */
		adat = (const uint8_t*)cdat;
		if( nbit == 2 )
			(hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1),
			(hash += *adat++ * MSN_H32XX_5), (hash = MSX_ROTL32(hash, 11) * MSN_H32XX_1);
	}
	/* wersja dla 4 bajtowego rozmiaru wchar_t */
	else if( sizeof(wchar_t) == 4 )
	{
		nbit = msf_hash_mbs_32s32b0_check( cdat );
		if( !nbit )
		{
			while( !nbit )
				slen += 16,

				(val1 += *cdat++ * MSN_H32XX_2), (val1 = MSX_ROTL32(val1, 13)), (val1 *= MSN_H32XX_1),
				(val2 += *cdat++ * MSN_H32XX_2), (val2 = MSX_ROTL32(val2, 13)), (val2 *= MSN_H32XX_1),
				(val3 += *cdat++ * MSN_H32XX_2), (val3 = MSX_ROTL32(val3, 13)), (val3 *= MSN_H32XX_1),
				(val4 += *cdat++ * MSN_H32XX_2), (val4 = MSX_ROTL32(val4, 13)), (val4 *= MSN_H32XX_1),

				nbit = msf_hash_mbs_32s32b0_check( cdat );

			hash = MSX_ROTL32( val1, 1  ) + MSX_ROTL32( val2, 7  ) +
			       MSX_ROTL32( val3, 12 ) + MSX_ROTL32( val4, 18 );
		}
		else
			hash = MSD_HASH_SEED + MSN_H32XX_5;

		/* wartość nbit trzeba pomnożyć przez 4 z racji rozmiaru wchar_t */
		slen += nbit ? (nbit - 1) << 2 : 0;
		hash += slen;

		/* liczba podzielona przez 4 względem wersji mbs z racji rozmiaru wchar_t */
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

		/* brak odpadków 8 bitowych, ponieważ 0 zajmuje całe 32 bity! */
	}
	/* czy wchar_t może mieć inne rozmiary? */

	hash ^= hash >> 15;
	hash *= MSN_H32XX_2;
	hash ^= hash >> 13;
	hash *= MSN_H32XX_3;
	hash ^= hash >> 16;

	return hash;
}

/* ================================================================================================================== */

uint64_t ms_hash_wcs_64_xxhash( const wchar_t *data )
{
	uint64_t hash = 0;
	size_t   slen = 0;
	size_t   nbit;

	const uint64_t *cdat = (const uint64_t*)data;
	const uint32_t *vdat;

	uint64_t val1 = MSD_HASH_SEED + MSN_H64XX_1 + MSN_H64XX_2;
	uint64_t val2 = MSD_HASH_SEED + MSN_H64XX_2;
	uint64_t val3 = MSD_HASH_SEED;
	uint64_t val4 = MSD_HASH_SEED - MSN_H64XX_1;

	assert( data );

	/* wersja dla 2 bajtowego rozmiaru wchar_t */
	if( sizeof(wchar_t) == 2 )
	{
		const uint8_t  *adat;

		nbit = msf_hash_mbs_64s16b0_check( cdat );
		if( !nbit )
		{
			while( !nbit )
				slen += 32,

				(val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
				(val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
				(val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
				(val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

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

		slen += (nbit - 1) << 1;
		hash += slen;

		/* liczba podzielona przez 2 względem wersji mbs z racji rozmiaru wchar_t */
		if( nbit > 12 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 12;
		if( nbit > 8 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 8;
		if( nbit > 4 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 4;

		vdat = (const uint32_t*)cdat;
		if( nbit > 2 )
			(hash ^= *vdat++ * MSN_H64XX_1), (hash = MSX_ROTL64(hash, 23) * MSN_H64XX_2 + MSN_H64XX_3), (nbit -= 2);

		/* tylko dwa odpadki 8 bitowe z racji tego, że 0 zajmuje całe 16 bitów */
		adat = (const uint8_t*)vdat;
		if( nbit == 2 )
			(hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1),
			(hash ^= *adat++ * MSN_H64XX_5), (hash = MSX_ROTL64(hash, 11) * MSN_H64XX_1);
	}
	/* wersja dla 4 bajtowego rozmiaru wchar_t */
	else if( sizeof(wchar_t) == 4 )
	{
		nbit = msf_hash_mbs_64s32b0_check( cdat );
		if( !nbit )
		{
			while( !nbit )
				slen += 32,

				(val1 += *cdat++ * MSN_H64XX_2), (val1 = MSX_ROTL64(val1, 31)), (val1 *= MSN_H64XX_1),
				(val2 += *cdat++ * MSN_H64XX_2), (val2 = MSX_ROTL64(val2, 31)), (val2 *= MSN_H64XX_1),
				(val3 += *cdat++ * MSN_H64XX_2), (val3 = MSX_ROTL64(val3, 31)), (val3 *= MSN_H64XX_1),
				(val4 += *cdat++ * MSN_H64XX_2), (val4 = MSX_ROTL64(val4, 31)), (val4 *= MSN_H64XX_1),

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

		slen += (nbit - 1) << 2;
		hash += slen;

		/* liczba podzielona przez 4 względem wersji mbs z racji rozmiaru wchar_t */
		if( nbit > 6 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 6;
		if( nbit > 4 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ),
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 4;
		if( nbit > 2 )
			hash = msf_hash_mix64_char64( hash, *cdat++ ), nbit -= 2;

		vdat = (const uint32_t*)cdat;
		if( nbit == 2 )
			hash ^= *vdat++ * MSN_H64XX_1,
			hash  = MSX_ROTL64( hash, 23 ) * MSN_H64XX_2 + MSN_H64XX_3;

		/* brak odpadków 8 bitowych, ponieważ 0 zajmuje całe 32 bity! */
	}
	/* czy wchar_t może mieć inne rozmiary? */

	hash ^= hash >> 33;
	hash *= MSN_H64XX_2;
	hash ^= hash >> 29;
	hash *= MSN_H64XX_3;
	hash ^= hash >> 32;

	return hash;
}

#endif

/* ================================================================================================================== */

INLINE static uint64_t msf_hash_mix64_value( uint64_t hash, uint64_t value )
{
	value *= MSN_H64XX_2;
	value  = MSX_ROTL64( value, 31 );
	value *= MSN_H64XX_1;
	hash  ^= value;

	return hash * MSN_H64XX_1 + MSN_H64XX_4;
}

/* ================================================================================================================== */

INLINE static uint64_t msf_hash_mix64_char64( uint64_t hash, uint64_t c64 )
{
	c64  *= MSN_H64XX_2;
	c64   = MSX_ROTL64( c64, 31 );
	c64  *= MSN_H64XX_1;
	hash ^= c64;

	return MSX_ROTL64( hash, 27 ) * MSN_H64XX_1 + MSN_H64XX_4;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_32s8b0_check( const uint32_t *data )
{
	size_t nbit;

	if( (nbit = MSX_FIND8B0IN32B(data[0])) != 0 )
		return nbit;
	if( (nbit = MSX_FIND8B0IN32B(data[1])) != 0 )
		return 4 + nbit;
	if( (nbit = MSX_FIND8B0IN32B(data[2])) != 0 )
		return 8 + nbit;
	if( (nbit = MSX_FIND8B0IN32B(data[3])) != 0 )
		return 12 + nbit;
	
	return 0;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_64s8b0_check( const uint64_t *data )
{
	size_t nbit;

	if( (nbit = MSX_FIND8B0IN64B(data[0])) != 0 )
		return nbit;
	if( (nbit = MSX_FIND8B0IN64B(data[1])) != 0 )
		return 8 + nbit;
	if( (nbit = MSX_FIND8B0IN64B(data[2])) != 0 )
		return 16 + nbit;
	if( (nbit = MSX_FIND8B0IN64B(data[3])) != 0 )
		return 24 + nbit;

	return 0;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_64s16b0_check( const uint64_t *data )
{
	size_t nbit;

	if( (nbit = MSX_FIND16B0IN64B(data[0])) != 0 )
		return nbit;
	if( (nbit = MSX_FIND16B0IN64B(data[1])) != 0 )
		return 4 + nbit;
	if( (nbit = MSX_FIND16B0IN64B(data[2])) != 0 )
		return 8 + nbit;
	if( (nbit = MSX_FIND16B0IN64B(data[3])) != 0 )
		return 12 + nbit;

	return 0;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_64s32b0_check( const uint64_t *data )
{
	size_t nbit;

	if( (nbit = MSX_FIND32B0IN64B(data[0])) != 0 )
		return nbit;
	if( (nbit = MSX_FIND32B0IN64B(data[1])) != 0 )
		return 2 + nbit;
	if( (nbit = MSX_FIND32B0IN64B(data[2])) != 0 )
		return 4 + nbit;
	if( (nbit = MSX_FIND32B0IN64B(data[3])) != 0 )
		return 6 + nbit;

	return 0;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_32s16b0_check( const uint32_t *data )
{
	size_t nbit;

	if( (nbit = MSX_FIND16B0IN32B(data[0])) != 0 )
		return nbit;
	if( (nbit = MSX_FIND16B0IN32B(data[1])) != 0 )
		return 2 + nbit;
	if( (nbit = MSX_FIND16B0IN32B(data[2])) != 0 )
		return 4 + nbit;
	if( (nbit = MSX_FIND16B0IN32B(data[3])) != 0 )
		return 6 + nbit;
	
	return 0;
}

/* ================================================================================================================== */

INLINE static size_t msf_hash_mbs_32s32b0_check( const uint32_t *data )
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

#ifdef MSD_COMPILER_MSC
	__pragma( warning(pop) )
#endif
