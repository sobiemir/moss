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
 *  Header file for "Buffer" module.
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

#ifndef _MSH_BUFFER_
#define _MSH_BUFFER_

// definicje i deklaracje głównych typów
#include "configuration.h"

// biblioteki systemowe
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <float.h>
#include <math.h>

/**
 * Systemy i formaty liczowe.
 * Używane w funkcjach z rodziny atoi, itoa, atod oraz dtoa.
**/
enum MSE_TYPE_CONVERT
{
// ------------------------------------- INT
	MSTC_DECIMAL,                                   // system dziesiętny
	MSTC_BINARY,                                    // system binarny
	MSTC_QUATERNARY,                                // system czwórkowy
	MSTC_OCTAL,                                     // system ósemkowy
	MSTC_HEXDEC_UPCASE,                             // system szesnastkowy, duże litery
	MSTC_HEXDEC_LOCASE,                             // system szesnastkowy, małe litery
// ------------------------------------- FLOAT
	MSTC_FLOAT,                                     // liczba zmiennoprzecinkowa
	MSTC_FLOAT_NO_ZERO,                             // liczba zmiennoprzecinkowa bez zer na końcu
	MSTC_SCIENT_UPCASE,                             // notacja naukowa, duże litery
	MSTC_SCIENT_LOCASE                              // notacja naukowa, małe litery
};

/**
 * Rodzina 6 funkcji do zamiany ciągu znaków typu wchar_t na char lub char na wchar_t.
 * ms_wcsntombs / ms_tcsntombs / ms_wcsntotcs / ms_mbsntowcs / ms_tcsntowcs / ms_mbsntotcs
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * 
 * param _out_ tchar       *dest   Bufor wyjściowy - docelowy.
 * param _in_  const tchar *src    Bufor wejściowy - do konwersji.
 * param _in_  size_t       length Rozmiar bufora docelowego.
 * param _in_  size_t       count  Maksymalna ilość przetwarzanych danych z bufora wejściowego.
 * 
 * return size_t Ilość zapisanch znaków do bufora docelowego.
**/
size_t ms_wcsntombs( char    *dest, const wchar_t *src, size_t length, size_t count );
size_t ms_tcsntombs( char    *dest, const tchar   *src, size_t length, size_t count );
size_t ms_wcsntotcs( tchar   *dest, const wchar_t *src, size_t length, size_t count );
size_t ms_mbsntowcs( wchar_t *dest, const char    *src, size_t length, size_t count );
size_t ms_tcsntowcs( wchar_t *dest, const tchar   *src, size_t length, size_t count );
size_t ms_mbsntotcs( tchar   *dest, const char    *src, size_t length, size_t count );

/**
 * Rodzina 9 funkcji do zamiany liczb naturalnych na ciąg znaków char, wchar_t lub tchar.
 * uint (unsigned int) / ulong (unsigned long int) / ullong (unsigned long long int)
 * ms_uitoa / ms_uitow / ms_uitos / ms_ultoa / ms_ultow / ms_ultos / ms_ulltoa / ms_ulltow / ms_ulltos
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Liczba zapisywana jest od końca, zwracany jest indeks pierwszego znaku liczby.
 * Dostępne systemy liczowe:
 * MSTC_BINARY, MSTC_QUATERNARY, MSTC_OCTAL, MSTC_DECIMAL, MSTC_HEXDEC_UPCASE, MSTC_HEXDEC_LOCASE
 *
 * Przykład użycia:
 * idx = ms_uitoa( buff, 32, 69524, MSTC_DECIMAL );
 * printf( "%s\n", buff + idx );
 * 
 * param _out_ tchar  *buffer Bufor wyjściowy - docelowy.
 * param _in_  size_t  limit  Maksymalna ilość możliwych do zapisania znaków w buforze docelowym.
 * param _in_  uint    number Zamieniana na ciąg znaków liczba naturalna.
 * param _in_  int     type   System liczbowy do którego będzie zamieniana liczba (MSE_TYPE_CONVERT)
 *
 * return size_t Indeks początkowy liczby.
**/
size_t ms_uitoa ( char    *buffer, size_t limit, uint   number, int type );
size_t ms_uitow ( wchar_t *buffer, size_t limit, uint   number, int type );
size_t ms_uitos ( tchar   *buffer, size_t limit, uint   number, int type );
size_t ms_ultoa ( char    *buffer, size_t limit, ulong  number, int type );
size_t ms_ultow ( wchar_t *buffer, size_t limit, ulong  number, int type );
size_t ms_ultos ( tchar   *buffer, size_t limit, ulong  number, int type );
size_t ms_ulltoa( char    *buffer, size_t limit, ullong number, int type );
size_t ms_ulltow( wchar_t *buffer, size_t limit, ullong number, int type );
size_t ms_ulltos( tchar   *buffer, size_t limit, ullong number, int type );

