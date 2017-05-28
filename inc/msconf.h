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

#ifndef __MSH_MSCONF__
#define __MSH_MSCONF__

/* MSD_CCMACRO pozwala na definiowanie makr bezpośrednio przez kompilator */
#ifndef MSD_CCMACRO
#	define MSD_HASH_MBS_FUNCTIONS  /* funkcje skrótu zaprojektowane specjalnie dla ciągu znaków (char*) */
#	define MSD_HASH_WCS_FUNCTIONS  /* funkcje skrótu zaprojektowane specjalnie dla ciągu znaków (wchar_t*) */

#	define MSD_HASH_MURMUR         /* funkcje skrótu używające algorytmów z rodziny Murmur */
#	define MSD_HASH_JOAAT          /* funkcje skrótu używające algorytmu JOAAT */
#	define MSD_HASH_FNV            /* funkcje skrótu używające algorytmów z rodziny FNV-1 */
#	define MSD_HASH_SDBM           /* funkcje skrótu używające algorytmu SDBM */
#	define MSD_HASH_DJB            /* funkcje skrótu używające algorytmów z rodziny DJB2 */
#	define MSD_HASH_XXHASH         /* funkcje skrótu używające algorytmu xxHash */

	/* 1.  sdbm    [SDBM]
	 * 2.  djb2    [DJB]
	 * 3.  djb2a   [DJB]
	 * 4.  joaat   [JOAAT]
	 * 5.  fnv1    [FNV]
	 * 6.  fnv1a   [FNV]
	 * 7.  murmur1 [MURMUR]
	 * 8.  murmur2 [MURMUR]
	 * 9.  murmur3 [MURMUR]
	 * 10. xxhash  [XXHASH]
	 */
#	define MSD_STRING_HASH sdbm        /* funkcja skrótu używana domyślnie dla ciągu znaków */
#	define MSD_STRING_HASH_64       /* preferuj 64 bitową wersję skrótu gdy jest dostępna */

#	define MSD_NOTERMCOLOR          /* nie wyświetlaj kolorów w terminalu */
#endif

/* Domyślny rozmiar tablicy używany w przypadku podania 0 podczas inicjalizacji. */
#ifndef MSD_ARRAY_DEFAULT_SIZE
#	define MSD_ARRAY_DEFAULT_SIZE 32
#endif

/* te makra muszą być zdefiniowane */
#ifndef MSD_HASH_SEED
#	define MSD_HASH_SEED 0x1C9D0367     /* wartość początkowa skrótu używana w niektórych funkcjach */
#endif

#define LIBEXPORT

#define IGRET          /* ignorowanie zwracanej wartości przez funkcję */
#define IGVAR (void)   /* ignorowanie zmiennej w przypadku gdy nie jest używana */

/* sprawdź czy skrót dla ciągu znaków został włączony, jeżeli tak, wymuś dołączanie danego algorytmu */
#ifdef MSD_STRING_HASH
	/* sdbm, tylko wersja 32 bitowa */
#	if MSD_STRING_HASH == sdbm
#		ifndef MSD_HASH_SDBM
#			define MSD_HASH_SDBM
#		endif
#		undef MSD_STRING_HASH_64
#		define MSD_STRING_HASH_DEDICATED

	/* djb2 / djb2a, tylko wersja 32 bitowa */
#	elif MSD_STRING_HASH == djb2 || MSD_STRING_HASH == djb2a
#		ifndef MSD_HASH_DJB
#			define MSD_HASH_DJB
#		endif
#		undef MSD_STRING_HASH_64
#		define MSD_STRING_HASH_DEDICATED

	/* joaat, tylko wersja 32 bitowa */
#	elif MSD_STRING_HASH == joaat
#		ifndef MSD_HASH_JOAAT
#			define MSD_HASH_JOAAT
#		endif
#		undef MSD_STRING_HASH_64
#		define MSD_STRING_HASH_DEDICATED

	/* fnv1 / fnv1a, bez problemów */
#	elif MSD_STRING_HASH == fnv1 || MSD_STRING_HASH == fnv1a
#		ifndef MSD_HASH_FNV
#			define MSD_HASH_FNV
#		endif
#		define MSD_STRING_HASH_DEDICATED

	/* murmur - tylko 2 ma wersje 64 bitową oraz tylko 3 algorytmy dedykowane dla ciągu znaków */
