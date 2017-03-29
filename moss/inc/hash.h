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
 * Header file for "Hash" module...
 */

#ifndef __MSH_HASH__
#define __MSH_HASH__

#include <wchar.h>
#include "configuration.h"

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    32 BIT
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

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

uint32_t ms_hash_32_djb2_mbs( const char *data );
uint32_t ms_hash_32_djb2_wcs( const wchar_t *data );

/**
 * Funkcja skrótu oparta na algorytmie DJB2.
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


uint32_t ms_hash_32_djb2a_mbs( const char *data );
uint32_t ms_hash_32_djb2a_wcs( const wchar_t *data );

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
 * Funkcja skrótu oparta na algorytmie MurmurHash.
 * Pierwsza odsłona algorytmu, przestarzała.
 * 
 * https://github.com/aappleby/smhasher/blob/master/src/MurmurHash1.cpp
 * https://sites.google.com/site/murmurhash/
 * Public Domain
 * 
 * @param  data   Dane do skrócenia.
 * @param  length Długość danych.
 * @return        Skrót w postaci 32 bitowej liczby.
 */
uint32_t ms_hash_32_murmur( const void *data, size_t length );

/**
 * Funkcja skrótu oparta na algorytmie xxHash.
 * 
 * https://github.com/Cyan4973/xxHash/blob/dev/xxhash.c
 * https://code.google.com/archive/p/xxhash/
 * The 3-Clause BSD License [New BSD License] (https://opensource.org/licenses/BSD-3-Clause)
 * 
 * @param  data   Dane do skrócenia.
 * @param  length Długość danych.
 * @return        Skrót w postaci 32 bitowej liczby.
 */
uint32_t ms_hash_32_xxhash( const void *data, size_t length );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    64 BIT
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

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

/**
 * Funkcja skrótu oparta na algorytmie MurmurHash2.
 * Druga odsłona algorytmu MurmurHash.
 * Wersja 64 bitowa.
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

#endif