/**
 * Rodzina 9 funkcji do zamiany liczb całkowitych na ciąg znaków char, wchar_t lub tchar.
 * int (signed int) / long (signed long int) / llong (signed long long int)
 * ms_itoa / ms_itow / ms_itos / ms_ltoa / ms_ltow / ms_ltos / ms_lltoa / ms_lltow / ms_lltos
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Liczba zapisywana jest od końca, zwracany jest indeks pierwszego znaku liczby.
 * Liczba całkowita może być zapisana tylko do systemu dziesiętnego.
 *
 * Przykład użycia:
 * idx = ms_itoa( buff, 32, -4954 );
 * printf( "%s\n", buff + idx );
 * 
 * param _out_ tchar  *buffer Bufor wyjściowy - docelowy.
 * param _in_  size_t  limit  Maksymalna ilość możliwych do zapisania znaków w buforze docelowym.
 * param _in_  int     number Zamieniana na ciąg znaków liczba całkowita.
 * 
 * return size_t Indeks początkowy liczby.
**/
size_t ms_itoa ( char    *buffer, size_t limit, int   number );
size_t ms_itow ( wchar_t *buffer, size_t limit, int   number );
size_t ms_itos ( tchar   *buffer, size_t limit, int   number );
size_t ms_ltoa ( char    *buffer, size_t limit, long  number );
size_t ms_ltow ( wchar_t *buffer, size_t limit, long  number );
size_t ms_ltos ( tchar   *buffer, size_t limit, long  number );
size_t ms_lltoa( char    *buffer, size_t limit, llong number );
size_t ms_lltow( wchar_t *buffer, size_t limit, llong number );
size_t ms_lltos( tchar   *buffer, size_t limit, llong number );

/**
 * Rodzina 6 funkcji do zamiany liczb zmiennoprzecinkowych na ciąg znaków char, wchar_t lub tchar.
 * double / ldouble (long double)
 * ms_dtoa / ms_dtow / ms_dtos / ms_ldtoa / ms_ldtow / ms_ldtos
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Liczba zapisywana jest od końca, zwracany jest indeks pierwszego znaku liczby.
 * Dostępne formaty liczby zmiennoprzecinkowej:
 * MSTC_FLOAT, MSTC_FLOAT_NO_ZERO, MSTC_SCIENT_UPCASE, MSTC_SCIENT_LOCASE
 *
 * Przykład użycia:
 * idx = ms_dtoa( buff, 32, 2.560, MSTC_FLOAT, 6 );
 * printf( "%s\n", buff + idx );
 * 
 * param _out_ tchar  *buffer  Bufor wyjściowy - docelowy.
 * param _in_  size_t  limit   Maksymalna ilość możliwych do zapisania znaków w buforze docelowym.
 * param _in_  uint    number  Zamieniana na ciąg znaków liczba zmiennoprzecinkowa.
 * param _in_  int     type    Format liczby zmiennoprzecinkowej (MSE_TYPE_CONVERT).
 * param _in_  size_t  precise Precyzja liczby, czyli ilość miejsc po przecinku.
 * 
 * return size_t Indeks początkowy liczby.
**/
size_t ms_dtoa ( char    *buffer, size_t limit, double  number, int type, size_t precise );
size_t ms_dtow ( wchar_t *buffer, size_t limit, double  number, int type, size_t precise );
size_t ms_dtos ( tchar   *buffer, size_t limit, double  number, int type, size_t precise );
size_t ms_ldtoa( char    *buffer, size_t limit, ldouble number, int type, size_t precise );
size_t ms_ldtow( wchar_t *buffer, size_t limit, ldouble number, int type, size_t precise );
size_t ms_ldtos( tchar   *buffer, size_t limit, ldouble number, int type, size_t precise );