#	elif MSD_STRING_HASH == murmur1 || MSD_STRING_HASH == murmur2 || MSD_STRING_HASH == murmur3
#		ifndef MSD_HASH_MURMUR
#			define MSD_HASH_MURMUR
#		endif
#		if MSD_STRING_HASH != murmur2
#			undef MSD_STRING_HASH_64
#		endif
#		if MSD_STRING_HASH == murmur3
#			define MSD_STRING_HASH_DEDICATED
#		endif

	/* xxhash, bez problemów */
#	elif MSD_STRING_HASH == xxhash
#		ifndef MSD_HASH_XXHASH
#			define MSD_HASH_XXHASH
#		endif
#		define MSD_STRING_HASH_DEDICATED
#	endif
#endif

/* próbuj wykryć system, pod którym działa preprocesor */
#ifdef _WIN32
#	define MSD_SYSTEM_WINDOWS
#elif defined __linux__
#	define MSD_SYSTEM_LINUX
#endif

/* próbuj wykryć kompilator */
#ifdef _MSC_VER
#	define MSD_COMPILER_MSC
#elif defined __GNUC__
#	define MSD_COMPILER_GNUC
#endif

/* wyłącz ostrzeżenia o niebezpiecznych funkcjach w MSC */
#ifdef MSD_COMPILER_MSC
#	define _CRT_SECURE_NO_WARNINGS
#	define PFSIZET   "Iu"
#	define PFSIZETLH "Ix"
#	define PFSIZETUH "IX"
#else
#	define PFSIZET   "zu"
#	define PFSIZETLH "zx"
#	define PFSIZETUH "zX"
#endif

/* różny inline w zależności od kompilatora */
#ifndef INLINE
#	ifdef MSD_COMPILER_MSC
#		define INLINE __inline
#	else
#		define INLINE inline
#	endif
#endif

/* wykrywanie nazwy funkcji w której makro zostało uruchomione */
#ifndef __FUNCTION__
#	ifdef __func__
#		define __FUNCTION__ __func__
#	else
#		define __FUNCTION__ "(null)"
#	endif
#endif

#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <errno.h>

/* typ bool dostępny jest od C99
   w C++ bool jest typem wbudowanym
   UWAGA na kompilator CL, który ustawia __cplusplus nawet gdy kompiluje w C! */
#if (defined __STDC_VERSION__ && (__STDC_VERSION__ >= 199901L)) || defined(__cplusplus)
#	include <stdbool.h>

#	ifndef TRUE
#		define TRUE true
#	endif
#	ifndef FALSE
#		define FALSE false
#	endif
#else
	typedef int bool;

#	ifndef TRUE
#		define TRUE 1
#	endif
#	ifndef FALSE
#		define FALSE 0
#	endif
#endif

typedef void*              voidptr;       /* wskaźnik na void */
typedef unsigned char      uchar;         /* 8 bitów */
typedef unsigned short     ushort;        /* 16 bit */
typedef unsigned int       uint;          /* 16/32 bity */
typedef unsigned long      ulong;         /* 32/64 bity */
typedef long long          llong;         /* 64 bity, może go nie być w starszych wersjach */
typedef unsigned long long ullong;        /* 64 bity, wersja bez znaku */
typedef long double        ldouble;       /* 64/80/128 bitów, może go nie być w starszych wersjach */

/* kolory w terminalu */
#ifdef MSD_NOTERMCOLOR
#	define TERMCOLORGREEN(_T_)   _T_
#	define TERMCOLORCYAN(_T_)    _T_
#	define TERMCOLORRED(_T_)     _T_
#	define TERMCOLORYELLOW(_T_)  _T_
#	define TERMCOLORMAGNETA(_T_) _T_
#	define TERMCOLORBLUE(_T_)    _T_
#else
#	define TERMCOLORGREEN(_T_)   "\033[0;32;32m" _T_ "\033[0m"
#	define TERMCOLORCYAN(_T_)    "\033[0;36m"    _T_ "\033[0m"
#	define TERMCOLORRED(_T_)     "\033[0;32;31m" _T_ "\033[0m"
#	define TERMCOLORYELLOW(_T_)  "\033[0;33m"    _T_ "\033[0m"
#	define TERMCOLORMAGNETA(_T_) "\033[0;35m"    _T_ "\033[0m"
#	define TERMCOLORBLUE(_T_)    "\033[0;32;34m" _T_ "\033[0m"
#endif

/* łączenie elementów */
#define MSD_CONNECT(x, y) x ## y
#define MSD_CALLCONNECT(x, y) MSD_CONNECT(x, y)

