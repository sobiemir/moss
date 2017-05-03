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
 *  Header file for "Hash" module.
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

#ifndef __MSH_HASH__
#define __MSH_HASH__

#include <msconf.h>
#include <wchar.h>
#include <stdint.h>

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DJB ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_DJB
	/**
	 * Funkcja skrótu oparta na algorytmie DJB2.
	 * Ta wersja algorytmu używa dodawania podczas operacji na pobieranym znaku.
	 * 
	 * http://www.cse.yorku.ca/~oz/hash.html#djb2
	 * https://cr.yp.to/softwarelaw.html
	 * https://github.com/libretro/libretro-common/blob/master/utils/djb2.c
	 * Public Domain?
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_djb2( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie DJB2a.
	 * Ta wersja algorytmu używa alternatywy wykluczającej (XOR) podczas operacji na pobieranym znaku.
	 * 
	 * http://www.cse.yorku.ca/~oz/hash.html#djb2
	 * https://cr.yp.to/softwarelaw.html
	 * Public Domain?
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_djb2a( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie DJB2.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_djb2( const char *data );

		/**
		 * Funkcja skrótu oparta na algorytmie DJB2a.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_djb2a( const char *data );

#	endif
#	ifdef MSD_HASH_WCS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie DJB2.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_djb2( const wchar_t *data );
		
		/**
		 * Funkcja skrótu oparta na algorytmie DJB2a.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_djb2a( const wchar_t *data );

#	endif
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	SDBM ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_SDBM
	/**
	 * Funkcja skrótu oparta na algorytmie zastosowanym w SDBM.
	 *
	 * http://www.cse.yorku.ca/~oz/hash.html#sdbm
	 * https://github.com/davidar/sdbm/blob/master/hash.c
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_sdbm( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie zastosowanym w SDBM.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_sdbm( const char *data );

#	endif
#	ifdef MSD_HASH_WCS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie zastosowanym w SDBM.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_sdbm( const wchar_t *data );

#	endif
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	JOAAT ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_JOAAT
	/**
	 * Funkcja skrótu oparta na algorytmie JOAAT (Jenkins One At A Time).
	 *
	 * https://en.wikipedia.org/wiki/Jenkins_hash_function
	 * http://www.burtleburtle.net/bob/hash/doobs.html
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_joaat( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie JoaaT.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_joaat( const char *data );

#	endif
#	ifdef MSD_HASH_WCS_FUNCTIONS
	
		/**
		 * Funkcja skrótu oparta na algorytmie JoaaT.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_joaat( const wchar_t *data );

#	endif
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	FNV ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_FNV
	/**
	 * Funkcja skrótu oparta na algorytmie FNV-1.
	 * 
	 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_fnv1( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie FNV-1a.
	 * Od zwykłego algorytmu FNV-1 różni się tym, że zamieniona jest kolejność wykonywania działań.
	 * 
	 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_fnv1a( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie FNV-1.
	 * Wersja 64 bitowa.
	 * 
	 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 64 bitowej liczby.
	 */
	uint64_t ms_hash_64_fnv1( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie FNV-1a.
	 * Od zwykłego algorytmu FNV-1 różni się tym, że zamieniona jest kolejność wykonywania działań.
	 * Wersja 64 bitowa.
	 * 
	 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 64 bitowej liczby.
	 */
	uint64_t ms_hash_64_fnv1a( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1 w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_fnv1( const char *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1a w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_fnv1a( const char *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1 w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_mbs_64_fnv1( const char *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1a w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_mbs_64_fnv1a( const char *data );

#	endif
#	ifdef MSD_HASH_WCS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1 w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_fnv1( const wchar_t *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1a w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_fnv1a( const wchar_t *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1 w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_wcs_64_fnv1( const wchar_t *data );

		/**
		 * Funkcja skrótu oparta na algorytmie FNV-1a w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_wcs_64_fnv1a( const wchar_t *data );

#	endif
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	MURMUR ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_MURMUR
	/**
	 * Funkcja skrótu oparta na algorytmie MurmurHash3.
	 * Trzecia odsłona algorytmu MurmurHash.
	 *
	 * https://en.wikipedia.org/wiki/MurmurHash
	 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
	 * https://sites.google.com/site/murmurhash/
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_murmur3( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie MurmurHash2.
	 * Druga odsłona algorytmu MurmurHash.
	 * Budowa algorytmu uniemożliwia stworzenie bezpośrednich funkcji dedykowanych dla ciągu znaków char i wchar_t.
	 *
	 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
	 * https://sites.google.com/site/murmurhash/
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_murmur2( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie MurmurHash2.
	 * Druga odsłona algorytmu MurmurHash.
	 * Wersja 64 bitowa.
	 * Budowa algorytmu uniemożliwia stworzenie bezpośrednich funkcji dedykowanych dla ciągu znaków char i wchar_t.
	 *
	 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash2.cpp
	 * https://sites.google.com/site/murmurhash/
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 64 bitowej liczby.
	 */
	uint64_t ms_hash_64_murmur2( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie MurmurHash.
	 * Pierwsza odsłona algorytmu, przestarzała.
	 * Budowa algorytmu uniemożliwia stworzenie bezpośrednich funkcji dedykowanych dla ciągu znaków char i wchar_t.
	 * 
	 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash1.cpp
	 * https://sites.google.com/site/murmurhash/
	 * Public Domain
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_murmur1( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie Murmur3.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_murmur3( const char *data );

#	endif
#	ifdef MSD_HASH_WCS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie Murmur3.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_murmur3( const wchar_t *data );

#	endif
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	XXHASH ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#ifdef MSD_HASH_XXHASH
	/**
	 * Funkcja skrótu oparta na algorytmie xxHash.
	 * 
	 * https://github.com/Cyan4973/xxHash/blob/dev/xxhash.c
	 * https://code.google.com/archive/p/xxhash/
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint32_t ms_hash_32_xxhash( const void *data, size_t length );

	/**
	 * Funkcja skrótu oparta na algorytmie xxHash.
	 * Wersja 64 bitowa.
	 * 
	 * https://github.com/Cyan4973/xxHash/blob/dev/xxhash.c
	 * https://code.google.com/archive/p/xxhash/
	 * The 3-Clause BSD License [New BSD License] (https://opensource.org/licenses/BSD-3-Clause)
	 * 
	 * @param  data   Dane do skrócenia.
	 * @param  length Długość danych.
	 * @return        Skrót w postaci 32 bitowej liczby.
	 */
	uint64_t ms_hash_64_xxhash( const void *data, size_t length );

#	ifdef MSD_HASH_MBS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie xxHash w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_mbs_32_xxhash( const char *data );

		/**
		 * Funkcja skrótu oparta na algorytmie xxHash w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie char.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_mbs_64_xxhash( const char *data );

#	endif

#	ifdef MSD_HASH_WCS_FUNCTIONS

		/**
		 * Funkcja skrótu oparta na algorytmie xxHash w wersji 32 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 32 bitowej liczby.
		 */
		uint32_t ms_hash_wcs_32_xxhash( const wchar_t *data );

		/**
		 * Funkcja skrótu oparta na algorytmie xxHash w wersji 64 bitowej.
		 * Dedykowana dla ciągu znaków z elementami o typie wchar_t.
		 * 
		 * @param  data   Dane do skrócenia.
		 * @return        Skrót w postaci 64 bitowej liczby.
		 */
		uint64_t ms_hash_wcs_64_xxhash( const wchar_t *data );

#	endif
#endif

#endif