/**
 * Rodzina 9 funkcji do zamiany ciągu znaków char, wchar_t lub tchar na liczbę naturalną.
 * uint (unsigned int) / ulong (unsigned long int) / ullong (unsigned long long int)
 * ms_atoui / ms_wtoui / ms_stoui / ms_atoul / ms_wtoul / ms_stoul / ms_atoull / ms_wtoull / ms_stoull
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Dostępne systemy liczowe:
 * MSTC_BINARY, MSTC_QUATERNARY, MSTC_OCTAL, MSTC_DECIMAL, MSTC_HEXDEC_UPCASE, MSTC_HEXDEC_LOCASE
 *
 * Przykład użycia:
 * num = ms_atoui( "1011101", MSTC_BINARY );
 * printf( "%d\n", num );
 * 
 * param _in_ const tchar  *buffer Bufor wejściowy, zawierający liczbę naturalną.
 * param _in_ int           type   System liczbowy z którego wczytywana jest liczba (MSE_TYPE_CONVERT).
 * 
 * return uint Liczba naturalna pobrana z ciągu znaków.
**/
uint   ms_atoui( const char     *str, int type );
uint   ms_wtoui( const wchar_t  *str, int type );
uint   ms_stoui( const tchar    *str, int type );
ulong  ms_atoul( const char     *str, int type );
ulong  ms_wtoul( const wchar_t  *str, int type );
ulong  ms_stoul( const tchar    *str, int type );
ullong ms_atoull( const char    *str, int type );
ullong ms_wtoull( const wchar_t *str, int type );
ullong ms_stoull( const tchar   *str, int type );

uint   ms_ftoui( const FILE *file, int type );

/**
 * Rodzina 9 funkcji do zamiany ciągu znaków char, wchar_t lub tchar na liczbę całkowitą.
 * int (signed int) / long (signed long int) / llong (signed long long int)
 * ms_atoi / ms_wtoi / ms_stoi / ms_atol / ms_wtol / ms_stol / ms_atoll / ms_wtoll / ms_stoll
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Liczba całkowita może być odczytana tylko z systemu dziesiętnego.
 *
 * Przykład użycia:
 * num = ms_atoi( "-20650" );
 * printf( "%d\n", num );
 * 
 * param _in_ const tchar  *buffer Bufor wejściowy, zawierający liczbę całkowitą.
 * 
 * return int Liczba całkowita pobrana z ciągu znaków.
**/
int   ms_atoi ( const char    *str );
int   ms_wtoi ( const wchar_t *str );
int   ms_stoi ( const tchar   *str );
long  ms_atol ( const char    *str );
long  ms_wtol ( const wchar_t *str );
long  ms_stol ( const tchar   *str );
llong ms_atoll( const char    *str );
llong ms_wtoll( const wchar_t *str );
llong ms_stoll( const tchar   *str );

/**
 * Rodzina 6 funkcji do zamiany ciągu znaków char, wchar_t lub tchar na liczbę zmiennoprzecinkową.
 * double / ldouble (long double)
 * ms_dtoa / ms_dtow / ms_dtos / ms_ldtoa / ms_ldtow / ms_ldtos
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Liczba zapisywana jest od końca, zwracany jest indeks pierwszego znaku liczby.
 * Dostępne formaty liczby zmiennoprzecinkowej:
 * MSTC_FLOAT, MSTC_FLOAT_NO_ZERO, MSTC_SCIENT_UPCASE, MSTC_SCIENT_LOCASE
 *
 * Przykład użycia:
 * idx = ms_atod( "-3.250e+04", MSTC_SCIENT_LOCASE );
 * printf( "%s\n", buff + idx );
 * 
 * param _in_ const tchar *buffer  Bufor wejściowy, zawierający liczbę całkowitą.
 * param _in_ int          type    Format z którego wczytywana jest liczba zmiennoprzecinkowa (MSE_TYPE_CONVERT).
 * 
 * return double Liczba zmiennoprzecinkowa pobrana z ciągu znaków.
**/
double  ms_atod ( const char    *str, int type );
double  ms_wtod ( const wchar_t *str, int type );
double  ms_stod ( const tchar   *str, int type );
ldouble ms_atold( const char    *str, int type );
ldouble ms_wtold( const wchar_t *str, int type );
ldouble ms_stold( const tchar   *str, int type );

