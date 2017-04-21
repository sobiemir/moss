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

#include "../inc/buffer.h"

/**
 * Flagi specyfikatorów.
 * Używane tylko i wyłącznie w funkcjach z rodziny vsprintf.
**/
struct MSS_PRINTF_FLAGS
{
    size_t to_left   :1,        // Przyleganie do lewej strony.
           center    :1,        // Środkowanie tekstu (tylko tekst).
           zerofill  :1,        // Wypełnianie zerami liczby z lewej strony.
           alternate :1,        // Automatyczna szerokość w n lub obcinanie zer w f/lf.
           text_space:1,        // Spacja przed tekstem.
           force_char:2,        // Znak przed liczbą całkowitą lub zmiennoprzecinkową (+, - lub spacja).
           is_signed :1,        // Liczba całkowita - jeżeli brak, liczba naturalna.
           is_pointer:1,        // Wskaźnik - tylko dla zapisu szesnastkowego.
           type      :4;        // Typ zapisu liczby - MSE_TYPE_CONVERT (MSTC)
};


/* ============================================================================== ms_wcsntombs / 3 ==================================== */

size_t ms_wcsntombs( char *dest, const wchar_t *src, size_t length, size_t max )
{
#if defined MSD_SYSTEM_WINDOWS
    // windows
    errno_t error;
    size_t  copied;
    
    error = wcstombs_s( &copied, dest, length, src, max );
    return error ? (size_t)-1 : copied-1;
#elif defined MSD_SYSTEM_LINUX
    // linux
    mbstate_t state = {0};
    if( !mbsinit(&state) )
        memset( &state, 0, sizeof(state) );
    return wcsnrtombs( dest, &src, max, length, &state ); 
#else
    // metoda uniwersalna
    wchar_t buffer[MSD_WCSMBS_BUFFER];

#   ifdef MSD_PROGRAM_DEBUG
        // ograniczenie nie może przekraczać rozmiaru buforu
        if( count > MSD_WCSMBS_BUFFER )
            return 0;
#   endif

    // kopiuj ograniczoną ilość znaków
    wcsncpy( buffer, src, max );
    buffer[max-1] = 0;

    // konwertuj znaki
    return wcstombs( dest, buffer, length );
#endif
}
// -------------
size_t ms_wcsntotcs( tchar *dest, const wchar_t *src, size_t length, size_t max )
{
#ifdef MSD_UNICODE_SUPPORT
    dest = (wchar_t*)src;
    return wcslen( src );
#else
    return ms_wcsntombs( dest, src, length, max );
#endif
}
// -------------
size_t ms_tcsntombs( char *dest, const tchar *src, size_t length, size_t max )
{
#ifdef MSD_UNICODE_SUPPORT
    return ms_wcsntombs( dest, src, length, max );
#else
    dest = (char*)src;
    return strlen( src );
#endif
}


/* ============================================================================== ms_mbsntowcs / 3 ==================================== */

size_t ms_mbsntowcs( wchar_t *dest, const char *src, size_t length, size_t max )
{
#if defined MSD_SYSTEM_WINDOWS
    // windows
    errno_t error;
    size_t  copied;
    
    error = mbstowcs_s( &copied, dest, length, src, max );
    return error ? (size_t)-1 : copied-1;
#elif defined MSD_SYSTEM_LINUX
    // linux
    mbstate_t state = {0};
    if( !mbsinit(&state) )
        memset( &state, 0, sizeof(state) );
    return mbsnrtowcs( dest, &src, max, length, &state );
#else
    // metoda uniwersalna
    char buffer[MSD_WCSMBS_BUFFER];

#   ifdef MSD_PROGRAM_DEBUG
        // ograniczenie nie może przekraczać rozmiaru buforu
        if( count > MSD_WCSMBS_BUFFER )
            return 0;
#   endif

    // kopiuj ograniczoną ilość znaków
    strncpy( buffer, src, max );
    buffer[max-1] = 0;

    // konwertuj znaki
    return mbstowcs( dest, buffer, length );
#endif
}
// -------------
size_t ms_mbsntotcs( tchar *dest, const char *src, size_t length, size_t max )
{
#ifdef MSD_UNICODE_SUPPORT
    return ms_mbsntowcs( dest, src, length, max );
#else
    dest = (char*)src;
    return strlen( src );
#endif
}
// -------------
size_t mb_tcsntowcs( wchar_t *dest, const tchar *src, size_t length, size_t max )
{
#ifdef MSD_UNICODE_SUPPORT
    dest = (wchar_t*)src;
    return wcslen( src );
#else
    return ms_mbsntowcs( dest, src, length, max );
#endif
}


/* ============================================================================== ms_uitos / 3 ======================================== #s
    buffer += limit-1, *buffer-- = 0, limit -= 2;
    if( !number && limit )
    {
        *buffer = '0';
        return limit;
    }
    switch( type )
    {
    // system dwójkowy (binarny)
    case MSTC_BINARY:
        while( number && limit )
            *buffer-- = (number & 0x1) + '0', --limit, number >>= 0x1;
    break;
    // system czwórkowy
    case MSTC_QUATERNARY:
        while( number && limit )
            *buffer-- = (number & 0x3) + '0', --limit, number >>= 0x2;
    break;
    // system ósemkowy (oktalny)
    case MSTC_OCTAL:
        while( number && limit )
            *buffer-- = (number & 0x7) + '0', --limit, number >>= 0x3;
    break;
    // system szesnastkowy (heksadecymalny) - duże litery
    case MSTC_HEXDEC_UPCASE:
        while( number && limit )
            *buffer-- = (number & 0xF) > 9 ? (number & 0xF)+'7' : (number & 0xF)+'0', --limit, number >>= 0x4;
    break;
    // system szesnastkowy (heksadecymalny) - małe litery
    case MSTC_HEXDEC_LOCASE:
        while( number && limit )
            *buffer-- = (number & 0xF) > 9 ? (number & 0xF)+'W' : (number & 0xF)+'0', --limit, number >>= 0x4;
    break;
    // system dziesiętny
    default:
        while( number && limit )
            *buffer-- = number % 10 + '0', --limit, number /= 10;
    }
    return limit + 1;
#e ============================================================================== ms_uitoa / 3 ======================================== */

size_t ms_uitoa( char *buffer, size_t limit, uint number, int type )
{
#define __MST_UINT2STR                                                                                                                  \
    buffer += limit-1, *buffer-- = 0, limit -= 2;                                                                                       \
    if( !number && limit )                                                                                                              \
    {                                                                                                                                   \
        *buffer = '0';                                                                                                                  \
        return limit;                                                                                                                   \
    }                                                                                                                                   \
    switch( type )                                                                                                                      \
    {                                                                                                                                   \
    /* system dwójkowy (binarny) */                                                                                                     \
    case MSTC_BINARY:                                                                                                                   \
        while( number && limit )                                                                                                        \
            *buffer-- = (number & 0x1) + '0', --limit, number >>= 0x1;                                                                  \
    break;                                                                                                                              \
    case MSTC_QUATERNARY:                                                                                                               \
        while( number && limit )                                                                                                        \
            *buffer-- = (number & 0x3) + '0', --limit, number >>= 0x2;                                                                  \
    break;                                                                                                                              \
    /* system ósemkowy */                                                                                                               \
    case MSTC_OCTAL:                                                                                                                    \
        while( number && limit )                                                                                                        \
            *buffer-- = (number & 0x7) + '0', --limit, number >>= 0x3;                                                                  \
    break;                                                                                                                              \
    /* system szesnastkowy (duże litery) */                                                                                             \
    case MSTC_HEXDEC_UPCASE:                                                                                                            \
        while( number && limit )                                                                                                        \
            *buffer-- = (number & 0xF) > 9 ? (number & 0xF)+'7' : (number & 0xF)+'0', --limit, number >>= 0x4;                          \
    break;                                                                                                                              \
    /* system szesnastkowy (małe litery) */                                                                                             \
    case MSTC_HEXDEC_LOCASE:                                                                                                            \
        while( number && limit )                                                                                                        \
            *buffer-- = (number & 0xF) > 9 ? (number & 0xF)+'W' : (number & 0xF)+'0', --limit, number >>= 0x4;                          \
    break;                                                                                                                              \
    /* system dziesiętny */                                                                                                             \
    default:                                                                                                                            \
        while( number && limit )                                                                                                        \
            *buffer-- = number % 10 + '0', --limit, number /= 10;                                                                       \
    }                                                                                                                                   \
    return limit + 1

    __MST_UINT2STR;
}
// -------------
size_t ms_uitow( wchar_t *buffer, size_t limit, uint number, int type )
{
    __MST_UINT2STR;
}
// -------------
size_t ms_uitos( tchar *buffer, size_t limit, uint number, int type )
{
    __MST_UINT2STR;
}


/* ============================================================================== ms_ultos / 3 ======================================== */

size_t ms_ultoa( char *buffer, size_t limit, ulong number, int type )
{
    __MST_UINT2STR;
}
// -------------
size_t ms_ultow( wchar_t *buffer, size_t limit, ulong number, int type )
{
    __MST_UINT2STR;
}
// -------------
size_t ms_ultos( tchar *buffer, size_t limit, ulong number, int type )
{
    __MST_UINT2STR;
}


