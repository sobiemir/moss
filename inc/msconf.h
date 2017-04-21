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
 *  Moss Library configuration header.
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

#ifndef __MSH_CONFIGURATION__
#define __MSH_CONFIGURATION__

/* MSD_CCMACRO pozwala na definiowanie makr bezpośrednio przez kompilator */
#ifndef MSD_CCMACRO
#   define MSD_HASH_MBS_FUNCTIONS  /* funkcje skrótu zaprojektowane specjalnie dla ciągu znaków (char*) */
#   define MSD_HASH_WCS_FUNCTIONS  /* funkcje skrótu zaprojektowane specjalnie dla ciągu znaków (wchar_t*) */

#   define MSD_HASH_MURMUR         /* funkcje skrótu używające algorytmów z rodziny Murmur */
#   define MSD_HASH_JOAAT          /* funkcje skrótu używające algorytmu JOAAT */
#   define MSD_HASH_FNV            /* funkcje skrótu używające algorytmów z rodziny FNV-1 */
#   define MSD_HASH_SDBM           /* funkcje skrótu używające algorytmu SDBM */
#   define MSD_HASH_DJB            /* funkcje skrótu używające algorytmów z rodziny DJB2 */
#   define MSD_HASH_XXHASH         /* funkcje skrótu używające algorytmu xxHash */

    /* sdbm         [SDBM]
     * djb2         [DJB]
     * djb2a        [DJB]
     * joaat        [JOAAT]
     * fnv1         [FNV]
     * fnv1a        [FNV]
     * murmur1      [MURMUR]
     * murmur2      [MURMUR]
     * murmur3      [MURMUR]
     * xxhash       [XXHASH]
     */
#   define MSD_STRING_HASH sdbm     /* funkcja skrótu używana domyślnie dla ciągu znaków */
#   define MSD_STRING_HASH_64       /* preferuj 64 bitową wersję skrótu gdy jest dostępna */
#endif

/* to makro musi być, sprawdza czy kompilator je zdefiniował */
#ifndef MSD_HASH_SEED
#   define MSD_HASH_SEED 0x1C9D0367     /* wartość początkowa skrótu używana w niektórych funkcjach */
#endif

#define IGRET   /* ignorowanie zwracanej wartości przez funkcję */
#define IGVAR   /* ignorowanie zmiennej w przypadku gdy nie jest używana */

/* sprawdź czy skrót dla ciągu znaków został włączony, jeżeli tak, wymuś dołączanie danego algorytmu */
#ifdef MSD_STRING_HASH
    /* sdbm, tylko wersja 32 bitowa */
#   if MSD_STRING_HASH == sdbm
#       ifndef MSD_HASH_SDBM
#           define MSD_HASH_SDBM
#       endif
#       undef MSD_STRING_HASH_64
#       define MSD_STRING_HASH_DEDICATED

    /* djb2 / djb2a, tylko wersja 32 bitowa */
#   elif MSD_STRING_HASH == djb2 || MSD_STRING_HASH == djb2a
#       ifndef MSD_HASH_DJB
#           define MSD_HASH_DJB
#       endif
#       undef MSD_STRING_HASH_64
#       define MSD_STRING_HASH_DEDICATED

    /* joaat, tylko wersja 32 bitowa */
#   elif MSD_STRING_HASH == joaat
#       ifndef MSD_HASH_JOAAT
#           define MSD_HASH_JOAAT
#       endif
#       undef MSD_STRING_HASH_64
#       define MSD_STRING_HASH_DEDICATED

    /* fnv1 / fnv1a, bez problemów */
#   elif MSD_STRING_HASH == fnv1 || MSD_STRING_HASH == fnv1a
#       ifndef MSD_HASH_FNV
#           define MSD_HASH_FNV
#       endif
#       define MSD_STRING_HASH_DEDICATED

    /* murmur - tylko 2 ma wersje 64 bitową oraz tylko 3 algorytmy dedykowane dla ciągu znaków */
#   elif MSD_STRING_HASH == murmur1 || MSD_STRING_HASH == murmur2 || MSD_STRING_HASH == murmur3
#       ifndef MSD_HASH_MURMUR
#           define MSD_HASH_MURMUR
#       endif
#       if MSD_STRING_HASH != murmur2
#           undef MSD_STRING_HASH_64
#       endif
#       if MSD_STRING_HASH == murmur3
#           define MSD_STRING_HASH_DEDICATED
#       endif

    /* xxhash, bez problemów */
