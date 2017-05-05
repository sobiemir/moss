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
 *  Source file for "Hash" module, Murmur algorithms [Public Domain].
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

#include <moss/hash.h>

#define MSN_H32M3_1 0xCC9E2D51
#define MSN_H32M3_2 0x1B873593
#define MSN_H32M3_3 0xE6546B64
#define MSN_H32M2   0x5BD1E995
#define MSN_H64M2   0xC6A4A7935BD1E995ull
#define MSN_H32M1   0xC6A4A793

#ifdef MSD_COMPILER_MSC
	__pragma( warning(push) )
	__pragma( warning(disable:4127))    /* conditional expression is constant */
#endif

/**
 * Miesza 32 bitowy skrót danych z podaną wartością.
 * 
 * @param  hash  Aktualny skrót danych.
 * @param  value Wartość do mieszania.
 * @return       Nowy skrót danych.
 */
INLINE static uint32_t msf_hash_mix32_value( uint32_t hash, uint32_t value );

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur3( const void *data, size_t length )
{
	uint32_t hash = MSD_HASH_SEED;
	uint32_t ch4b = 0;
	size_t   iter;

	const uint32_t *cdat = data;
	const uint8_t  *adat;

	assert( data );

	/* murmur3 pobiera 4 bajtowe porcje */
	for( iter = length >> 2; iter--; )
		hash = msf_hash_mix32_value( hash, *cdat++ );

	/* dolicz odpadki */
	adat = (const uint8_t*)cdat;
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

	hash ^= length;
	hash ^= hash >> 16;
	hash *= 0x85EBCA6B;
	hash ^= hash >> 13;
	hash *= 0xC2B2AE35;
	hash ^= hash >> 16;

	return hash;
}

/* ================================================================================================================== */

uint32_t ms_hash_32_murmur2( const void *data, size_t length )
{
	/* ta linijka uniemożliwia stworzenie rozwiązania dedykowanego dla mbs i wcs */
	uint32_t hash = MSD_HASH_SEED ^ length;
	uint32_t ch4b;

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
	
	/* murmur2 również posiada odpadki */
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

uint32_t ms_hash_32_murmur1( const void *data, size_t length )
{
	/* ta linijka uniemożliwia stworzenie rozwiązania dedykowanego dla mbs i wcs */
	uint32_t hash = MSD_HASH_SEED ^ (length * MSN_H32M1);
	
	const uint32_t *cdat = data;
	const uint8_t  *adat;

	assert( data );

	for( ; length > 3; length -= 4 )
		(hash += *cdat++), (hash *= MSN_H32M1), (hash ^= hash >> 16);

	/* dolicz odpadki */
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
	/* ta linijka uniemożliwia stworzenie rozwiązania dedykowanego dla mbs i wcs */
	uint64_t hash = MSD_HASH_SEED ^ (length * MSN_H64M2);
	uint64_t ch4b;
	
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

	/* dolicz odpadki, jest ich 8 w 64 bitowej porcji */
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
	uint32_t hash = MSD_HASH_SEED;
	size_t   slen = 0;
	uint32_t ch4b;
	size_t   nbit;

	const uint32_t *cdat = (const uint32_t*)data;

	assert( data );

	/* mieszaj i licz długość ciągu znaków */
	while( ch4b = *cdat++, (nbit = MSX_FIND8B0IN32B(ch4b)) == 0 )
		hash  = msf_hash_mix32_value( hash, ch4b ),
		slen += 4;

	/* dolicz odpadki */
	if( nbit > 1 )
		ch4b &= 0xFFFFFFFF >> ((5 - nbit) << 3),
		ch4b *= MSN_H32M3_1,
		ch4b  = MSX_ROTL32( ch4b, 15 ),
		ch4b *= MSN_H32M3_2,
		hash ^= ch4b,
		slen += nbit - 1;

	hash ^= slen;
	hash ^= hash >> 16;
	hash *= 0x85EBCA6B;
	hash ^= hash >> 13;
	hash *= 0xC2B2AE35;
	hash ^= hash >> 16;

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

	/* 2 bajtowy wchar_t */
	if( sizeof(wchar_t) == 2 )
		while( (ch4b = *cdat++) != 0 )
		{
			/* sprawdź czy pierwszy znak w porcji będzie równy 0 */
			if( !(ch4b & 0x0000FFFF) )
				break;
			/* jeżeli drugi znak jest równy 0, to ten pierwszy nadal pozostaje do obliczenia */
			if( !(ch4b & 0xFFFF0000) )
			{
				ch4b *= MSN_H32M3_1;
				ch4b  = MSX_ROTL32( ch4b, 15 );
				ch4b *= MSN_H32M3_2;
				hash ^= ch4b;
				slen += 2;
				break;
			}
			/* w przeciwnym wypadku licz całość */
			hash  = msf_hash_mix32_value( hash, ch4b );
			slen += 4;
		}
	/* 4 bajtowy wchar_t - brak odpadków... */
	else if( sizeof(wchar_t) == 4 )
		while( (ch4b = *cdat++) != 0 )
			hash  = msf_hash_mix32_value( hash, ch4b ),
			slen += 4;

	hash ^= slen;
	hash ^= hash >> 16;
	hash *= 0x85EBCA6B;
	hash ^= hash >> 13;
	hash *= 0xC2B2AE35;
	hash ^= hash >> 16;

	return hash;
}

#endif

/* ================================================================================================================== */

INLINE static uint32_t msf_hash_mix32_value( uint32_t hash, uint32_t value )
{
	value *= MSN_H32M3_1;
	value  = MSX_ROTL32( value, 15 );
	value *= MSN_H32M3_2;
	hash  ^= value;
	hash   = MSX_ROTL32( hash, 13 );

	return hash + ((hash << 2) + MSN_H32M3_3);
}

#ifdef MSD_COMPILER_MSC
	__pragma( warning(pop) )
#endif