/* ============================================================================== ms_ulltos / 3 ======================================= */

size_t ms_ulltoa( char *buffer, size_t limit, ullong number, int type )
{
    __MST_UINT2STR;
}
// -------------
size_t ms_ulltow( wchar_t *buffer, size_t limit, ullong number, int type )
{
    __MST_UINT2STR;
}
// -------------
size_t ms_ulltos( tchar *buffer, size_t limit, ullong number, int type )
{
    __MST_UINT2STR;
}


/* ============================================================================== ms_itos / 3 ========================================= #s
    if( number < 0 )
    {
        limit = ms_uitoa( buffer, limit, (uint)-number, MSTC_DECIMAL );
        if( limit )
            --limit, buffer[limit] = '-';
    }
    else
        limit = ms_uitoa( buffer, limit, (uint)number, MSTC_DECIMAL );
    return limit;
#e ============================================================================== ms_itos / 3 ========================================= */

size_t ms_itoa( char *buffer, size_t limit, int number )
{
#define __MST_INT2STR(_iType,_fName)                                                                                                    \
    if( number < 0 )                                                                                                                    \
    {                                                                                                                                   \
        limit = _fName( buffer, limit, (_iType)-number, MSTC_DECIMAL );                                                                 \
        if( limit )                                                                                                                     \
            --limit, buffer[limit] = '-';                                                                                               \
    }                                                                                                                                   \
    else                                                                                                                                \
        limit = _fName( buffer, limit, (_iType)number, MSTC_DECIMAL );                                                                  \
    return limit

    __MST_INT2STR( uint, ms_uitoa );
}
// -------------
size_t ms_itow( wchar_t *buffer, size_t limit, int number )
{
    __MST_INT2STR( uint, ms_uitow );
}
// -------------
size_t ms_itos( tchar *buffer, size_t limit, int number )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_INT2STR( uint, ms_uitow );
#else
    __MST_INT2STR( uint, ms_uitoa );
#endif
}


/* ============================================================================== ms_ltos / 3 ========================================= */

size_t ms_ltoa( char *buffer, size_t limit, long number )
{
    __MST_INT2STR( ulong, ms_ultoa );
}
// -------------
size_t ms_ltow( wchar_t *buffer, size_t limit, long number )
{
    __MST_INT2STR( ulong, ms_ultow );
}
// -------------
size_t ms_ltos( tchar *buffer, size_t limit, long number )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_INT2STR( ulong, ms_ultow );
#else
    __MST_INT2STR( ulong, ms_ultoa );
#endif
}


/* ============================================================================== ms_lltos / 3 ======================================== */

size_t ms_lltoa( char *buffer, size_t limit, llong number )
{
    __MST_INT2STR( ullong, ms_ulltoa );
}
// -------------
size_t ms_lltow( wchar_t *buffer, size_t limit, llong number )
{
    __MST_INT2STR( ullong, ms_ulltow );
}
// -------------
size_t ms_lltos( tchar *buffer, size_t limit, llong number )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_INT2STR( ullong, ms_ulltow );
#else
    __MST_INT2STR( ullong, ms_ulltoa );
#endif
}


/* ============================================================================== ms_dtos / 3 ========================================= #s
    double fract, helper;
    size_t start, curpos = limit - precise - 3, minus = 0, logf10;

    // liczba ujemna
    if( number < 0 )
        minus |= 0x1, number = -number;
    if( limit < precise + 6 + MSD_DBL_MAX_10_EXP )
        return limit;
    buffer[limit-1] = 0, limit -= 2;
    
    // notacja naukowa
    if( type >= MSTC_SCIENT_UPCASE )
    {
        start = curpos -= 2 + MSD_DBL_MAX_10_EXP;
        if( !number )
        {
            buffer[curpos++] = '0', buffer[curpos++] = '.', logf10 = 0, minus = 0;
            while( precise )
                --precise, buffer[curpos++] = '0';
        }
        else
        {
            // w przypadku ujemnego wyniku liczba ma tylko częśc ułamkową
            helper = log10( number );
            if( helper < 0.0 )
                logf10 = (size_t)-helper + 1, minus |= 0x2, number *= pow( 10.0, (double)logf10 );
            else
                logf10 = (size_t)helper, number /= pow( 10.0, (double)logf10 );
            fract = modf( number, &number ), buffer[curpos++] = (int)number + '0', buffer[curpos++] = '.', type = 0;

            // minus
            if( minus & 0x1 )
                --start, buffer[start] = '-';
            
            // precyzja jest większa niż ilość cyfr znaczących - uzupełnianie zerami
            if( precise > DBL_DIG + 1 )
                type = precise - DBL_DIG - 1, precise = DBL_DIG + 1;
            
            // zaokrąglenie liczby
            if( !type )
                fract += 0.5 / pow( 10.0, (double)precise );

            // zamiana na ciąg znaków
            while( precise )
                --precise, fract = modf( fract * 10.0, &number ), buffer[curpos++] = (int)number + '0';
            while( type )
                --type, buffer[curpos++] = '0';
        }
        // uzupełnij przesunięcie (E+XXX)
        buffer[curpos++] = type == MSTC_SCIENT_UPCASE ? 'E' : 'e';
        buffer[curpos++] = minus & 0x2 ? '-' : '+', minus = MSD_DBL_MAX_10_EXP;
        
        while( minus )
            --minus, buffer[curpos++] = '0';
        if( logf10 )
            ms_itoa( buffer + (limit - MSD_DBL_MAX_10_EXP), MSD_DBL_MAX_10_EXP + 2, logf10 );
    }
    // system dziesiętny
    else
    {
        // oddziel część dziesiętną od liczby
        fract = modf( number, &number );
        if( !number )
            minus |= 0x2, start = curpos, buffer[start] = '0';
        else if( number <= (double)LLONG_MAX )
        {
_MSG_DTOA_DBL_NUMBER:
            // zamiana cyfry na znak
            while( number && curpos )
                helper = modf( number / 10.0, &number ), buffer[curpos--] = (int)(helper * 10.0 + 0.5) + '0';
            start = curpos + 1, curpos = limit - precise - 1, minus |= (curpos + 1 - start) << 2;
        }
        else
        {
            // przesunięcie - ilość cyfr
            logf10 = (size_t)log10( number ) + 1;
            
            // usuwanie zer w przypadku zbyt dużego przesunięcia
            if( logf10 > DBL_DIG + 2 )
                number /= pow( 10.0, (double)(logf10 - DBL_DIG - 2) ), logf10 -= DBL_DIG + 2, curpos -= logf10;
            else
                logf10 = 0;

            // wypełnianie zerami
            if( logf10 )
            {
                while( logf10 )
                    --logf10, buffer[curpos--] = '0';
                curpos = limit - precise - 1;
            }
            goto _MSG_DTOA_DBL_NUMBER;
        }
        // separator i minus
        buffer[curpos+1] = '.', curpos += 2;
        if( minus & 0x1 )
            --start, buffer[start] = '-';

        // część dziesiętna
        if( !fract )
            while( precise )
                --precise, buffer[curpos++] = '0';
        else if( precise <= DBL_DIG + 2 )
        {
            // zaokrąglanie liczby
            fract += 0.5 / pow( 10.0, (double)precise );
_MSG_DTOA_DBL_FRACT:
            // zamiana części dziesiętnej na znaki
            while( precise )
                --precise, fract = modf( fract * 10.0, &helper ), buffer[curpos++] = (int)helper + '0';
        }
        else
        {
            // liczenie przesunięcia cyfr w części dziesiętnej i uzupełnianie zerami przed liczbą
            if( minus & 0x2 )
            {
                logf10 = (size_t)-log10( fract ), precise -= logf10, fract *= pow( 10.0, (double)logf10 );
                while( logf10 )
                    --logf10, buffer[curpos++] = '0';
            }
            minus >>= 2;

            // uzupełnianie zerami po liczbie
            if( precise > DBL_DIG + 2 - minus )
            {
                logf10 = precise - (DBL_DIG + 2 - minus), precise = DBL_DIG + 2 - minus, minus = curpos, curpos += precise;
                while( logf10 )
                    --logf10, buffer[curpos++] = '0';
                curpos = minus;
            }
            goto _MSG_DTOA_DBL_FRACT;
        }
        // obcinanie zer i przesuwanie liczby
        if( type == MSTC_FLOAT_NO_ZERO )
        {
            curpos = minus = limit;
            while( buffer[curpos] == '0' )
                --curpos;

            if( buffer[curpos] == '.' )
                ++curpos;

            while( curpos >= start )
                buffer[minus--] = buffer[curpos--];
            start = minus + 1;
        }
    }
    return start;
#e ============================================================================== ms_dtoa / 3 ========================================= */

