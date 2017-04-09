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
 *  See LICENSE file for copyright information.
 */

#ifndef __MSH_STRING__
#define __MSH_STRING__

#include <stdlib.h>
#include <wchar.h>

#include "configuration.h"
#include "../inc/array.h"

/* dołącza plik zawierający funkcje do tworzenia skrótów */
#ifdef MSD_STRING_HASH
#	include "../inc/hash.h"
#endif

#define MSD_CONNECT(x, y) x ## y
#define MSD_CALLCONNECT(x, y) MSD_CONNECT(x, y)

/* standardowe funkcje skrótu dla ciągu znaków, używające jednego algorytmu */
#ifdef MSD_STRING_HASH
    /* dedykowana dla mbs */
#   if defined MSD_STRING_HASH_DEDICATED && defined MSD_HASH_MBS_FUNCTIONS
#       ifdef MSD_STRING_HASH_64
#           define ms_string_hash_mbs(mbs) \
                MSD_CALLCONNECT(ms_hash_mbs_64_, MSD_STRING_HASH)( mbs )
#       else
#           define ms_string_hash_mbs(mbs) \
                MSD_CALLCONNECT(ms_hash_mbs_32_, MSD_STRING_HASH)( mbs )
#       endif
#   endif
    /* dedykowana dla wcs */
#   if defined MSD_STRING_HASH_DEDICATED && defined MSD_HASH_WCS_FUNCTIONS
#       ifdef MSD_STRING_HASH_64
#           define ms_string_hash_wcs(wcs) \
                MSD_CALLCONNECT(ms_hash_wcs_64_, MSD_STRING_HASH)( wcs )
#       else
#           define ms_string_hash_wcs(wcs) \
                MSD_CALLCONNECT(ms_hash_wcs_32_, MSD_STRING_HASH)( wcs )
#       endif
#   endif
    /* ogólna */
#   ifdef MSD_STRING_HASH_64
#       define ms_string_hash_byte(data, len) \
            MSD_CALLCONNECT(ms_hash_64_, MSD_STRING_HASH)( data, len )
#   else
#   define ms_string_hash_byte(data, len) \
        MSD_CALLCONNECT(ms_hash_32_, MSD_STRING_HASH)( data, len )
#   endif
#endif

typedef struct MSS_MBINFO
{
    size_t        Offset;
    unsigned char Bytes;
}
MS_MBINFO;

typedef struct MSS_STRING
{
    union
    {
        char     *CData;
        wchar_t  *WData;
        tchar    *TData;
    };

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
int ms_string_insert_mbs( MS_STRING *str, size_t index, const char *mbstr, size_t count );
int ms_string_insert_wcs( MS_STRING *str, size_t index, const wchar_t *wcstr, size_t count );

int ms_string_insert_c( MS_STRING *str, size_t index, char chr );
int ms_string_insert_wc( MS_STRING *str, size_t index, wchar_t wchr );

size_t ms_string_hash( MS_STRING *str );

void ms_string_clear( MS_STRING *str );
void ms_string_clean( MS_STRING *str );
void ms_string_free( MS_STRING *str );

int ms_string_mbs_info( MS_ARRAY *info, const char *mbs, size_t bytes );

#endif