#   elif MSD_STRING_HASH == xxhash
#       ifndef MSD_HASH_XXHASH
#           define MSD_HASH_XXHASH
#       endif
#       define MSD_STRING_HASH_DEDICATED
#   endif
#endif



#define MSD_DEBUG
#define MSD_ERRORINERRNO
//#define MSD_HASSTDBOOL

#ifdef _WIN32
#   define MSD_SYSTEM_WINDOWS
#elif defined __linux__
#   define MSD_SYSTEM_LINUX
#endif

#ifdef _MSC_VER
#   define MSD_COMPILER_MSC
#elif defined __GNUC__
#   define MSD_COMPILER_GNUC
#endif

#ifdef MSD_COMPILER_MSC
#   define _CRT_SECURE_NO_WARNINGS
#endif


#ifndef INLINE
#	ifdef MSD_COMPILER_MSC
#		define INLINE __inline
#	else
#		define INLINE inline
#	endif
#endif




#ifndef __FUNCTION__
#   ifdef __func__
#       define __FUNCTION__ __func__
#   else
#       define __FUNCTION__ "(null)"
#   endif
#endif

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#ifdef MSD_HASSTDBOOL
#   include <stdbool.h>
#else
#   ifndef bool
#       define bool int
#   endif
#   ifndef true
#       define true 1
#   endif
#   ifndef false
#       define false 0
#   endif
#endif
#ifndef TRUE
#   define TRUE  1
#   define FALSE 0
#endif

#ifdef MSD_ERRORINERRNO
#   define SETERRNOANDRETURN(err) return (errno = err), err
#   define SETERRNO(err) errno = err
#   define SETERRNOANDRETURNV(err) errno = err; return
#   define SETERRNOANDRETURNC(err, ret) return (errno = err), ret
#else
#   define SETERRNOANDRETURN(err) return err
#   define SETERRNO(err)
#   define SETERRNOANDRETURNV(err) return
#   define SETERRNOANDRETURNC(err, ret) return ret
#endif

#define TEMPSWAPVALUES(tmp, a, b) tmp = a, a = b, b = tmp

// kolory w konsoli...
#define TERMCLEARLINE         "\033[2K\r"
#define TERMCOLORGREEN(_T_)   "\033[0;32;32m" _T_ "\033[0m"
#define TERMCOLORCYAN(_T_)    "\033[0;36m"    _T_ "\033[0m"
#define TERMCOLORRED(_T_)     "\033[0;32;31m" _T_ "\033[0m"
#define TERMCOLORYELLOW(_T_)  "\033[0;33m"    _T_ "\033[0m"
#define TERMCOLORMAGNETA(_T_) "\033[0;35m"    _T_ "\033[0m"
#define TERMCOLORBLUE(_T_)    "\033[0;32;34m" _T_ "\033[0m"

#define MSX_ROTL32(x, r) ((x << r) | (x >> (32 - r)))
#define MSX_ROTR32(x, r) ((x >> r) | (x << (32 - r)))
#define MSX_ROTL64(x, r) ((x << r) | (x >> (64 - r)))
#define MSX_ROTR64(x, r) ((x >> r) | (x << (64 - r)))

#define MSX_FIND8B0IN32B(bits) \
    (!(bits & 0x000000FF) \
        ? 1 \
        : !(bits & 0x0000FF00) \
        ? 2 \
        : !(bits & 0x00FF0000) \
        ? 3 \
        : !(bits & 0xFF000000) \
        ? 4 \
        : 0)

#define MSX_FIND8B0IN64B(bits) \
    (!(bits & 0x00000000000000FFull) \
        ? 1 \
        : !(bits & 0x000000000000FF00ull) \
        ? 2 \
        : !(bits & 0x0000000000FF0000ull) \
        ? 3 \
        : !(bits & 0x00000000FF000000ull) \
        ? 4 \
        : !(bits & 0x000000FF00000000ull) \
        ? 5 \
        : !(bits & 0x0000FF0000000000ull) \
        ? 6 \
        : !(bits & 0x00FF000000000000ull) \
        ? 7 \
        : !(bits & 0xFF00000000000000ull) \
        ? 8 \
        : 0)

