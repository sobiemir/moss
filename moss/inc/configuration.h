#ifndef __MSH_CONFIGURATION__
#define __MSH_CONFIGURATION__

#define MSD_DEBUG
#define MSD_ERRORINERRNO
#define MSD_HASSTDBOOL

#define IGRET
#define IGVAR

#ifdef _WIN32
#	define MSD_SYSTEM_WINDOWS
#elif defined __linux__
#	define MSD_SYSTEM_LINUX
#endif

#ifdef _MSC_VER
#	define MSD_COMPILER_MSC
#elif defined __GNUC__
#	define MSD_COMPILER_GNUC
#endif

#ifdef MSD_COMPILER_MSC
#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef __FUNCTION__
#	ifdef __func__
#		define __FUNCTION__ __func__
#	else
#		define __FUNCTION__ "(null)"
#	endif
#endif

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

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

enum MS_ERROR_CODE
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

//#define MSD_UNICODE_SUPPORT

#define MSD_PRINTF_BUFFER   1024        // rozmiar bufora funkcji ms_printf i pokrewnych
#define MSD_REPORT_BUFFER   512			// rozmiar bufora raportów
#define MSD_WCSMBS_BUFFER   256         // rozmiar bufora funkcji ms_wcsntombs i ms_mbsntowcs
#define MSD_FLT_MAX_10_EXP  2			// ilość cyfr w FLT_MAX_10_EXP
#define MSD_DBL_MAX_10_EXP  3			// ilość cyfr w DBL_MAX_10_EXP
#define MSD_LDBL_MAX_10_EXP 4			// ilość cyfr w LDBL_MAX_10_EXP

// skrócone typy danych int i domyślnie przyjmowane wartości
typedef unsigned char      uchar;		// 8 bitów
typedef unsigned short     ushort;		// 16 bit
typedef unsigned int       uint;		// 16/32 bity
typedef unsigned long      ulong;		// 32/64 bity
typedef unsigned long long ullong;		// 64 bity
typedef long long          llong;		// 64 bity
typedef long double        ldouble;		// 64/80/128 bitów

// unicode
#ifdef MSD_UNICODE_SUPPORT
#	include <wchar.h>
	typedef wchar_t tchar;

#   define MSD_EOF WEOF
    
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

#   define MSD_EOF EOF
    
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

#endif