/* debugowanie i ustawianie błędów w errno */
#define MSD_DEBUG
#define MSD_ERRORINERRNO

/* zwracanie i ustawianie błędu w zmiennej errno */
#ifdef MSD_ERRORINERRNO
#	define SETERRNOANDRETURN(err) return (errno = err), err
#	define SETERRNO(err) errno = err
#	define SETERRNOANDRETURNV(err) errno = err; return
#	define SETERRNOANDRETURNC(err, ret) return (errno = err), ret
/* oczywiście ustawianie w errno można wyłączyć */
#else
#	define SETERRNOANDRETURN(err) return err
#	define SETERRNO(err)
#	define SETERRNOANDRETURNV(err) return
#	define SETERRNOANDRETURNC(err, ret) return ret
#endif

/* szacunkowa ilość liczb w podanym typie liczbowym */
#define ESTIMATEDINTSIZE(type) (3 * sizeof(type) * CHAR_BIT / 8)

/* tworzy wyrażenie z podwyrażeń, używając podanego pośrodku komparatora */
#define EXPRESSIONMAKE(left, compare, right) \
	STRINGIFY(left) " " TERMCOLORYELLOW(STRINGIFY(compare)) " " STRINGIFY(right)

#define TEMPSWAPVALUES(tmp, a, b) tmp = a, a = b, b = tmp


#define STRINGIFY(X) #X
#define DBLSTRINGIFY(X) STRINGIFY(X)

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




// -------------- MS_DEFINES -------------
// DO PRZEROBIENIA =======================================================

// #define MSD_UNICODE_STRINGS

#define MSD_PRINTF_BUFFER   1024        // rozmiar bufora funkcji ms_printf i pokrewnych
#define MSD_REPORT_BUFFER   512         // rozmiar bufora raportów
#define MSD_WCSMBS_BUFFER   256         // rozmiar bufora funkcji ms_wcsntombs i ms_mbsntowcs
#define MSD_FLT_MAX_10_EXP  2           // ilość cyfr w FLT_MAX_10_EXP
#define MSD_DBL_MAX_10_EXP  3           // ilość cyfr w DBL_MAX_10_EXP
#define MSD_LDBL_MAX_10_EXP 4           // ilość cyfr w LDBL_MAX_10_EXP

// unicode
#ifdef MSD_UNICODE_STRINGS
#	include <wchar.h>
	typedef wchar_t tchar;

#	define MSD_EOF WEOF
	
#	define MSF_TEXT(text) _MSF_TEXT(text)
#	define _MSF_TEXT(text) L##text
#	define MSF_STRLEN(text) wcslen(text)
#	define MSF_STRCPY(dst,src) wcscpy(dst,src)
#	define MSF_STRNCPY(dst,src,max) wcsncpy(dst,src,max)
#	define MSF_FPUTC(chr,file) fputwc(chr,file)
#	define MSF_FPUTS(str,file) fputws(str,file)
#	define MSF_STRCHR(str,chr) wcschr(str,chr)

#	define msd_text(a)          L##a
#	define msd_strlen(a)        wcslen(a)
#	define msd_strcpy(a,b)      wcscpy(a,b)
#	define msd_fputs(a,b)       fputws(a,b)
#	define msd_printf(a,...)    wprintf(a, ##__VA_ARGS__)
#	define msd_vsprintf(a,b,c)  vswprintf(a,1024,b,c)
#	define msd_sprintf(a,b,...) swprintf(a,1024,b,##__VA_ARGS__)
#else
#	include <string.h>
	typedef char tchar;

#	define MSD_EOF EOF
	
#	define MSF_TEXT(text)      text
#	define MSF_STRLEN(text)    strlen(text)
#	define MSF_STRCPY(dst,src) strcpy(dst,src)
#	define MSF_STRNCPY(dst,src,max) strncpy(dst,src,max)
#	define MSF_FPUTC(chr,file) fputc(chr,file)
#	define MSF_FPUTS(str,file) fputs(str,file)
#	define MSF_STRCHR(str,chr) strchr(str,chr)

#	define msd_text(a)          a
#	define msd_strlen(a)        strlen(a)
#	define msd_strcpy(a,b)      strcpy(a,b)
#	define msd_fputs(a,b)       fputs(a,b)
#	define msd_vsprintf(a,b,c)  vsprintf(a,b,c)
#	define msd_sprintf(a,b,...) sprintf(a,b,##__VA_ARGS__)
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