/**
 * Rodzina 3 funkcji do wyświetlania formatowanego ciągu znaków w konsoli.
 * ms_printf / ms_aprintf / ms_wprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * 
 * Specyfikatory i domyślne typy:
 * > d/i : int     : Liczba całkowita zapisana dziesiętnie.
 * > u   : uint    : Liczba naturalna zapisana dziesiętnie.
 * > X/x : uint    : Liczba naturalna zapisana szesnastkowo (duże lub małe litery).
 * > o   : uint    : Liczba naturalna zapisana ósemkowo.
 * > q   : uint    : Liczba naturalna zapisana czwórkowo.
 * > b   : uint    : Liczba naturalna zapisana dwójkowo (binarnie).
 * > f   : double  : Liczba zmiennoprzecinkowa.
 * > e/E : double  : Liczba zmiennoprzecinkowa - notacja naukowa.
 * > c/C : char    : Znak char (w przypadku C char lub wint_t w zależności od MSD_UNICODE_SUPPORT).
 * > s/S : char*   : Ciąg znaków char (w przypadku S char lub wchar_t w zależności od MSD_UNICODE_SUPPORT).
 * > p   : void*   : Wskaźnik do zmiennej.
 * > n   : size_t* : Formatowany tekst bez niczego lub aktualna pozycja zapisanego znaku.
 * > %   : ---     : Znak procentu.
 *
 * Flagi:
 * > -  : Przyleganie do lewej strony (wszystkie).
 * > +  : Wymuszony plus przy dodatnich liczbach całkowitych (d/i).
 * >    : Spacja dla przed dodatnią liczbą całkowitą (d/i).
 * > #  : Automatyczna szerokość lub obcinanie zer dla liczb zmiennoprzecinkowych (n/s/S/c/C/f/e/E).
 * > 0  : Dodawanie zer do liczby z prawej strony (d/i/u/o/q/b/X/x).
 * > _  : Dodatkowa spacja przed lub po tekście lub znaku (c/C/s/S).
 * > ?* : Powtarzanie znaku podanego po pytajniku (wszystkie).
 *
 * Rozmiar:
 * > 0-9 : Minimalna ilość znaków które mają być wyświetlone. Gdy tekst posiada mniej znaków niż zostało tutaj podane,
 *         domyślnie uzupełniany jest z lewej strony znakami spacji. Można to zmienić dzięki odpowiednim flagom.
 * > #   : Działa tylko specyfikatorze n/s/S/c/C - minimalna ilość znaków (powyżej) traktowana jest jako minimalna pozycja
 *         do której tekst musi być zapisany. Minimalna pozycja: ilość znaków - aktualna pozycja.
 * > *   : Szerokość (pierwsza pozycja) pobierana jest z argumentu funkcji.
 *
 * Precyzja (tylko f/e/E):
 * > .0-9 : Ilość znaków precyzji, jest to wartość stała i uzupełniana zerami od końca po przecinku w przypadku braku liczb.
 * > #    : Precyzja traktowana jest jako maksymalna, co oznacza że nie są wyświetlane zbędne zera na końcu.
 * > .*   : Precyzja (pierwsza pozycja) pobierana jest z argumentu funkcji.
 *
 * Modyfikatory:
 * > l : Zamiana int => long, uint => ulong, char => wint_t, char* => wchar_t*, double => ldouble.
 * > L : Zamiana int => llong, uint => ullong.
 * > z : Zamiana int => ptrdiff_t, uint => size_t.
 *
 * Przykłady użycia:
 * ms_printf( "%10?.s", "liczba" );         <- ....liczba
 * ms_printf( "%12?#_s", "koliber" );       <- #### koliber
 * ms_printf( "test %20?._#s", "amnezja" ); <- test ....... amnezja 
 * ms_printf( "%.8#f", 25.2302" );          <- 25.2302
 * ms_printf( "%.8f", 14.12004" );          <- 14.12004000
 * ms_printf( "%b", 150 );                  <- 10010110
 *
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ ...          ...    Dodatkowe argumenty przekazywane do tekstu.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_printf ( const tchar   *format, ... );
size_t ms_aprintf( const char    *format, ... );
size_t ms_wprintf( const wchar_t *format, ... );

/**
 * Rodzina 3 funkcji do wyświetlania formatowanego ciągu znaków w konsoli.
 * ms_vprintf / ms_vaprintf / ms_vwprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * Dokładny opis formatowania znajduje się w funkcji ms_printf.
 *
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ va_list      args   Dodatkowe argumenty przekazywane do tekstu w postaci listy.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_vprintf ( const tchar   *format, va_list args );
size_t ms_vaprintf( const char    *format, va_list args );
size_t ms_vwprintf( const wchar_t *format, va_list args );

/**
 * Rodzina 3 funkcji do zapisu formatowanego ciągu znaków do pliku.
 * ms_fprintf / ms_faprintf / ms_fwprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * Dokładny opis formatowania znajduje się w funkcji ms_printf.
 *
 * param _in_ FILE        *buffer Uchwyt pliku do którego będzie zapisywany tekst.
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ ...          ...    Dodatkowe argumenty przekazywane do tekstu.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_fprintf ( FILE *buffer, const tchar   *format, ... );
size_t ms_faprintf( FILE *buffer, const char    *format, ... );
size_t ms_fwprintf( FILE *buffer, const wchar_t *format, ... );

/**
 * Rodzina 3 funkcji do zapisu formatowanego ciągu znaków do pliku.
 * ms_vfprintf / ms_vfaprintf / ms_vfwprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * Dokładny opis formatowania znajduje się w funkcji ms_printf.
 *
 * param _in_ FILE        *buffer Uchwyt pliku do którego będzie zapisywany tekst.
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ va_list      args   Dodatkowe argumenty przekazywane do tekstu w postaci listy.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_vfprintf ( FILE *buffer, const tchar   *format, va_list args );
size_t ms_vfaprintf( FILE *buffer, const char    *format, va_list args );
size_t ms_vfwprintf( FILE *buffer, const wchar_t *format, va_list args );

/**
 * Rodzina 3 funkcji do zapisu formatowanego ciągu znaków do zmiennej tekstowej.
 * ms_sprintf / ms_saprintf / ms_swprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * Dokładny opis formatowania znajduje się w funkcji ms_printf.
 *
 * param _in_ tchar       *buffer Zmienna tekstowa do której będzie zapisywany formatowany tekst.
 * param _in_ size_t       limit  Maksymalna ilość możliwych do zapisania znaków, rozmiar zmiennej buffer.
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ ...          ...    Dodatkowe argumenty przekazywane do tekstu.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_sprintf ( tchar   *buffer, size_t limit, const tchar   *format, ... );
size_t ms_saprintf( char    *buffer, size_t limit, const char    *format, ... );
size_t ms_swprintf( wchar_t *buffer, size_t limit, const wchar_t *format, ... );

/**
 * Rodzina 3 funkcji do zapisu formatowanego ciągu znaków do zmiennej tekstowej.
 * ms_sprintf / ms_saprintf / ms_swprintf
 * 
 * Typ tchar zależy od makra MSD_UNICODE_SUPPORT - char lub wchar_t.
 * Formatowanie tekstu jest podobne do funkcji printf.
 * Dokładny opis formatowania znajduje się w funkcji ms_printf.
 *
 * param _in_ tchar       *buffer Zmienna tekstowa do której będzie zapisywany formatowany tekst.
 * param _in_ size_t       limit  Maksymalna ilość możliwych do zapisania znaków, rozmiar zmiennej buffer.
 * param _in_ const tchar *format Formatowany tekst.
 * param _in_ va_list      args   Dodatkowe argumenty przekazywane do tekstu w postaci listy.
 * 
 * return size_t Ilość wyświetlonych znaków lub błąd (size_t)EOF / (size_t)WEOF.
**/
size_t ms_vsprintf ( tchar   *buffer, size_t limit, const tchar   *format, va_list args );
size_t ms_vsaprintf( char    *buffer, size_t limit, const char    *format, va_list args );
size_t ms_vswprintf( wchar_t *buffer, size_t limit, const wchar_t *format, va_list args );

#endif
