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
 *  Header file for "String" module.
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

#ifndef __MSH_STRING__
#define __MSH_STRING__

#include <msconf.h>
#include <moss/array.h>

#include <stdlib.h>
#include <limits.h>
#include <wchar.h>


/* dołącza plik zawierający funkcje do tworzenia skrótów */
#ifdef MSD_STRING_HASH
#	include <moss/hash.h>
#endif

/* standardowe funkcje skrótu dla ciągu znaków, używające jednego algorytmu */
#ifdef MSD_STRING_HASH
	/* dedykowana dla mbs */
#	if defined MSD_STRING_HASH_DEDICATED && defined MSD_HASH_MBS_FUNCTIONS
#		ifdef MSD_STRING_HASH_64
#			define ms_string_hash_mbs(mbs) \
				MSD_CALLCONNECT(ms_hash_mbs_64_, MSD_STRING_HASH)( mbs )
#		else
#			define ms_string_hash_mbs(mbs) \
				MSD_CALLCONNECT(ms_hash_mbs_32_, MSD_STRING_HASH)( mbs )
#		endif
#	endif
	/* dedykowana dla wcs */
#	if defined MSD_STRING_HASH_DEDICATED && defined MSD_HASH_WCS_FUNCTIONS
#		ifdef MSD_STRING_HASH_64
#			define ms_string_hash_wcs(wcs) \
				MSD_CALLCONNECT(ms_hash_wcs_64_, MSD_STRING_HASH)( wcs )
#		else
#			define ms_string_hash_wcs(wcs) \
				MSD_CALLCONNECT(ms_hash_wcs_32_, MSD_STRING_HASH)( wcs )
#		endif
#	endif
	/* ogólna */
#	ifdef MSD_STRING_HASH_64
#		define ms_string_hash_byte(data, len) \
			MSD_CALLCONNECT(ms_hash_64_, MSD_STRING_HASH)( data, len )
#	else
#		define ms_string_hash_byte(data, len) \
			MSD_CALLCONNECT(ms_hash_32_, MSD_STRING_HASH)( data, len )
#	endif
#endif

typedef struct MSS_MBINFO
{
	size_t Offset;
	char   Bytes;
}
MS_MBINFO;

typedef struct MSS_STRING
{
	union
	{
		char    *Char;
		wchar_t *Wide;
		tchar   *Auto;
	}
	Data;

	size_t Length;
	size_t Capacity;
	size_t Hash;
	float  Modifier;
	bool   Wide;
	bool   Destroy;
	bool   Hashed;

	MS_ARRAY *MBInfo;

	size_t (*FuncIncrease)(size_t capacity, float modifier);
}
MS_STRING;


MS_STRING *ms_string_alloc_cs( const char *cstr, size_t capacity );
MS_STRING *ms_string_alloc_mbs( const char *mbstr, size_t capacity );
MS_STRING *ms_string_alloc_wcs( const wchar_t *wcstr, size_t capacity );

int ms_string_init_cs( MS_STRING *str, const char *cstr, size_t capacity );
int ms_string_init_mbs( MS_STRING *str, const char *mbstr, size_t capacity );
int ms_string_init_wcs( MS_STRING *str, const wchar_t *wcstr, size_t capacity );

MS_STRING ms_string_return_cs( const char *cstr, size_t capacity );
MS_STRING ms_string_return_mbs( const char *mbstr, size_t capacity );
MS_STRING ms_string_return_wcs( const wchar_t *wcstr, size_t capacity );

int ms_string_realloc( MS_STRING *str, size_t capacity );
int ms_string_realloc_min( MS_STRING *str, size_t min );

int ms_string_insert_cs( MS_STRING *str, size_t index, const char *cstr, size_t count );
int ms_string_insert_mbs( MS_STRING *str, size_t index, const char *mbstr, size_t bytes );
// int ms_string_insert_mbsc( MS_STRING *str, size_t index, const char *mbstr, size_t count );
int ms_string_insert_wcs( MS_STRING *str, size_t index, const wchar_t *wcstr, size_t count );

// int ms_string_insert_c( MS_STRING *str, size_t index, char chr );
// int ms_string_insert_wc( MS_STRING *str, size_t index, wchar_t wchr );

// size_t ms_string_hash( MS_STRING *str );

size_t ms_string_length_mbs( const char *mbstr, size_t bytes );
size_t ms_string_length_wcstombs( const wchar_t *wstr, size_t count );
int ms_string_info_mbs( const char *mbs, size_t bytes, MS_ARRAY *info );
int ms_string_info_wcstombs( const wchar_t *wstr, size_t count, MS_ARRAY *info );

void ms_string_clear( MS_STRING *str );
void ms_string_free( MS_STRING *str );

#define ms_string_push_cs(str, cstr, count) \
	ms_string_insert_cs( str, (str)->MBInfo \
		? (str)->MBInfo->Length \
		: (str)->Length, cstr, count )

#define ms_string_push_mbs(str, mbstr, bytes) \
	ms_string_insert_mbs( str, (str)->MBInfo \
		? (str)->MBInfo->Length \
		: (str)->Length, mbstr, bytes )

#define ms_string_push_wcs(str, wcstr, count) \
	ms_string_insert_wcs( str, (str)->MBInfo \
		? (str)->MBInfo->Length \
		: (str)->Length, wcstr, count )

#endif