#define MSX_FIND16B0IN32B(bits) \
    (!(bits & 0x0000FFFF) \
        ? 1 \
        : !(bits & 0xFFFF0000) \
        ? 2 \
        : 0)

#define MSX_FIND16B0IN64B(bits) \
    (!(bits & 0x000000000000FFFFull) \
        ? 1 \
        : !(bits & 0x00000000FFFF0000ull) \
        ? 2 \
        : !(bits & 0x0000FFFF00000000ull) \
        ? 3 \
        : !(bits & 0xFFFF000000000000ull) \
        ? 4 \
        : 0)

#define MSX_FIND32B0IN64B(bits) \
    (!(bits & 0x00000000FFFFFFFFull) \
        ? 1 \
        : !(bits & 0xFFFFFFFF00000000ull) \
        ? 2 \
        : 0)

typedef void*       mst_voidptr_t;
typedef long double mst_longdouble_t;
typedef long long   mst_longlong_t;

extern const bool msc_float_in_range;
extern const bool msc_double_in_range;
extern const bool msc_long_double_in_range;
extern const bool msc_voidptr_in_range;

#ifdef MSD_DEBUG

#   include <stdio.h>

    void ms_debug_display_types( void );

#endif

enum MSE_ERROR_CODE
{
    MSEC_OK = 0,
    MSEC_MEMORY_ALLOCATION = 0x8000,
    MSEC_OUT_OF_RANGE,
    MSEC_INVALID_ARGUMENT,
    MSEC_DATA_OVERFLOW,
    MSEC_INVALID_VALUE,
    
    

    MSEC_NULL_POINTER,
    MSEC_BAD_CONTENT,
    MSEC_TOO_LONG_STRING,
    MSEC_FILE_OPEN,
    MSEC_CHAIN_ALLOCATION
};

/* Domyślny rozmiar tablicy używany w przypadku podania 0 podczas inicjalizacji. */
#define MSD_ARRAY_INITIAL_SIZE 32




// -------------- MS_DEFINES -------------
// DO PRZEROBIENIA =======================================================

// #define MSD_UNICODE_STRINGS

#define MSD_PRINTF_BUFFER   1024        // rozmiar bufora funkcji ms_printf i pokrewnych
#define MSD_REPORT_BUFFER   512         // rozmiar bufora raportów
#define MSD_WCSMBS_BUFFER   256         // rozmiar bufora funkcji ms_wcsntombs i ms_mbsntowcs
#define MSD_FLT_MAX_10_EXP  2           // ilość cyfr w FLT_MAX_10_EXP
#define MSD_DBL_MAX_10_EXP  3           // ilość cyfr w DBL_MAX_10_EXP
#define MSD_LDBL_MAX_10_EXP 4           // ilość cyfr w LDBL_MAX_10_EXP

// skrócone typy danych int i domyślnie przyjmowane wartości
typedef unsigned char      uchar;       // 8 bitów
typedef unsigned short     ushort;      // 16 bit
typedef unsigned int       uint;        // 16/32 bity
typedef unsigned long      ulong;       // 32/64 bity
typedef unsigned long long ullong;      // 64 bity
typedef long long          llong;       // 64 bity
typedef long double        ldouble;     // 64/80/128 bitów

// unicode
#ifdef MSD_UNICODE_STRINGS
#   include <wchar.h>
    typedef wchar_t tchar;

#   define MSD_EOF WEOF
    
#   define MSF_TEXT(text) _MSF_TEXT(text)
#   define _MSF_TEXT(text) L##text
#   define MSF_STRLEN(text) wcslen(text)
#   define MSF_STRCPY(dst,src) wcscpy(dst,src)
#   define MSF_STRNCPY(dst,src,max) wcsncpy(dst,src,max)
#   define MSF_FPUTC(chr,file) fputwc(chr,file)
#   define MSF_FPUTS(str,file) fputws(str,file)
#   define MSF_STRCHR(str,chr) wcschr(str,chr)