size_t ms_dtoa( char *buffer, size_t limit, double number, int type, size_t precise )
{
#define __MST_FLT2STR(_iType,_fModf,_fPow,_fLog10,_fItox,_sName,_gName)                                                                 \
    _iType fract, helper;                                                                                                               \
    size_t start, curpos = limit - precise - 3, minus = 0, logf10;                                                                      \
    /* liczba ujemna */                                                                                                                 \
    if( number < 0 )                                                                                                                    \
        minus |= 0x1, number = -number;                                                                                                 \
    if( limit < precise + 6 + MSD_ ## _sName ## _MAX_10_EXP )                                                                           \
        return limit;                                                                                                                   \
    buffer[limit-1] = 0, limit -= 2;                                                                                                    \
    /* notacja naukowa */                                                                                                               \
    if( type >= MSTC_SCIENT_UPCASE )                                                                                                    \
    {                                                                                                                                   \
        start = curpos -= 2 + MSD_ ## _sName ## _MAX_10_EXP;                                                                            \
        /* zero */                                                                                                                      \
        if( !number )                                                                                                                   \
        {                                                                                                                               \
            buffer[curpos++] = '0', buffer[curpos++] = '.', logf10 = 0, minus = 0;                                                      \
            while( precise )                                                                                                            \
                --precise, buffer[curpos++] = '0';                                                                                      \
        }                                                                                                                               \
        else                                                                                                                            \
        {                                                                                                                               \
            /* w przypadku ujemnego wyniku liczba ma tylko częśc ułamkową */                                                            \
            helper = _fLog10( number );                                                                                                 \
            if( helper < 0.0 )                                                                                                          \
                logf10 = (size_t)-helper + 1, minus |= 0x2, number *= _fPow( 10.0, (_iType)logf10 );                                    \
            else                                                                                                                        \
                logf10 = (size_t)helper, number /= _fPow( 10.0, (_iType)logf10 );                                                       \
            fract = _fModf( number, &number ), buffer[curpos++] = (int)number + '0', buffer[curpos++] = '.', type = 0;                  \
            /* minus */                                                                                                                 \
            if( minus & 0x1 )                                                                                                           \
                --start, buffer[start] = '-';                                                                                           \
            /* precyzja jest większa niż ilość cyfr znaczących - uzupełnianie zerami */                                                 \
            if( precise > _sName ## _DIG + 1 )                                                                                          \
                type = precise - _sName ## _DIG - 1, precise = _sName ## _DIG + 1;                                                      \
            /* zaokrąglenie liczby */                                                                                                   \
            if( !type )                                                                                                                 \
                fract += (_iType)0.5 / _fPow( 10.0, (_iType)precise );                                                                  \
            /* zamiana na ciąg znaków */                                                                                                \
            while( precise )                                                                                                            \
                --precise, fract = _fModf( fract * 10.0, &number ), buffer[curpos++] = (int)number + '0';                               \
            while( type )                                                                                                               \
                --type, buffer[curpos++] = '0';                                                                                         \
        }                                                                                                                               \
        /* uzupełnij przesunięcie (E+XXX) */                                                                                            \
        buffer[curpos++] = type == MSTC_SCIENT_UPCASE ? 'E' : 'e';                                                                      \
        buffer[curpos++] = minus & 0x2 ? '-' : '+', minus = MSD_ ## _sName ## _MAX_10_EXP;                                              \
        while( minus )                                                                                                                  \
            --minus, buffer[curpos++] = '0';                                                                                            \
        if( logf10 )                                                                                                                    \
            _fItox( buffer + (limit - MSD_ ## _sName ## _MAX_10_EXP), MSD_ ## _sName ## _MAX_10_EXP + 2, logf10 );                      \
    }                                                                                                                                   \
    /* system dziesiętny */                                                                                                             \
    else                                                                                                                                \
    {                                                                                                                                   \
        /* oddziel część dziesiętną od liczby */                                                                                        \
        fract = _fModf( number, &number );                                                                                              \
        if( !number )                                                                                                                   \
            minus |= 0x2, start = curpos, buffer[start] = '0';                                                                          \
        else if( number <= (_iType)LLONG_MAX )                                                                                          \
        {                                                                                                                               \
_MSG_FTOA_ ## _gName ## _NUMBER:                                                                                                        \
            /* zamiana cyfry na znak */                                                                                                 \
            while( number && curpos )                                                                                                   \
                helper = _fModf( number / 10.0, &number ), buffer[curpos--] = (int)(helper * 10.0 + 0.5) + '0';                         \
            start = curpos + 1, curpos = limit - precise - 1, minus |= (curpos + 1 - start) << 2;                                       \
        }                                                                                                                               \
        else                                                                                                                            \
        {                                                                                                                               \
            /* przesunięcie - ilość cyfr */                                                                                             \
            logf10 = (size_t)_fLog10( number ) + 1;                                                                                     \
            /* usuwanie zer w przypadku zbyt dużego przesunięcia */                                                                     \
            if( logf10 > _sName ## _DIG + 2 )                                                                                           \
                number /= _fPow( 10.0, (_iType)(logf10 - _sName ## _DIG - 2) ), logf10 -= _sName ## _DIG + 2, curpos -= logf10;         \
            else                                                                                                                        \
                logf10 = 0;                                                                                                             \
            /* wypełnianie zerami */                                                                                                    \
            if( logf10 )                                                                                                                \
            {                                                                                                                           \
                while( logf10 )                                                                                                         \
                    --logf10, buffer[curpos--] = '0';                                                                                   \
                curpos = limit - precise - 1;                                                                                           \
            }                                                                                                                           \
            goto _MSG_FTOA_ ## _gName ## _NUMBER;                                                                                       \
        }                                                                                                                               \
        /* separator i minus */                                                                                                         \
        buffer[curpos+1] = '.', curpos += 2;                                                                                            \
        if( minus & 0x1 )                                                                                                               \
            --start, buffer[start] = '-';                                                                                               \
        /* część dziesiętna */                                                                                                          \
        if( !fract )                                                                                                                    \
            while( precise )                                                                                                            \
                --precise, buffer[curpos++] = '0';                                                                                      \
        else if( precise <= _sName ## _DIG + 2 )                                                                                        \
        {                                                                                                                               \
            /* zaokrąglanie liczby */                                                                                                   \
            fract += (_iType)0.5 / _fPow( 10.0, (_iType)precise );                                                                      \
_MSG_FTOA_ ## _gName ## _FRACT:                                                                                                         \
            /* zamiana części dziesiętnej na znaki */                                                                                   \
            while( precise )                                                                                                            \
                --precise, fract = _fModf( fract * 10.0, &helper ), buffer[curpos++] = (int)helper + '0';                               \
        }                                                                                                                               \
        else                                                                                                                            \
        {                                                                                                                               \
            /* liczenie przesunięcia cyfr w części dziesiętnej i uzupełnianie zerami przed liczbą */                                    \
            if( minus & 0x2 )                                                                                                           \
            {                                                                                                                           \
                logf10 = (size_t)-_fLog10( fract ), precise -= logf10, fract *= _fPow( 10.0, (_iType)logf10 );                          \
                while( logf10 )                                                                                                         \
                    --logf10, buffer[curpos++] = '0';                                                                                   \
            }                                                                                                                           \
            minus >>= 2;                                                                                                                \
            /* uzupełnianie zerami po liczbie */                                                                                        \
            if( precise > _sName ## _DIG + 2 - minus )                                                                                  \
            {                                                                                                                           \
                logf10 = precise-(_sName ## _DIG + 2 - minus), precise = _sName ## _DIG + 2 - minus, minus = curpos, curpos += precise; \
                while( logf10 )                                                                                                         \
                    --logf10, buffer[curpos++] = '0';                                                                                   \
                curpos = minus;                                                                                                         \
            }                                                                                                                           \
            goto _MSG_FTOA_ ## _gName ## _FRACT;                                                                                        \
        }                                                                                                                               \
        /* obcinanie zer i przesuwanie liczby */                                                                                        \
        if( type == MSTC_FLOAT_NO_ZERO )                                                                                                \
        {                                                                                                                               \
            curpos = minus = limit;                                                                                                     \
            while( buffer[curpos] == '0' )                                                                                              \
                --curpos;                                                                                                               \
            if( buffer[curpos] == '.' )                                                                                                 \
                ++curpos;                                                                                                               \
            while( curpos >= start )                                                                                                    \
                buffer[minus--] = buffer[curpos--];                                                                                     \
            start = minus + 1;                                                                                                          \
        }                                                                                                                               \
    }                                                                                                                                   \
    return start;

    __MST_FLT2STR( double, modf, pow, log10, ms_itoa, DBL, DBL );
}
// -------------
size_t ms_dtow( wchar_t *buffer, size_t limit, double number, int type, size_t precise )
{
    __MST_FLT2STR( double, modf, pow, log10, ms_itow, DBL, DBLW );
}
// -------------
size_t ms_dtos( tchar *buffer, size_t limit, double number, int type, size_t precise )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_FLT2STR( double, modf, pow, log10, ms_itow, DBL, DBLT );
#else
    __MST_FLT2STR( double, modf, pow, log10, ms_itoa, DBL, DBLT );
#endif
}


/* ============================================================================== ms_ldtos / 3 ======================================== */

size_t ms_ldtoa( char *buffer, size_t limit, ldouble number, int type, size_t precise )
{
    __MST_FLT2STR( ldouble, modfl, powl, log10l, ms_itoa, LDBL, LDBL );
}
// -------------
size_t ms_ldtow( wchar_t *buffer, size_t limit, ldouble number, int type, size_t precise )
{
    __MST_FLT2STR( ldouble, modfl, powl, log10l, ms_itow, LDBL, LDBLW );
}
// -------------
size_t ms_ldtos( tchar *buffer, size_t limit, ldouble number, int type, size_t precise )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_FLT2STR( ldouble, modfl, powl, log10l, ms_itow, LDBL, LDBLT );
#else
    __MST_FLT2STR( ldouble, modfl, powl, log10l, ms_itoa, LDBL, LDBLT );
#endif
}


/* ============================================================================== ms_stoui / 3 ======================================== #s
    uint   num = 0;
    size_t dig = (uchar)*str;

    // przetwórz odpowiedni typ
    switch( type )
    {
    // system dwójkowy (binarny)
    case MSTC_BINARY:
        while( dig == '0' || dig == '1' )
            num <<= 0x1, num |= dig - '0', dig = *(++str);
    break;
    // system czwórkowy
    case MSTC_QUATERNARY:
        while( dig >= '0' && dig <= '3' )
            num <<= 0x2, num |= dig - '0', dig = *(++str);
    break;
    // system ósemkowy (oktalny)
    case MSTC_OCTAL:
        while( dig >= '0' && dig <= '7' )
            num <<= 0x3, num |= dig - '0', dig = *(++str);
    break;
    // system dziesiętny
    case MSTC_DECIMAL:
        while( dig >= '0' && dig <= '9' )
            num = num * 10 + (dig - '0'), dig = *(++str);
    break;
    // system szesnastkowy (heksadecymalny) - duże litery
    case MSTC_HEXDEC_UPCASE:
        while( dig )
        {
            if( dig < '0' || dig > '9' )
            {
                if( dig < 'A' || dig > 'F' )
                    break;
                num <<= 0x4, num |= dig - ('A' - 10), dig = *(++str);
                continue;
            }
            num <<= 0x4, num |= dig - '0', dig = *(++str);
        }
    break;
    // system szesnastkowy (heksadecymalny) - małe litery
    case MSTC_HEXDEC_LOCASE:
        while( dig )
        {
            if( dig < '0' || dig > '9' )
            {
                if( dig < 'a' || dig > 'f' )
                    break;
                num <<= 0x4, num |= dig - ('a' - 10), dig = *(++str);
                continue;
            }
            num <<= 0x4, num |= dig - '0', dig = *(++str);
        }
    break;
    }
    return num;
#e ============================================================================== ms_stoui / 3 ======================================== */

uint ms_atoui( const char *str, int type )
{
#define __MST_STR2UINT(_iType)                                                                                                          \
    _iType num = 0;                                                                                                                     \
    size_t dig = *str;                                                                                                                  \
    /* przetwórz odpowiedni typ */                                                                                                      \
    switch( type )                                                                                                                      \
    {                                                                                                                                   \
    /* system dwójkowy (binarny) */                                                                                                     \
    case MSTC_BINARY:                                                                                                                   \
        while( dig == '0' || dig == '1' )                                                                                               \
            num <<= 0x1, num |= dig - '0', dig = *(++str);                                                                              \
    break;                                                                                                                              \
    /* system czwórkowy */                                                                                                              \
    case MSTC_QUATERNARY:                                                                                                               \
        while( dig >= '0' && dig <= '7' )                                                                                               \
            num <<= 0x2, num |= dig - '0', dig = *(++str);                                                                              \
    break;                                                                                                                              \
    /* system ósemkowy (oktalny) */                                                                                                     \
    case MSTC_OCTAL:                                                                                                                    \
        while( dig >= '0' && dig <= '7' )                                                                                               \
            num <<= 0x3, num |= dig - '0', dig = *(++str);                                                                              \
    break;                                                                                                                              \
    /* system dziesiętny */                                                                                                             \
    case MSTC_DECIMAL:                                                                                                                  \
        while( dig >= '0' && dig <= '9' )                                                                                               \
            num = num * 10 + (dig - '0'), dig = *(++str);                                                                               \
    break;                                                                                                                              \
    /* system szesnastkowy (heksadecymalny) - duże litery */                                                                            \
    case MSTC_HEXDEC_UPCASE:                                                                                                            \
        while( dig )                                                                                                                    \
        {                                                                                                                               \
            if( dig < '0' || dig > '9' )                                                                                                \
            {                                                                                                                           \
                if( dig < 'A' || dig > 'F' )                                                                                            \
                    break;                                                                                                              \
                num <<= 0x4, num |= dig - ('A' - 10), dig = *(++str);                                                                   \
                continue;                                                                                                               \
            }                                                                                                                           \
            num <<= 0x4, num |= dig - '0', dig = *(++str);                                                                              \
        }                                                                                                                               \
    break;                                                                                                                              \
    /* system szesnastkowy (heksadecymalny) - małe litery */                                                                            \
    case MSTC_HEXDEC_LOCASE:                                                                                                            \
        while( dig )                                                                                                                    \
        {                                                                                                                               \
            if( dig < '0' || dig > '9' )                                                                                                \
            {                                                                                                                           \
                if( dig < 'a' || dig > 'f' )                                                                                            \
                    break;                                                                                                              \
                num <<= 0x4, num |= dig - ('a' - 10), dig = *(++str);                                                                   \
                continue;                                                                                                               \
            }                                                                                                                           \
            num <<= 0x4, num |= dig - '0', dig = *(++str);                                                                              \
        }                                                                                                                               \
    break;                                                                                                                              \
    }                                                                                                                                   \
    return num

    __MST_STR2UINT( uint );
}
// -------------
uint ms_wtoui( const wchar_t *str, int type )
{
    __MST_STR2UINT( uint );
}
// -------------
uint ms_stoui( const tchar *str, int type )
{
    __MST_STR2UINT( uint );
}


/* ============================================================================== ms_stoul / 3 ======================================== */

ulong ms_atoul( const char *str, int type )
{
    __MST_STR2UINT( ulong );
}
// -------------
ulong ms_wtoul( const wchar_t *str, int type )
{
    __MST_STR2UINT( ulong );
}
// -------------
ulong ms_stoul( const tchar *str, int type )
{
    __MST_STR2UINT( ulong );
}


/* ============================================================================== ms_stoull / 3 ======================================= */

ullong ms_atoull( const char *str, int type )
{
    __MST_STR2UINT( ullong );
}
// -------------
ullong ms_wtoull( const wchar_t *str, int type )
{
    __MST_STR2UINT( ullong );
}
// -------------
ullong ms_toull( const tchar *str, int type )
{
    __MST_STR2UINT( ullong );
}


/* ============================================================================== ms_stoi / 3 ========================================= #s
    size_t dig = *str;

    if( dig < '0' || dig > '9' )
    {
        // liczba ujemna
        if( dig == '-' )
            return -ms_atoui( ++str, MSTC_DECIMAL );
        return 0;
    }
    return ms_atoui( str, MSTC_DECIMAL );
#e ============================================================================== ms_stoi / 3 ========================================= */
int ms_atoi( const char *str )
{
#define __MST_STR2INT(_iType,_fName)                                                                                                    \
    size_t dig = *str;                                                                                                                  \
    if( dig < '0' || dig > '9' )                                                                                                        \
    {                                                                                                                                   \
        /* liczba ujemna */                                                                                                             \
        if( dig == '-' )                                                                                                                \
            return -(_iType)_fName( ++str, MSTC_DECIMAL );                                                                              \
        return 0;                                                                                                                       \
    }                                                                                                                                   \
    return (_iType)_fName( str, MSTC_DECIMAL )

    __MST_STR2INT( int, ms_atoui );
}
// -------------
int ms_wtoi( const wchar_t *str )
{
    __MST_STR2INT( int, ms_wtoui );
}
// -------------
int ms_stoi( const tchar *str )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_STR2INT( int, ms_wtoui );
#else
    __MST_STR2INT( int, ms_atoui );
#endif
}


/* ============================================================================== ms_stol / 3 ========================================= */

long ms_atol( const char *str )
{
    __MST_STR2INT( long, ms_atoul );
}
// -------------
long ms_wtol( const wchar_t *str )
{
    __MST_STR2INT( long, ms_wtoul );
}
// -------------
long ms_stol( const tchar *str )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_STR2INT( long, ms_wtoul );
#else
    __MST_STR2INT( long, ms_atoul );
#endif
}


/* ============================================================================== ms_stoll / 3 ======================================== */

llong ms_atoll( const char *str )
{
    __MST_STR2INT( llong, ms_atoull );
}
// -------------
llong ms_wtoll( const wchar_t *str )
{
    __MST_STR2INT( llong, ms_wtoull );
}
// -------------
llong ms_stoll( const tchar *str )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_STR2INT( llong, ms_wtoull );
#else
    __MST_STR2INT( llong, ms_atoull );
#endif
}


/* ============================================================================== ms_atod / 3 ========================================= #s
    double num = 0.0, mpo = 1.0;
    size_t dig = *str, min = 0;

    // minus
    if( dig == '-' )
        dig = *(++str), min = 1;

    // część całkowita
    while( dig >= '0' && dig <= '9' )
        num *= 10.0, num += dig - '0', dig = *(++str);
    if( dig != '.' )
        return num;
    dig = *(++str);
    
    // część dziesiętna
    while( dig >= '0' && dig <= '9' )
        mpo *= 10.0, num += (double)(dig - '0') / mpo, dig = *(++str);
    
    // notacja naukowa
    if( (dig == 'e' && type == MSTC_SCIENT_LOCASE) || (dig == 'E' && type == MSTC_SCIENT_UPCASE) )
    {
        int minus, shift;

        dig = *(++str);
        minus = dig == '-' ? 1 : 0;
        ++str;

        // przesunięcie liczby
        shift = ms_atoui( str, MSTC_DECIMAL );
        if( minus )
            num /= pow( 10.0, shift );
        else
            num *= pow( 10.0, shift );
    }
    return min ? -num : num;
#e ============================================================================== ms_atod / 3 ========================================= */

double ms_atod( const char *str, int type )
{
#define __MST_STR2FLT(_iType,_fPow,_fAtoui)                                                                                             \
    _iType num = 0.0, mpo = 1.0;                                                                                                        \
    size_t dig = *str, min = 0;                                                                                                         \
    /* minus */                                                                                                                         \
    if( dig == '-' )                                                                                                                    \
        dig = *(++str), min = 1;                                                                                                        \
    /* część całkowita */                                                                                                               \
    while( dig >= '0' && dig <= '9' )                                                                                                   \
        num *= 10.0, num += (_iType)(dig - '0'), dig = *(++str);                                                                        \
    if( dig != '.' )                                                                                                                    \
        return num;                                                                                                                     \
    dig = *++str;                                                                                                                       \
    /* część dziesiętna */                                                                                                              \
    while( dig >= '0' && dig <= '9' )                                                                                                   \
        mpo *= 10.0, num += (_iType)(dig - '0') / mpo, dig = *(++str);                                                                  \
    /* notacja naukowa */                                                                                                               \
    if( (dig == 'e' && type == MSTC_SCIENT_LOCASE) || (dig == 'E' && type == MSTC_SCIENT_UPCASE) )                                      \
    {                                                                                                                                   \
        int minus, shift;                                                                                                               \
        dig   = *(++str);                                                                                                               \
        minus = dig == '-' ? 1 : 0;                                                                                                     \
        ++str;                                                                                                                          \
        /* przesunięcie liczby */                                                                                                       \
        shift = _fAtoui( str, MSTC_DECIMAL );                                                                                           \
        if( minus )                                                                                                                     \
            num /= _fPow( 10.0, shift );                                                                                                \
        else                                                                                                                            \
            num *= _fPow( 10.0, shift );                                                                                                \
    }                                                                                                                                   \
    return min ? -num : num;

    __MST_STR2FLT( double, pow, ms_atoui );
}
// -------------
double ms_wtod( const wchar_t *str, int type )
{
    __MST_STR2FLT( double, pow, ms_wtoui );
}
// -------------
double ms_stod( const tchar *str, int type )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_STR2FLT( double, pow, ms_wtoui );
#else
    __MST_STR2FLT( double, pow, ms_atoui );
#endif
}


/* ============================================================================== ms_atold / 3 ======================================== */

ldouble ms_atold( const char *str, int type )
{
    __MST_STR2FLT( ldouble, pow, ms_atoui );
}
// -------------
ldouble ms_wtold( const wchar_t *str, int type )
{
    __MST_STR2FLT( ldouble, pow, ms_wtoui );
}
// -------------
ldouble ms_stold( const tchar *str, int type )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_STR2FLT( ldouble, pow, ms_wtoui );
#else
    __MST_STR2FLT( ldouble, pow, ms_atoui );
#endif
}


/* ============================================================================== ms_sprintf / 3 ====================================== #s
    va_list args;
    size_t  chars;

    va_start( args, type );
    chars = ms_vsprintf( buffer, limit, type, args );
    va_end( args );

    return chars;
#e ============================================================================== ms_sprintf / 3 ====================================== */

size_t ms_sprintf( tchar *buffer, size_t limit, const tchar *format, ... )
{
#define __MST_VSPRINTF_JUMP(_fName)                                                                                                     \
    va_list args;                                                                                                                       \
    size_t  chars;                                                                                                                      \
    va_start( args, format );                                                                                                           \
    chars = _fName( buffer, limit, format, args );                                                                                      \
    va_end( args );                                                                                                                     \
    return chars

#ifdef MSD_UNICODE_SUPPORT
    __MST_VSPRINTF_JUMP( ms_vswprintf );
#else
    __MST_VSPRINTF_JUMP( ms_vsaprintf );
#endif
}
// -------------
size_t ms_saprintf( char *buffer, size_t limit, const char *format, ... )
{
    __MST_VSPRINTF_JUMP( ms_vsaprintf );
}
// -------------
size_t ms_swprintf( wchar_t *buffer, size_t limit, const wchar_t *format, ... )
{
    __MST_VSPRINTF_JUMP( ms_vswprintf );
}


/* ============================================================================== ms_vsprintf / 3 ===================================== */

size_t ms_vsprintf( tchar *buffer, size_t limit, const tchar *format, va_list args )
{
#ifdef MSD_UNICODE_SUPPORT
    return ms_vswprintf( buffer, limit, format, args );
#else
    return ms_vsaprintf( buffer, limit, format, args );
#endif
}
// -------------
size_t ms_vsaprintf( char *buffer, size_t limit, const char *format, va_list args )
{
    char  *text;
    size_t size = --limit, textlen, width, precise, fillchar = 0;

    // struktura flag specyfikatora
    struct MSS_PRINTF_FLAGS flags;

#ifdef MSD_PROGRAM_DEBUG
    if( !buffer || !format )
        return 0;
#endif

    while( *format && size )
    {
        // znak typeowania
        if( *format != '%' )
        {
            *buffer++ = *format++, --size;
            continue;
        }
        // resetuj zmienne i flagi
        precise = 0, width = 0, textlen = 0, *(size_t*)&flags = 0;

_MSG_VSP_FLAG_LOOP:
        // flagi specyfikatora
        ++format;
        switch( *format )
        {
        case '0': flags.zerofill   = 1; goto _MSG_VSP_FLAG_LOOP;
        case '=': flags.center     = 1; goto _MSG_VSP_FLAG_LOOP;
        case '_': flags.text_space = 1; goto _MSG_VSP_FLAG_LOOP;
        case '+': flags.force_char = 2; goto _MSG_VSP_FLAG_LOOP;
        case ' ': flags.force_char = 3; goto _MSG_VSP_FLAG_LOOP;
        case '#': flags.alternate  = 1; goto _MSG_VSP_FLAG_LOOP;
        case '-': flags.to_left    = 1; goto _MSG_VSP_FLAG_LOOP;
        case '?': fillchar = *++format; goto _MSG_VSP_FLAG_LOOP;
        case '%': *buffer++ = '%', --size; continue; break;
        }

        // szerokość pola
        if( *format == '*' )
            width = (size_t)va_arg( args, int ), ++format;
        else
            while( *format >= '0' && *format <= '9' )
                width = width * 10 + *format++ - '0';

        // precyzja (lub nożyce)
        if( *format == '.' )
        {
            ++format;
            if( *format == '*' )
                precise = (size_t)va_arg( args, int ), ++format;
            else
                while( *format >= '0' && *format <= '9' )
                    precise = precise * 10 + *format++ - '0';
        }
        // modyfikatory długości
        if( *format == 'l' || *format == 'L' || *format == 'z' )
            textlen = *format++;
        else
            textlen = 0;

        // specyfikatory
        switch( *format )
        {
        case 'n':
            // n jak nic... lub ilość zapisanych znaków
            textlen = 0;

            // automatyczna ilość znaków
            if( flags.alternate && width > limit - size )
                width -= size - limit;

            // wyświetl znaki
            if( width )
                goto _MSG_VSP_PRINT_TEXT;

            // zapisz aktualną pozycje
            {
                size_t *ppos = va_arg( args, size_t* );
                if( !ppos )
                    continue;
                *ppos = size - limit;
            }
        break;
        case 'C':
            // unicode lub ascii w zależności od ustawień
#ifdef MSD_UNICODE_SUPPORT
            textlen = 'l';
#else
            textlen = 0;
#endif
        case 'c':
            // znak unicode (wchar_t)
            if( textlen == 'l' )
            {
                // maksymalna długość znaku (w bajtach) dla aktywnego kodowania
                size_t  length = (size_t)MB_CUR_MAX;
                wchar_t wchar  = va_arg( args, wint_t );

                if( width && size >= length )
                {
                    // automatyczna ilość znaków
                    if( flags.alternate && width > limit - size )
                        width -= size - limit;

                    text    = buffer + (size-length);
                    textlen = wctomb( text, wchar );
                    width   = width > textlen ? width - textlen : 0;
                    goto _MSG_VSP_PRINT_TEXT;
                }
                // kopiowanie bezpośrednio do bufora
                else if( size >= length )
                {
                    length  = wctomb( buffer, wchar );
                    buffer += length;
                    size   -= length;
                    break;
                }
            }
            else if( width )
            {
                // automatyczna ilość znaków
                if( flags.alternate && width > limit - size )
                    width -= size - limit;

                textlen = 1;
                text    = buffer + size;
                *text   = va_arg( args, int );

                if( width )
                    --width;
                goto _MSG_VSP_PRINT_TEXT;
            }
            *buffer++ = va_arg( args, int ), --size;
        break;
        case 'S':
            // unicode lub ascii w zależności od ustawień
#ifdef MSD_UNICODE_SUPPORT
            textlen = 'l';
#else
            textlen = 0;
#endif
        case 's':
            // tekst unicode (wchar_t)
            if( textlen == 'l' )
            {
                wchar_t *wtext = va_arg( args, wchar_t* );

                if( !wtext )
                {
                    textlen = 6, text = "(null)";
                    if( width > 6 )
                        width -= 6;
                    goto _MSG_VSP_PRINT_TEXT;
                }
                else if( width || precise )
                {
                    // automatyczna ilość znaków
                    if( flags.alternate && width > limit - size )
                        width -= size - limit;

                    // długość i szerokość tekstu
                    textlen = wcslen( wtext );
                    text    = NULL;
                    width   = width > textlen ? width - textlen : 0;

                    // konwersja znaków bez lewego wcięcia
                    if( flags.to_left || !width )
                    {
                        if( !precise || precise >= textlen )
                            textlen = wcstombs( buffer, wtext, size );
                        else
                            textlen = ms_wcsntombs( buffer, wtext, size, precise );
                    }
                    // konwersja znaków z lewym wcięciem
                    else
                    {
                        if( !precise || precise >= textlen )
                            textlen = wcstombs( buffer + width, wtext, size - width );
                        else
                            textlen = ms_wcsntombs( buffer + width, wtext, size - width, precise );
                    }
                    goto _MSG_VSP_PRINT_TEXT;
                }
                else if( size )
                {
                    if( flags.text_space )
                        *buffer++ = ' ', --size;

                    // zmiana kodowania
                    textlen = wcstombs( buffer, wtext, size );
                    size   -= textlen;
                    buffer += textlen;
                    break;
                }
            }
            // zwykły tekst
            text = va_arg( args, char* );

            if( !text )
            {
                textlen = 6, text = "(null)";
                if( width > 6 )
                    width -= 6;
            }
            else if( width )
            {
                // automatyczna ilość znaków
                if( flags.alternate && width > limit - size )
                    width -= size - limit;

                // nożyce
                textlen = strlen( text );
                if( precise && precise < textlen )
                    textlen = precise < size ? precise : size;
                else if( textlen > size )
                    textlen = size;

                width = textlen < width ? width - textlen : 0;
            }
            else if( size )
            {
                // dodatkowa spacja
                if( flags.text_space )
                    *buffer++ = ' ', --size;
                
                // nożyce
                textlen = strlen( text );
                if( textlen > size )
                    textlen = size;

                // precyzja musi być mniejsza lub równa długości tekstu
                if( !precise || precise > textlen )
                    precise = textlen;
                
                strncpy( buffer, text, precise );
                size   -= precise;
                buffer += precise;
                break;
            }
_MSG_VSP_PRINT_TEXT:
            // wykryj możliwą długość zapisu
            if( size < width + textlen )
            {
                if( size < textlen )
                    textlen = size, width = 0;
                else
                    width = size - textlen;
            }
            size -= width + textlen;

            // przyleganie do prawej i środkowanie
            if( width && (flags.center || !flags.to_left) )
            {
                if( !fillchar )
                    fillchar = ' ';

                // środkowanie tekstu
                if( flags.center )
                {
                    precise = width >> 0x1;
                    width  -= precise;

                    if( precise )
                    {
                        while( precise )
                            *buffer++ = fillchar, --precise;
                        if( flags.text_space )
                            *(buffer-1) = ' ';
                    }
                    flags.to_left = 1;
                }
                // przyleganie do prawej
                else
                {
                    while( width )
                        *buffer++ = fillchar, --width;

                    if( flags.text_space )
                        *(buffer-1) = ' ';
                }
            }

            // kopiowanie tekstu
            if( text )
                strncpy( buffer, text, textlen );
            buffer += textlen;

            // dopełnianie z prawej
            if( width && flags.to_left )
            {
                if( !fillchar )
                    fillchar = ' ';

                if( flags.text_space )
                    *buffer++ = ' ', --width;

                while( width )
                    *buffer++ = fillchar, --width;
            }
            fillchar = 0;
        break;
        case 'd': case 'i': // liczby całkowite
            switch( textlen )
            {
            case 'l': textlen = ms_ltoa ( buffer, size, va_arg(args, long ) ); break;
            case 'L': textlen = ms_lltoa( buffer, size, va_arg(args, llong) ); break;
            case 'z':
                // odpowiedni typ konwersji
                textlen = sizeof(ptrdiff_t) <= sizeof(int ) ? ms_itoa( buffer, size, (int )va_arg(args, ptrdiff_t) )
                        : sizeof(ptrdiff_t) <= sizeof(long) ? ms_ltoa( buffer, size, (long)va_arg(args, ptrdiff_t) )
                        : ms_lltoa( buffer, size, (llong)va_arg(args, ptrdiff_t) );
            break;
            default: textlen = ms_itoa( buffer, size, va_arg(args, int) );
            }
            // liczba ujemna
            if( buffer[textlen] == '-' )
                flags.force_char = 1, ++textlen;
            flags.is_signed = 1;

            goto _MSG_VSP_PARSE_NUMBER;

        // liczby naturalne
        case 'b': flags.type = MSTC_BINARY;        goto _MSG_VSP_GET_UINT;
        case 'q': flags.type = MSTC_QUATERNARY;    goto _MSG_VSP_GET_UINT;
        case 'o': flags.type = MSTC_OCTAL;         goto _MSG_VSP_GET_UINT;
        case 'P': flags.is_pointer = 1;
        case 'X': flags.type = MSTC_HEXDEC_UPCASE; goto _MSG_VSP_GET_UINT;
        case 'p': flags.is_pointer = 1;
        case 'x': flags.type = MSTC_HEXDEC_LOCASE; goto _MSG_VSP_GET_UINT;
        case 'u': flags.type = MSTC_DECIMAL;
_MSG_VSP_GET_UINT:
            switch( textlen )
            {
            case 'l': textlen = ms_ultoa ( buffer, size, va_arg(args, ulong ), flags.type ); break;
            case 'L': textlen = ms_ulltoa( buffer, size, va_arg(args, ullong), flags.type ); break;
            case 'z':
                // odpowiedni typ konwersji
                textlen = sizeof(size_t) <= sizeof(int ) ? ms_uitoa( buffer, size, (int )va_arg(args, size_t), flags.type )
                        : sizeof(size_t) <= sizeof(long) ? ms_ultoa( buffer, size, (long)va_arg(args, size_t), flags.type )
                        : ms_ulltoa( buffer, size, (llong)va_arg(args, size_t), flags.type );
            break;
            default: textlen = ms_uitoa( buffer, size, va_arg(args, int), flags.type );
            }
_MSG_VSP_PARSE_NUMBER:
            // korekcja długości i początku tekstu
            text    = buffer + textlen;
            textlen = size - textlen - 1;

            // wskaźnik
            if( flags.is_pointer )
            {
                precise = sizeof(void*) * 2;
                if( precise > textlen )
                    while( precise > textlen && precise < size )
                        *--text = '0', ++textlen;
                goto _MSG_VSP_PRINT_TEXT;
            }
            // dodatkowy znak dla liczby całkowitej (+, - lub spacja)
            if( flags.is_signed )
            {
                if( flags.force_char == 1 )
                    precise = '-';
                else if( flags.force_char == 2 )
                    precise = '+';
                else if( flags.force_char == 3 )
                    precise = ' ';
            }
            // wypełnianie zerami
            if( flags.zerofill && !flags.to_left && size > textlen )
            {
                if( precise )
                    *buffer++ = precise, --width;
                width = textlen < width ? width - textlen : 0;

                // zera wiodące
                while( size > textlen && width )
                    *buffer++ = '0', --size, --width;

                goto _MSG_VSP_PRINT_TEXT;
            }
            // dodatkowy znak
            else if( precise )
                *--text = precise, ++textlen;

            // korekcja szerokości tekstu
            width = textlen < width ? width - textlen : 0;
            goto _MSG_VSP_PRINT_TEXT;
        break;

        // liczby zmiennoprzecinkowe
        case 'E': flags.type = MSTC_SCIENT_UPCASE; goto _MSG_VSP_PARSE_FLOAT;
        case 'e': flags.type = MSTC_SCIENT_LOCASE; goto _MSG_VSP_PARSE_FLOAT;
        case 'f': flags.type = flags.alternate ? MSTC_FLOAT_NO_ZERO : MSTC_FLOAT;
_MSG_VSP_PARSE_FLOAT:
            if( !precise )
                precise = 6;

            // double lub long double - float automatycznie zamienia się na double
            if( textlen == 'l' )
                textlen = ms_ldtoa( buffer, size, va_arg(args, long double), flags.type, precise );
            else
                textlen = ms_dtoa( buffer, size, va_arg(args, double), flags.type, precise );

            // liczba ujemna
            if( buffer[textlen] == '-' )
                flags.force_char = 1, textlen++;
            flags.is_signed = 1;

            goto _MSG_VSP_PARSE_NUMBER;
        break;
        }
        ++format;
    }
    *buffer = 0;
    return limit - size;
}
// -------------
size_t ms_vswprintf( wchar_t *buffer, size_t limit, const wchar_t *format, va_list args )
{
    wchar_t *text;
    size_t   size = --limit, textlen, width, precise, fillchar = 0;

    // struktura flag specyfikatora
    struct MSS_PRINTF_FLAGS flags;

#ifdef MSD_PROGRAM_DEBUG
    if( !buffer || !format )
        return 0;
#endif

    while( *format && size )
    {
        // znak typeowania
        if( *format != '%' )
        {
            *buffer++ = *format++, --size;
            continue;
        }
        // resetuj zmienne i flagi
        precise = 0, width = 0, textlen = 0, *(size_t*)&flags = 0;

_MSG_VSWP_FLAG_LOOP:
        // flagi specyfikatora
        ++format;
        switch( *format )
        {
        case '0': flags.zerofill   = 1; goto _MSG_VSWP_FLAG_LOOP;
        case '_': flags.text_space = 1; goto _MSG_VSWP_FLAG_LOOP;
        case '+': flags.force_char = 2; goto _MSG_VSWP_FLAG_LOOP;
        case ' ': flags.force_char = 3; goto _MSG_VSWP_FLAG_LOOP;
        case '#': flags.alternate  = 1; goto _MSG_VSWP_FLAG_LOOP;
        case '-': flags.to_left    = 1; goto _MSG_VSWP_FLAG_LOOP;
        case '?': fillchar = *++format; goto _MSG_VSWP_FLAG_LOOP;
        case '%': *buffer++ = '%', --size; continue; break;
        }

        // szerokość pola
        if( *format == '*' )
            width = (size_t)va_arg( args, int ), ++format;
        else
            while( *format >= '0' && *format <= '9' )
                width = width * 10 + *format++ - '0';

        // precyzja (lub nożyce)
        if( *format == '.' )
        {
            ++format;
            if( *format == '*' )
                precise = (size_t)va_arg( args, int ), ++format;
            else
                while( *format >= '0' && *format <= '9' )
                    precise = precise * 10 + *format++ - '0';
        }
        // modyfikatory długości
        if( *format == 'l' || *format == 'L' || *format == 'z' )
            textlen = *format++;
        else
            textlen = 0;

        // specyfikatory
        switch( *format )
        {
        case 'n':
            // n jak nic... lub ilość zapisanych znaków
            textlen = 0;

            // automatyczna ilość znaków
            if( flags.alternate && width > limit - size )
                width -= size - limit;

            // wyświetl znaki
            if( width )
                goto _MSG_VSWP_PRINT_TEXT;

            // zapisz aktualną pozycje
            {
                size_t *ppos = va_arg( args, size_t* );
                if( !ppos )
                    continue;
                *ppos = size - limit;
            }
        break;
        case 'C':
            // unicode lub ascii w zależności od ustawień
#ifdef MSD_UNICODE_SUPPORT
            textlen = 'l';
#else
            textlen = 0;
#endif
        case 'c':
            // szerokość
            if( width )
            {
                // automatyczna ilość znaków
                if( flags.alternate && width > limit - size )
                    width -= size - limit;

                textlen = 1, --width;
                text    = buffer + size;
                *text   = textlen == 'l' ? va_arg(args, wint_t) : va_arg(args, int);
                goto _MSG_VSWP_PRINT_TEXT;
            }
            // sam znak
            *buffer++ = textlen == 'l' ? va_arg(args, wint_t) : va_arg(args, int);
        break;
        case 'S':
            // unicode lub ascii w zależności od ustawień
#ifdef MSD_UNICODE_SUPPORT
            textlen = 'l';
#else
            textlen = 0;
#endif
        case 's':
            if( textlen != 'l' )
            {
                char *ctext = va_arg( args, char* );

                if( !ctext )
                {
                    textlen = 6, text = L"(null)";
                    if( width > 6 )
                        width -= 6;
                    goto _MSG_VSWP_PRINT_TEXT;
                }
                else if( width || precise )
                {
                    // automatyczna ilość znaków
                    if( flags.alternate && width > limit - size )
                        width -= size - limit;

                    // długość i szerokość tekstu
                    textlen = strlen( ctext );
                    text    = NULL;
                    width   = width > textlen ? width - textlen : 0;

                    // konwersja znaków bez lewego wcięcia
                    if( flags.to_left || !width )
                    {
                        if( !precise || precise >= textlen )
                            textlen = mbstowcs( buffer, ctext, size );
                        else
                            textlen = ms_mbsntowcs( buffer, ctext, size, precise );
                    }
                    // konwersja znaków z lewym wcięciem
                    else
                    {
                        if( !precise || precise >= textlen )
                            textlen = mbstowcs( buffer + width, ctext, size - width );
                        else
                            textlen = ms_mbsntowcs( buffer + width, ctext, size - width, precise );
                    }
                    goto _MSG_VSWP_PRINT_TEXT;
                }
                else if( size )
                {
                    if( flags.text_space )
                        *buffer++ = ' ', --size;

                    // zmiana kodowania
                    textlen = mbstowcs( buffer, ctext, size );
                    size   -= textlen;
                    buffer += textlen;
                    break;
                }
            }
            text = va_arg( args, wchar_t* );

            if( !text )
            {
                textlen = 6, text = L"(null)";
                if( width > 6 )
                    width -= 6;
            }
            else if( width )
            {
                // automatyczna ilość znaków
                if( flags.alternate && width > limit - size )
                    width -= size - limit;

                // nożyce
                textlen = wcslen( text );
                if( precise && precise < textlen )
                    textlen = precise < size ? precise : size;
                else if( textlen > size )
                    textlen = size;

                width = textlen < width ? width - textlen : 0;
            }
            else if( size )
            {
                // dodatkowa spacja
                if( flags.text_space )
                    *buffer++ = ' ', --size;
                
                // nożyce
                textlen = wcslen( text );
                if( textlen > size )
                    textlen = size;

                // precyzja musi być mniejsza lub równa długości tekstu
                if( !precise || precise > textlen )
                    precise = textlen;
                
                wcsncpy( buffer, text, precise );
                size   -= precise;
                buffer += precise;
                break;
            }

_MSG_VSWP_PRINT_TEXT:
            // wykryj możliwą długość zapisu
            if( size < width + textlen )
            {
                if( size < textlen )
                    textlen = size, width = 0;
                else
                    width = size - textlen;
            }
            size -= width + textlen;

            // dopełnianie z lewej
            if( width && !flags.to_left )
            {
                if( !fillchar )
                    fillchar = ' ';

                while( width )
                    *buffer++ = fillchar, width--;

                if( flags.text_space )
                    *(buffer-1) = ' ';
                fillchar = 0;
            }

            // kopiowanie tekstu
            if( text )
                wcsncpy( buffer, text, textlen );
            buffer += textlen;

            // dopełnianie z prawej
            if( width && flags.to_left )
            {
                if( !fillchar )
                    fillchar = ' ';

                if( flags.text_space )
                    *buffer++ = ' ', --width;

                while( width )
                    *buffer++ = fillchar, --width;
                fillchar = 0;
            }
        break;
        case 'd': case 'i': // liczby całkowite
            switch( textlen )
            {
            case 'l': textlen = ms_ltow ( buffer, size, va_arg(args, long ) ); break;
            case 'L': textlen = ms_lltow( buffer, size, va_arg(args, llong) ); break;
            case 'z':
                // odpowiedni typ konwersji
                textlen = sizeof(ptrdiff_t) <= sizeof(int ) ? ms_itow( buffer, size, (int )va_arg(args, ptrdiff_t) )
                        : sizeof(ptrdiff_t) <= sizeof(long) ? ms_ltow( buffer, size, (long)va_arg(args, ptrdiff_t) )
                        : ms_lltow( buffer, size, (llong)va_arg(args, ptrdiff_t) );
            break;
            default: textlen = ms_itow( buffer, size, va_arg(args, int) );
            }
            // liczba ujemna
            if( buffer[textlen] == '-' )
                flags.force_char = 1, ++textlen;
            flags.is_signed = 1;

            goto _MSG_VSWP_PARSE_NUMBER;

        // liczby naturalne
        case 'b': flags.type = MSTC_BINARY;        goto _MSG_VSWP_GET_UINT;
        case 'q': flags.type = MSTC_QUATERNARY;    goto _MSG_VSWP_GET_UINT;
        case 'o': flags.type = MSTC_OCTAL;         goto _MSG_VSWP_GET_UINT;
        case 'P': flags.is_pointer = 1;
        case 'X': flags.type = MSTC_HEXDEC_UPCASE; goto _MSG_VSWP_GET_UINT;
        case 'p': flags.is_pointer = 1;
        case 'x': flags.type = MSTC_HEXDEC_LOCASE; goto _MSG_VSWP_GET_UINT;
        case 'u': flags.type = MSTC_DECIMAL;

_MSG_VSWP_GET_UINT:
            switch( textlen )
            {
            case 'l': textlen = ms_ultow ( buffer, size, va_arg(args, ulong ), flags.type ); break;
            case 'L': textlen = ms_ulltow( buffer, size, va_arg(args, ullong), flags.type ); break;
            case 'z':
                // odpowiedni typ konwersji
                textlen = sizeof(ptrdiff_t) <= sizeof(int ) ? ms_uitow( buffer, size, (int )va_arg(args, ptrdiff_t), flags.type )
                        : sizeof(ptrdiff_t) <= sizeof(long) ? ms_ultow( buffer, size, (long)va_arg(args, ptrdiff_t), flags.type )
                        : ms_ulltow( buffer, size, (llong)va_arg(args, ptrdiff_t), flags.type );
            break;
            default: textlen = ms_uitow( buffer, size, va_arg(args, int), flags.type );
            }
_MSG_VSWP_PARSE_NUMBER:
            // korekcja długości i początku tekstu
            text    = buffer + textlen;
            textlen = size - textlen - 1;

            // wskaźnik
            if( flags.is_pointer )
            {
                precise = sizeof(void*) * 2;
                if( precise > textlen )
                    while( precise > textlen && precise < size )
                        *--text = '0', ++textlen;
                goto _MSG_VSWP_PRINT_TEXT;
            }
            // dodatkowy znak dla liczby całkowitej (+, - lub spacja)
            if( flags.is_signed )
            {
                if( flags.force_char == 1 )
                    precise = '-';
                else if( flags.force_char == 2 )
                    precise = '+';
                else if( flags.force_char == 3 )
                    precise = ' ';
            }
            // wypełnianie zerami
            if( flags.zerofill && !flags.to_left && size > textlen )
            {
                if( precise )
                    *buffer++ = precise, --width;
                width = textlen < width ? width - textlen : 0;

                // zera wiodące
                while( size > textlen && width )
                    *buffer++ = '0', --size, --width;

                goto _MSG_VSWP_PRINT_TEXT;
            }
            // dodatkowy znak
            else if( precise )
                *--text = precise, ++textlen;

            // korekcja szerokości tekstu
            width = textlen < width ? width - textlen : 0;
            goto _MSG_VSWP_PRINT_TEXT;
        break;

        // liczby zmiennoprzecinkowe
        case 'E': flags.type = MSTC_SCIENT_UPCASE; goto _MSG_VSWP_PARSE_FLOAT;
        case 'e': flags.type = MSTC_SCIENT_LOCASE; goto _MSG_VSWP_PARSE_FLOAT;
        case 'f': flags.type = flags.alternate ? MSTC_FLOAT_NO_ZERO : MSTC_FLOAT;

_MSG_VSWP_PARSE_FLOAT:
            if( !precise )
                precise = 6;

            // double lub long double - float automatycznie zamienia się na double
            if( textlen == 'l' )
                textlen = ms_ldtow( buffer, size, va_arg(args, long double), flags.type, precise );
            else
                textlen = ms_dtow( buffer, size, va_arg(args, double), flags.type, precise );

            // liczba ujemna
            if( buffer[textlen] == '-' )
                flags.force_char = 1, ++textlen;
            flags.is_signed = 1;

            goto _MSG_VSWP_PARSE_NUMBER;
        break;
        }
        ++format;
    }
    *buffer = 0;
    return limit - size;
}


/* ============================================================================== ms_printf / 3 ======================================= #s
    va_list args;
    char    helper[MSD_PRINTF_BUFFER];
    size_t  chars, error;

    va_start( args, format );
    chars = ms_vsprintf( helper, MSD_PRINTF_BUFFER, format, args );
    error = fputs( helper, stdout );
    va_end( args );

    return error == EOF ? EOF : chars;
#e ============================================================================== ms_printf / 3 ======================================= */

size_t ms_printf( const tchar *format, ... )
{
#define __MST_VSPRINTF_JUMP2(_iType,_fName,_fPuts,_defEof,_outBuff)                                                                     \
    va_list args;                                                                                                                       \
    _iType  helper[MSD_PRINTF_BUFFER];                                                                                                  \
    size_t  chars, error;                                                                                                               \
    va_start( args, format );                                                                                                           \
    chars = _fName( helper, MSD_PRINTF_BUFFER, format, args );                                                                          \
    error = _fPuts( helper, _outBuff );                                                                                                 \
    va_end( args );                                                                                                                     \
    return error == _defEof ? _defEof : chars

#ifdef MSD_UNICODE_SUPPORT
    __MST_VSPRINTF_JUMP2( wchar_t, ms_vswprintf, fputws, WEOF, stdout );
#else
    __MST_VSPRINTF_JUMP2( char, ms_vsaprintf, fputs, EOF, stdout );
#endif
}
// -------------
size_t ms_aprintf( const char *format, ... )
{
    __MST_VSPRINTF_JUMP2( char, ms_vsaprintf, fputs, EOF, stdout );
}
// -------------
size_t ms_wprintf( const wchar_t *format, ... )
{
    __MST_VSPRINTF_JUMP2( wchar_t, ms_vswprintf, fputws, WEOF, stdout );
}


/* ============================================================================== ms_vprintf / 3 ====================================== #s
    char  helper[MSD_PRINTF_BUFFER];
    size_t chars, error;
    
    chars = ms_vsprintf( helper, MSD_PRINTF_BUFFER, format, args );
    error = fputs( helper, stdout );

    return error == EOF ? EOF : chars;
#e ============================================================================== ms_vprintf / 3 ====================================== */

size_t ms_vprintf( const tchar *format, va_list args )
{
#define __MST_VSPRINTF_JUMP3(_iType,_fName,_fPuts,_defEof,_outBuff)                                                                     \
    _iType helper[MSD_PRINTF_BUFFER];                                                                                                   \
    size_t chars, error;                                                                                                                \
    chars = _fName( helper, MSD_PRINTF_BUFFER, format, args );                                                                          \
    error = _fPuts( helper, _outBuff );                                                                                                 \
    return error == _defEof ? _defEof : chars

#ifdef MSD_UNICODE_SUPPORT
    __MST_VSPRINTF_JUMP3( wchar_t, ms_vswprintf, fputws, WEOF, stdout );
#else
    __MST_VSPRINTF_JUMP3( char, ms_vsaprintf, fputs, EOF, stdout );
#endif
}
// -------------
size_t ms_vaprintf( const char *format, va_list args )
{
    __MST_VSPRINTF_JUMP3( char, ms_vsaprintf, fputs, EOF, stdout );
}
// -------------
size_t ms_vwprintf( const wchar_t *format, va_list args )
{
    __MST_VSPRINTF_JUMP3( wchar_t, ms_vswprintf, fputws, WEOF, stdout );
}


/* ============================================================================== ms_fprintf / 3 ====================================== */

size_t ms_fprintf( FILE *buffer, const tchar *format, ... )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_VSPRINTF_JUMP2( wchar_t, ms_vswprintf, fputws, WEOF, buffer );
#else
    __MST_VSPRINTF_JUMP2( char, ms_vsaprintf, fputs, EOF, buffer );
#endif
}
// -------------
size_t ms_faprintf( FILE *buffer, const char *format, ... )
{
    __MST_VSPRINTF_JUMP2( char, ms_vsaprintf, fputs, EOF, buffer );
}
// -------------
size_t ms_fwprintf( FILE *buffer, const wchar_t *format, ... )
{
    __MST_VSPRINTF_JUMP2( wchar_t, ms_vswprintf, fputws, WEOF, buffer );
}


/* ============================================================================== ms_vfprintf / 3 ===================================== */

size_t ms_vfprintf( FILE *buffer, const tchar *format, va_list args )
{
#ifdef MSD_UNICODE_SUPPORT
    __MST_VSPRINTF_JUMP3( wchar_t, ms_vswprintf, fputws, WEOF, buffer );
#else
    __MST_VSPRINTF_JUMP3( char, ms_vsaprintf, fputs, EOF, buffer );
#endif
}
// -------------
size_t ms_vfaprintf( FILE *buffer, const char *format, va_list args )
{
    __MST_VSPRINTF_JUMP3( char, ms_vsaprintf, fputs, EOF, buffer );
}
// -------------
size_t ms_vfwprintf( FILE *buffer, const wchar_t *format, va_list args )
{
    __MST_VSPRINTF_JUMP3( wchar_t, ms_vswprintf, fputws, WEOF, buffer );
}