#   define msd_text(a)          L##a
#   define msd_strlen(a)        wcslen(a)
#   define msd_strcpy(a,b)      wcscpy(a,b)
#   define msd_fputs(a,b)       fputws(a,b)
#   define msd_printf(a,...)    wprintf(a, ##__VA_ARGS__)
#   define msd_vsprintf(a,b,c)  vswprintf(a,1024,b,c)
#   define msd_sprintf(a,b,...) swprintf(a,1024,b,##__VA_ARGS__)
#else
#   include <string.h>
    typedef char tchar;

#   define MSD_EOF EOF
    
#   define MSF_TEXT(text)      text
#   define MSF_STRLEN(text)    strlen(text)
#   define MSF_STRCPY(dst,src) strcpy(dst,src)
#   define MSF_STRNCPY(dst,src,max) strncpy(dst,src,max)
#   define MSF_FPUTC(chr,file) fputc(chr,file)
#   define MSF_FPUTS(str,file) fputs(str,file)
#   define MSF_STRCHR(str,chr) strchr(str,chr)

#   define msd_text(a)          a
#   define msd_strlen(a)        strlen(a)
#   define msd_strcpy(a,b)      strcpy(a,b)
#   define msd_fputs(a,b)       fputs(a,b)
#   define msd_vsprintf(a,b,c)  vsprintf(a,b,c)
#   define msd_sprintf(a,b,...) sprintf(a,b,##__VA_ARGS__)
#endif

#define MSD_HASH_LOOP_UNROLL 4

#define MSX_LOOP_UNROLL_ONELINE_MACRONUM(macro, count) MSX_LOOP_UNROLL_ONELINE(macro, count)
#define MSX_LOOP_UNROLL_ONELINE(macro, count) __MSX_LOOP_UNROLL_ONELINE_ ## count ( macro )

#define __MSX_LOOP_UNROLL_ONELINE_16(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_15(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_15(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_14(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_14(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_13(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_13(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_12(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_12(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_11(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_11(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_10(_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_10(_Macro) _Macro, __MSX_LOOP_UNROLL_ONELINE_9 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_9(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_8 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_8(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_7 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_7(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_6 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_6(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_5 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_5(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_4 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_4(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_3 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_3(_Macro)  _Macro, __MSX_LOOP_UNROLL_ONELINE_2 (_Macro)
#define __MSX_LOOP_UNROLL_ONELINE_2(_Macro)  _Macro, _Macro

#define MSX_CASE_UNROLL_NOBREAK_MACRONUM(macro, count) MSX_CASE_UNROLL_NOBREAK(macro, count)
#define MSX_CASE_UNROLL_NOBREAK(macro, count) __MSX_CASE_UNROLL_NOBREAK_ ## count ( macro )

#define __MSX_CASE_UNROLL_NOBREAK_16(_Macro) case 15: _Macro; __MSX_CASE_UNROLL_NOBREAK_15(_Macro )
#define __MSX_CASE_UNROLL_NOBREAK_15(_Macro) case 14: _Macro; __MSX_CASE_UNROLL_NOBREAK_14(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_14(_Macro) case 13: _Macro; __MSX_CASE_UNROLL_NOBREAK_13(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_13(_Macro) case 12: _Macro; __MSX_CASE_UNROLL_NOBREAK_12(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_12(_Macro) case 11: _Macro; __MSX_CASE_UNROLL_NOBREAK_11(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_11(_Macro) case 10: _Macro; __MSX_CASE_UNROLL_NOBREAK_10(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_10(_Macro) case 9:  _Macro; __MSX_CASE_UNROLL_NOBREAK_9(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_9(_Macro)  case 8:  _Macro; __MSX_CASE_UNROLL_NOBREAK_8(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_8(_Macro)  case 7:  _Macro; __MSX_CASE_UNROLL_NOBREAK_7(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_7(_Macro)  case 6:  _Macro; __MSX_CASE_UNROLL_NOBREAK_6(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_6(_Macro)  case 5:  _Macro; __MSX_CASE_UNROLL_NOBREAK_5(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_5(_Macro)  case 4:  _Macro; __MSX_CASE_UNROLL_NOBREAK_4(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_4(_Macro)  case 3:  _Macro; __MSX_CASE_UNROLL_NOBREAK_3(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_3(_Macro)  case 2:  _Macro; __MSX_CASE_UNROLL_NOBREAK_2(_Macro)
#define __MSX_CASE_UNROLL_NOBREAK_2(_Macro)  case 1:  _Macro

#endif
