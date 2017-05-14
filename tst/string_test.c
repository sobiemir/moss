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
 *  Test file for "String" module.
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

#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include <moss/string.h>
#include <mstest.h>

/**
 * Struktura zawierająca dane używane w funkcjach testujących.
 * Dane przekazywane są z funkcji do funkcji.
 * Dzięki temu po wystąpieniu błędu i przerwaniu funkcji, pamięć może zostać w prosty sposób zwolniona.
 * To właśnie głównie dzięki tej tablicy nie można zmieniać kolejności wywoływania testów.
 * Zapobiega wyciekom pamięci.
 */
typedef struct MSSTST_STRING_DATA
{
	MS_STRING *Pointer1;   /* wskaźnik do ciągu znaków alokowanego w pamięci */
	MS_STRING *Pointer2;   /* wskaźnik do drugiego ciągu znaków alokowanego w pamięci */
	MS_STRING *Pointer3;   /* wskaźnik do trzeciego ciągu znaków alokowanego w pamięci */
	MS_STRING  Local1;     /* zmienna dla ciągu znaków, zwracana przez return lub dla init */
	MS_STRING  Local2;     /* druga zmienna dla ciągu znaków, zwracana przez return lub dla init */
	MS_STRING  Local3;     /* trzecia zmienna dla ciągu znaków, zwracana przez return lub dla init */
	MS_ARRAY   Array;
}
MSTST_STRING_DATA;

/**
 * Pangramy.
 * Pangram to zdanie, które zawiera wszystkie litery w danym języku.
 */
const char    PANGRAM1[] =  "Pack my box with five dozen liquor jugs.";
const char    PANGRAM2[] =  "Pchnąć w tę łódź jeża lub ośm skrzyń fig.";
const wchar_t PANGRAM3[] = L"Pchnąć w tę łódź jeża lub ośm skrzyń fig.";

/* ilość bajtów przypadająca na dany znak dla polskiego pangramu */
const int PANGRAM2MB[] = {
/*  P  c  h  n  ą  ć     w     t  ę     ł  ó  d  ź     j  e  ż */
	1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 2,
/*  a     l  u  b     o  ś  m     s  k  r  z  y  ń     f  i  g  . */
	1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1
};
/* przesunięcie indeksu bajtu względem danego znaku dla polskiego pangramu */
const int PANGRAM2OF[] = {
/*  P   c   h   n   ą   ć       w       t   ę       ł   ó   d   ź       j   e   ż */
	0,  1,  2,  3,  4,  6,  8,  9,  10, 11, 12, 14, 15, 17, 19, 20, 22, 23, 24, 25,
/*  a       l   u   b       o   ś   m       s   k   r   z   y   ń       f   i   g   . */
	27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49
};

/**
 * Funkcja wywoływana przy kończeniu testu.
 * Zwalnia miejsce przydzielone w pamięci podczas działania programu.
 */
void mst_string_teardown( MST_SUITE *suite )
{
	MSTST_STRING_DATA *data;

	if( !suite->Data )
		return;

	data = suite->Data;

	ms_string_free( data->Pointer1 );
	ms_string_free( data->Pointer2 );
	ms_string_free( data->Pointer3 );

	data->Pointer1 = NULL;
	data->Pointer2 = NULL;
	data->Pointer3 = NULL;

	ms_string_free( &data->Local1 );
	ms_string_free( &data->Local2 );
	ms_string_free( &data->Local3 );

	ms_array_free( &data->Array );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DŁUGOŚĆ CIĄGU / INFORMACJE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int mst_string_length_mbs( MST_FUNCTION *info )
{
	size_t length;
	mst_prepare( info );

	/* długość całego tekstu - 41 znaków, 50 bajtów */
	length = ms_string_length_mbs( PANGRAM2, 0 );
	mst_assert_uint( length, ==, 41 );

	/* słowo: Pchnąć - 6 znaków, 8 bajtów */
	length = ms_string_length_mbs( PANGRAM2, 8 );
	mst_assert_uint( length, ==, 6 );

	/* słowo: łódź - 4 znaki, 7 bajtów */
	length = ms_string_length_mbs( &PANGRAM2[15], 7 );
	mst_assert_uint( length, ==, 4 );

	/* powinien być błąd - błędny znak po literze d w słowie łódź
	   dlatego, że ź nie zostało podane w całości, ale w połowie. */
	length = ms_string_length_mbs( &PANGRAM2[15], 6 );
	mst_assert_uint( length, ==, 0 );
	/* można sprawdzić dodatkowo ERRNO */

	/* a teraz próba podania za dużej ilości bajtów (sam wyraz ma 50)
	   o ile ciąg zakończony jest znakiem \0, to wszystko powinno być w porządku. */
	length = ms_string_length_mbs( PANGRAM2, 100 );
	mst_assert_uint( length, ==, 41 );

	return MSEC_OK;
}

int mst_string_length_wcstombs( MST_FUNCTION *info )
{
	size_t  length;
	wchar_t corrupted[5];

	mst_prepare( info );

	/* obliczanie długości wielobajtowego ciągu z wchar_t
	   obliczana jest ilość bajtów, z których będzie się składał ciąg */
	length = ms_string_length_wcstombs( PANGRAM3, 0 );
	mst_assert_uint( length, ==, sizeof PANGRAM2 - 1 );

	/* słowo: Pchnąć - 6 znaków, ale 8 bajtów */
	length = ms_string_length_wcstombs( PANGRAM3, 6 );
	mst_assert_uint( length, ==, 8 );

	/* słowo: łódź - 4 znaki, 7 bajtów */
	length = ms_string_length_wcstombs( &PANGRAM3[12], 4 );
	mst_assert_uint( length, ==, 7 );

	/* próba symulacji błędu */
	corrupted[0] = L'Ż';
	corrupted[1] = L'ó';
	corrupted[2] = WCHAR_MIN; /* symulacja błędu (być może działa tylko na linuksie) */
	corrupted[3] = L'ć';
	corrupted[4] = L'\0';

	length = ms_string_length_wcstombs( corrupted, 4 );
	mst_assert_uint( length, ==, 0 );

	/* próba podania za dużej ilości znaków
	   jeżeli ciąg zakończony jest znakiem L\0, to wszystko powinno być w porządku */
	length = ms_string_length_wcstombs( PANGRAM3, 100 );
	mst_assert_uint( length, ==, sizeof PANGRAM2 - 1 );

	return MSEC_OK;
}

int mst_string_info_mbs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	size_t             x, length;
	MS_ARRAY          *array;

	mst_prepare( info );
	data = info->Data;

	/* utwórz tablicę w której przechowywane będą dane o ciągu wielobajtowym */
	data->Array = ms_array_return_local( sizeof(MS_MBINFO), 0 );
	array = &data->Array;

	mst_assert( array->Items );

	/* pobierz tablicę z informacjami o znakach */
	ercode = ms_string_info_mbs( PANGRAM2, 0, array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź jej długość */
	length = ms_string_length_mbs( PANGRAM2, 0 );
	mst_assert_uint( length, ==, array->Length );

	/* i dane czy są poprawne */
	for( x = 0; x < array->Length; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(array, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}

	/* wyczyść tablicę, dane nie są potrzebne w tym momencie */
	ms_array_clear( array );

	/* spróbuj jeszcze stworzyć tablicę dla danych, zawierających błędny znak */
	ercode = ms_string_info_mbs( PANGRAM2, 7, array );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* dane są dodawane do momentu wystąpienia błędu */
	mst_assert_uint( array->Length, ==, 5 );

	/* wyczyść tablicę po raz kolejny */
	ms_array_clear( array );

	/* pobierz tablicę z informacjami podając zbyt dużą wielkość ciągu znaków
	   funkcja powinna sobie z tym poradzić */
	ercode = ms_string_info_mbs( PANGRAM2, 100, array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź jej długość */
	length = ms_string_length_mbs( PANGRAM2, 0 );
	mst_assert_uint( length, ==, array->Length );

	/* oraz dane, jak wcześniej */
	for( x = 0; x < array->Length; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(array, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}

	/* sprawdź jeszcze dla ciągu jednobajtowego, powinien mieć same 1 */
	ms_array_clear( array );

	ercode = ms_string_info_mbs( PANGRAM1, 0, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length, ==, sizeof PANGRAM1 - 1 );

	for( x = 0; x < array->Length; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(array, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, 1 );
		mst_assert_uint( mbinfo->Offset, ==, x );
	}

	return MSEC_OK;
}

int mst_string_info_wcstombs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	size_t             x;
	MS_ARRAY          *array;
	wchar_t            corrupted[5];

	mst_prepare( info );
	data = info->Data;

	/* tablica powinna być zainicjalizowana we wcześniejszej funkcji */
	mst_assert( data->Array.Items );
	array = &data->Array;
	ms_array_clear( array );

	/* pobierz strukturę dla wielobajtowego ciągu znaków wprost z typu wchar_t */
	ercode = ms_string_info_wcstombs( PANGRAM3, 0, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length, ==, sizeof PANGRAM3 / sizeof(wchar_t) - 1 );

	/* porównaj dane z poprawnymi */
	for( x = 0; x < array->Length; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(array, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}

	/* wyczyść tablicę, dane nie są potrzebne w tym momencie */
	ms_array_clear( array );

	/* próba symulacji błędu */
	corrupted[0] = L'Ż';
	corrupted[1] = L'ó';
	corrupted[2] = WCHAR_MIN; /* symulacja błędu (być może tylko na linuksie) */
	corrupted[3] = L'ć';
	corrupted[4] = '\0';

	/* spróbuj jeszcze stworzyć tablicę dla danych, zawierających błędny znak */
	ercode = ms_string_info_wcstombs( corrupted, 4, array );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* dane są dodawane do momentu wystąpienia błędu */
	mst_assert_uint( array->Length, ==, 2 );
	ms_array_clear( array );

	/* teraz to samo, tylko przekaż za dużą ilość znaków w ciągu
	   powinno wszystko być w porządku z powodu ograniczenia */
	ercode = ms_string_info_wcstombs( PANGRAM3, 100, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length, ==, ms_string_length_mbs(PANGRAM2, 0) );

	/* porównaj dane z poprawnymi */
	for( x = 0; x < array->Length; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(array, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	TWORZENIE I NISZCZENIE CIĄGU ZNAKÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int mst_string_create_cs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	MS_STRING         *str;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer1 = ms_string_alloc_cs( PANGRAM1, 100 );

	mst_assert(  str );
	mst_assert(  str->Data.Char );
	mst_assert( !str->Wide );
	mst_assert(  str->Destroy );
	mst_assert( !str->Hashed );
	mst_assert( !str->MBInfo );
	mst_assert(  str->FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( str->Length,   ==, sizeof PANGRAM1 - 1 );
	mst_assert_uint ( str->Capacity, ==, 100 );
	mst_assert_uint ( str->Hash,     ==, 0 );
	mst_assert_float( str->Modifier, ==, 2.f );

	mst_assert_cs( str->Data.Char, PANGRAM1 );

	/* jednobajtowy ciąg znaków o pojemności takiej samej jak długość całego ciągu */
	data->Local1 = ms_string_return_cs( PANGRAM1, 0 );
	str = &data->Local1;

	mst_assert(  str->Data.Char );
	mst_assert( !str->Wide );
	mst_assert( !str->Destroy );
	mst_assert( !str->Hashed );
	mst_assert( !str->MBInfo );
	mst_assert(  str->FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( str->Length,   ==, sizeof PANGRAM1 - 1 );
	mst_assert_uint ( str->Capacity, ==, sizeof PANGRAM1 );
	mst_assert_uint ( str->Hash,     ==, 0 );
	mst_assert_float( str->Modifier, ==, 2.f );

	mst_assert_cs( str->Data.Char, PANGRAM1 );

	return MSEC_OK;
}

int mst_string_create_mbs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	MS_STRING         *str;
	char               corrupted[32];

	mst_prepare( info );
	data = info->Data;

	/* próbuj utworzyć ciąg znaków z błędnych danych */
	IGRET strcpy( corrupted, "Pszczółka" );
	corrupted[6] = '\0';

	str = data->Pointer2 = ms_string_alloc_mbs( corrupted, 0 );
	mst_assert( !data->Pointer2 );

	/* teraz utwórz go */
	str = data->Pointer2 = ms_string_alloc_mbs( PANGRAM2, 100 );

	mst_assert(  str );
	mst_assert(  str->Data.Char );
	mst_assert( !str->Wide );
	mst_assert(  str->Destroy );
	mst_assert( !str->Hashed );
	mst_assert(  str->MBInfo );
	mst_assert(  str->FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( str->Length,   ==, sizeof PANGRAM2 - 1 );
	mst_assert_uint ( str->Capacity, ==, 100 );
	mst_assert_uint ( str->Hash,     ==, 0 );
	mst_assert_float( str->Modifier, ==, 2.f );

	mst_assert_mbs( str->Data.Char, PANGRAM2 );

	/* urojona długość ciągu (ilość liter wyświetlanych na ekranie)
	   poprawność tablicy została sprawdzona wcześniej, więc nie ma co się tutaj męczyć. */
	mst_assert_uint( str->MBInfo->Length, ==, ms_string_length_mbs(PANGRAM2, 0) );

	/* wielobajtowy ciąg znaków o pojemności takiej samej jak długość całego ciągu */
	data->Local2 = ms_string_return_mbs( PANGRAM2, 0 );
	str = &data->Local2;

	mst_assert(  str->Data.Char );
	mst_assert( !str->Wide );
	mst_assert( !str->Destroy );
	mst_assert( !str->Hashed );
	mst_assert(  str->MBInfo );
	mst_assert(  str->FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( str->Length,   ==, sizeof PANGRAM2 - 1 );
	mst_assert_uint ( str->Capacity, ==, sizeof PANGRAM2 );
	mst_assert_uint ( str->Hash,     ==, 0 );
	mst_assert_float( str->Modifier, ==, 2.f );

	/* rzeczywista długość wielobajtowego ciągu znaków */
	mst_assert_uint( str->MBInfo->Length, ==, ms_string_length_mbs(PANGRAM2, 0) );
	mst_assert_mbs( str->Data.Char, PANGRAM2 );

	return MSEC_OK;
}

int mst_string_create_wcs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	MS_STRING         *str;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer3 = ms_string_alloc_wcs( PANGRAM3, 100 );

	mst_assert(  str );
	mst_assert(  str->Data.Wide );
	mst_assert(  str->Wide );
	mst_assert(  str->Destroy );
	mst_assert( !str->Hashed );
	mst_assert( !str->MBInfo );
	mst_assert(  str->FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( str->Length,   ==, sizeof PANGRAM3 / sizeof(wchar_t) - 1 );
	mst_assert_uint ( str->Capacity, ==, 100 );
	mst_assert_uint ( str->Hash,     ==, 0 );
	mst_assert_float( str->Modifier, ==, 2.f );

	mst_assert_wcs( str->Data.Wide, PANGRAM3 );

	/* lokalna wersja dla wchar_t o pojemności takiej samej jak długość ciągu */
	data->Local3 = ms_string_return_wcs( PANGRAM3, 0 );

	mst_assert(  data->Local3.Data.Wide );
	mst_assert(  data->Local3.Wide );
	mst_assert( !data->Local3.Destroy );
	mst_assert( !data->Local3.Hashed );
	mst_assert( !data->Local3.MBInfo );
	mst_assert(  data->Local3.FuncIncrease == MSC_ArrayFunctions.IncMultiply );

	mst_assert_uint ( data->Local3.Length,   ==, (sizeof PANGRAM3 / sizeof(wchar_t)) - 1 );
	mst_assert_uint ( data->Local3.Capacity, ==, sizeof PANGRAM3 / sizeof(wchar_t) );
	mst_assert_uint ( data->Local3.Hash,     ==, 0 );
	mst_assert_float( data->Local3.Modifier, ==, 2.f );

	mst_assert_wcs( data->Local3.Data.Wide, PANGRAM3 );

	return MSEC_OK;
}

int mst_string_destroy( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;

	mst_prepare( info );
	data = info->Data;

	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Data.Char );
	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer2->Data.Char );
	mst_assert( data->Pointer2->MBInfo );
	mst_assert( data->Pointer3 );
	mst_assert( data->Pointer3->Data.Char );
	mst_assert( data->Local1.Data.Char );
	mst_assert( data->Local2.Data.Char );
	mst_assert( data->Local2.MBInfo );
	mst_assert( data->Local3.Data.Char );

	/* zwolnij pamięć - funkcja zwalnia wszystko */
	ms_string_free( data->Pointer1 );
	ms_string_free( data->Pointer2 );
	ms_string_free( data->Pointer3 );
	/* nie można sprawdzić danych po usunięciu struktury */

	/* tutaj widać różnicę pomiędzy clear a free */
	ms_string_clear( &data->Local1 );
	mst_assert( data->Local1.Data.Char );
	mst_assert_uint( data->Local1.Length, ==, 0 );

	ms_string_free( &data->Local1 );
	mst_assert( !data->Local1.Data.Char );

	/* tutaj clear na wielobajtowym ciągu znaków */
	ms_string_clear( &data->Local2 );
	mst_assert( data->Local2.Data.Char );
	mst_assert( data->Local2.MBInfo );
	mst_assert_uint( data->Local2.Length, ==, 0 );
	mst_assert_uint( data->Local2.MBInfo->Length, ==, 0 );

	ms_string_free( &data->Local2 );
	mst_assert( !data->Local2.Data.Char );
	mst_assert( !data->Local2.MBInfo );

	/* tutaj clear dla wchar_t */
	ms_string_clear( &data->Local3 );
	mst_assert( data->Local3.Data.Wide );
	mst_assert_uint( data->Local3.Length, ==, 0 );

	ms_string_free( &data->Local3 );
	mst_assert( !data->Local2.Data.Char );

	data->Pointer1 = NULL;
	data->Pointer2 = NULL;
	data->Pointer3 = NULL;

	/* próbuj zwolnić coś, co nie prowadzi do niczego */
	ms_string_free( data->Pointer1 );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int mst_string_realloc( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Local1.Data.Char );

	ercode = ms_string_init_cs( &data->Local1, "abc", 5 );
	str    = &data->Local1;

	mst_assert( ercode == MSEC_OK );

	str->FuncIncrease = MSC_ArrayFunctions.IncPower;
	str->Modifier     = 1.1f;

	/* 5^1.1 = ~5.87, co daje po zaokrągleniu 5 - zaokrąglenie następuje w dół
	   w takim przypadku do wartości powinno być dodane 1 */
	ercode = ms_string_realloc( str, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 6 );

	/* 6^1.71 = ~21.41, co daje po zaokrągleniu wartość 21 */
	str->Modifier = 1.71f;
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert_uint( str->Capacity, ==, 21 );

	str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
	str->Modifier     = 2.f;

	/* 21 * 2 == 42 */
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert_uint( str->Capacity, ==, 42 );

	/* 42 + 2 = 44 */
	str->FuncIncrease = MSC_ArrayFunctions.IncAdd;
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert_uint( str->Capacity, ==, 44 );

	/* zmiana pojemności ciągu do podanej wartości, w tym przypadku powinno być 15 a nie 46 (34+2) */
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 15 );
	mst_assert_uint( str->Capacity, ==, 15 );

	/* symulacja 8 elementów i zmiana pojemności ciągu do podanej wartości
	   powinien wystąpić błąd, ponieważ chcemy za bardzo zmniejszyć pamięć
	   dlaczego 8 a nie 9?
	   ciąg powinien przechowywać dodatkowe miejsce na wartość NULL */
	str->Length = 8;
	ercode = ms_string_realloc( str, 8 );
	mst_assert( ercode == MSEC_DATA_OVERFLOW );
	mst_assert_uint( str->Capacity, ==, 15 );

	/* no ale do 9 powinno się dać rady zmniejszyć */
	ercode = ms_string_realloc( str, 9 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 9 );

	/* dokładne zwiększanie, w tym przypadku spodziewamy się błędu
	 * ta technika nie działa na zwykłym realloc, lecz na min_realloc */
	str->FuncIncrease = NULL;
	ercode = ms_string_realloc( str, 0 );
	mst_assert( ercode == MSEC_INVALID_VALUE );
	mst_assert_uint( str->Capacity, ==, 9 );

	/* zmniejsz do 5 */
	str->Length = 3;
	ercode = ms_string_realloc( str, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 5 );

	return MSEC_OK;
}

int mst_string_realloc_min( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local1.Data.Char );
	str = &data->Local1;

	str->FuncIncrease = MSC_ArrayFunctions.IncPower;
	str->Modifier     = 1.1f;

	/* 5^1.1 = ~5.87 ~= 5 (+1 -> gdy lewa jest równa prawej) = 6 */
	ercode = ms_string_realloc_min( str, 6 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 6 );

	/* 6^1.5 ~= 14.70, 14^1.5 ~= 52.38 -> STOP, wartość minimalna (20) została osiągnięta */
	str->Modifier = 1.5f;
	ercode = ms_string_realloc_min( str, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 52 );

	/* w tym przypadku wartość nie powinna się zmienić, wystąpiło żądanie mniejszej wartości niż jest
	 * tutaj 0 nie oznacza wartości automatycznej, podawana jest wartość minimalna */
	ercode = ms_string_realloc_min( str, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 52 );

	/* dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna */
	str->FuncIncrease = NULL;
	ercode = ms_string_realloc_min( str, 256 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 256 );

	/* zmniejsz ilość elementów do 2 */
	ercode = ms_string_realloc( str, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, 5 );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	WSTAWIANIE ZNAKÓW / CIĄGU ZNAKÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int mst_string_cs_insert_cs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t x, y;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Pointer1 );

	str = data->Pointer1 = ms_string_alloc_cs( PANGRAM1, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* dodaj "Pack" na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 4 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( str->Length,   ==, p1s + 4 );
	mst_assert_uint( str->Capacity, ==, (p1s+1) * 2 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );

	mst_assert_uint( str->Length,   ==, p1s + 4 );
	mst_assert_uint( str->Capacity, ==, p1s * 2 + 2 );

	/* dodaj po "Pack", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 4, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( str->Length,   ==, p1s + 4 + p1s );
	mst_assert_uint( str->Capacity, ==, (p1s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );

	mst_assert_uint( str->Length,   ==, p1s + 4 + p1s );
	mst_assert_uint( str->Capacity, ==, (p1s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 4; ++x )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[x] );
	for( x = 4, y = 0; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );
	for( x = p1s + 4, y = 4; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );
	for( x = p1s + p1s, y = 0; y < 4; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );

	return MSEC_OK;
}

int mst_string_cs_insert_mbs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t p2s = ms_string_length_mbs( PANGRAM2, 0 );
	size_t x, y;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer1;
	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_cs( str, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* teraz powinno dodać - "Pchnąć" na sam koniec */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p1s + 6 );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p1s + 6 );

	/* dodaj po "Pack", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 4, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p1s + p2s + 6 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM2, sizeof PANGRAM2 - 1 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p1s + p2s + 6 );

	/* pobierz informacje o wielobajtowym ciągu znaków dla sprawdzenia */
	ms_array_clear( &data->Array );
	ercode = ms_string_info_mbs( PANGRAM2, p2s, &data->Array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 4; ++x )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[x] );
	for( x = 4, y = 0; y < data->Array.Length; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[ms_array_getl(data->Array, MS_MBINFO, y).Offset] );
	for( x = p2s + 4, y = 4; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );
	for( x = p2s + p1s, y = 0; y < 6; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[ms_array_getl(data->Array, MS_MBINFO, y).Offset] );

	return MSEC_OK;
}

int mst_string_cs_insert_wcs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t p3s = sizeof PANGRAM3 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer1;
	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_cs( str, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj część na koniec - "Pchnąć"" */
	ercode = ms_string_push_wcs( str, PANGRAM3, 6 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p1s + 6 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p1s + 6 );

	/* dodaj po "Pack", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 4, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p1s + 6 + p3s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM3, p3s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p1s + 6 + p3s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 4; ++x )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[x] );
	for( x = 4, y = 0; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, (char)PANGRAM3[y] );
	for( x = p3s + 4, y = 4; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );
	for( x = p1s + p3s, y = 0; y < 6; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, (char)PANGRAM3[y] );

	return MSEC_OK;
}

int mst_string_mbs_insert_mbs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p2sm = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p2s  = sizeof PANGRAM2 - 1;
	size_t x, y, offset;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Pointer2 );

	str = data->Pointer2 = ms_string_alloc_mbs( PANGRAM2, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );
	mst_assert_uint( str->Capacity, ==, p2s + 1 );

	/* teraz powinno dodać - "Pchnąć" */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Capacity, ==, (p2s+1) * 2 );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );

	mst_assert_uint( str->Length,   ==, p2s + 8 );
	mst_assert_uint( str->Capacity, ==, (p2s+1) * 2 );

	/* dodaj zaraz po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 6, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( str->Length,         ==, p2s * 2 + 8 );
	mst_assert_uint( str->MBInfo->Length, ==, p2sm * 2 + 6 );
	mst_assert_uint( str->Capacity,       ==, (p2s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM2, p2s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );

	mst_assert_uint( str->Length,         ==, p2s * 2 + 8 );
	mst_assert_uint( str->MBInfo->Length, ==, p2sm * 2 + 6 );
	mst_assert_uint( str->Capacity,       ==, (p2s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 8; ++x )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[x] );
	for( x = 8, y = 0; y < p2s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[y] );
	for( x = p2s + 8, y = 8; y < p2s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[y] );
	for( x = p2s * 2, y = 0; y < 8; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 6; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}
	for( offset = PANGRAM2OF[x], x = 6, y = 0; y < p2sm; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}
	for( offset = PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm + 6, y = 6; y < p2sm; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}
	for( offset += PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm * 2, y = 0; y < 6; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_uint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_uint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}

	return MSEC_OK;
}

int mst_string_mbs_insert_cs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s  = sizeof PANGRAM1 - 1;
	size_t p2sm = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p2s  = sizeof PANGRAM2 - 1;
	size_t x, y, offset;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer2;
	mst_assert( str );
	mst_assert( str->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_mbs( str, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj "Pack" na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 4 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p2s + 4 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p2s + 4 );

	/* dodaj po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 6, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p2s + 4 + p1s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p2s + 4 + p1s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 8; ++x )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[x] );
	for( x = 8, y = 0; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );
	for( x = p1s + 8, y = 8; y < p2s; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM2[y] );
	for( x = p2s + p1s, y = 0; y < 4; ++x, ++y )
		mst_assert_sint( str->Data.Char[x], ==, PANGRAM1[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 6; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert( mbinfo->Bytes  == PANGRAM2MB[x] );
		mst_assert( mbinfo->Offset == PANGRAM2OF[x] );
	}
	for( offset = PANGRAM2OF[x], x = 6, y = 0; y < p1s; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert( mbinfo->Bytes  == 1 );
		mst_assert( mbinfo->Offset == y + offset );
	}
	for( offset = y, x = p1s + 6, y = 6; y < p2sm; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert( mbinfo->Bytes  == PANGRAM2MB[y] );
		mst_assert( mbinfo->Offset == PANGRAM2OF[y] + offset );
	}
	for( offset += PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm + p1s, y = 0; y < 4; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert( mbinfo->Bytes  == 1 );
		mst_assert( mbinfo->Offset == y + offset );
	}

	return MSEC_OK;
}

int mst_string_mbs_insert_wcs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p2sm = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p2s  = sizeof PANGRAM2 - 1;
	size_t p3s  = sizeof PANGRAM3 - 1;
	size_t p3sm = ms_string_length_wcstombs( PANGRAM3, 0 );
	size_t x, y, offset;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer2;
	mst_assert( str );
	mst_assert( str->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_mbs( str, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj "Pchnąć" na koniec */
	ercode = ms_string_push_wcs( str, PANGRAM3, 6 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p2s + 8 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p2s + 8 );

	/* dodaj po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 8, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p2s + 8 + p3sm );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM3, p3s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p2s + 8 + p3sm );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 10; ++x )
		mst_assert_uint( str->Data.Char[x], ==, PANGRAM2[x] );
	for( x = 10, y = 0; y < p2s; ++x, ++y )
		mst_assert_uint( str->Data.Char[x], ==, PANGRAM2[y] );
	for( x = p2s + 10, y = 10; y < p2s; ++x, ++y )
		mst_assert_uint( str->Data.Char[x], ==, PANGRAM2[y] );
	for( x = p2s * 2, y = 0; y < 8; ++x, ++y )
		mst_assert_uint( str->Data.Char[x], ==, PANGRAM2[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 8; ++x )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_sint( mbinfo->Bytes,  ==, PANGRAM2MB[x] );
		mst_assert_sint( mbinfo->Offset, ==, PANGRAM2OF[x] );
	}
	for( offset = PANGRAM2OF[x], x = 8, y = 0; y < p2sm; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_sint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_sint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}
	for( offset = PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm + 8, y = 8; y < p2sm; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_sint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_sint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}
	for( offset += PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm * 2, y = 0; y < 6; ++x, ++y )
	{
		MS_MBINFO *mbinfo = &ms_array_get(str->MBInfo, MS_MBINFO, x);
		mst_assert_sint( mbinfo->Bytes,  ==, PANGRAM2MB[y] );
		mst_assert_sint( mbinfo->Offset, ==, PANGRAM2OF[y] + offset );
	}

	return MSEC_OK;
}

int mst_string_wcs_insert_wcs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p3s = sizeof PANGRAM3 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Pointer3 );

	/* przypadek 1 - wstawiany ciąg znaków jest ciągiem znaków wchar_t */
	str = data->Pointer3 = ms_string_alloc_wcs( PANGRAM3, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* dodaj "Pchnąć" na koniec */
	ercode = ms_string_push_wcs( str, PANGRAM3, 6 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( str->Length,   ==, p3s + 6 );
	mst_assert_uint( str->Capacity, ==, (p3s+1) * 2 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );

	mst_assert_uint( str->Length,   ==, p3s + 6 );
	mst_assert_uint( str->Capacity, ==, p3s * 2 + 2 );

	/* dodaj po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 6, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( str->Length,   ==, p3s * 2 + 6 );
	mst_assert_uint( str->Capacity, ==, (p3s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM3, p3s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );

	mst_assert_uint( str->Length,   ==, p3s * 2 + 6 );
	mst_assert_uint( str->Capacity, ==, (p3s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 6; ++x )
		mst_assert( str->Data.Wide[x] == PANGRAM3[x] );
	for( x = 6, y = 0; y < p3s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM3[y] );
	for( x = p3s + 6, y = 6; y < p3s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM3[y] );
	for( x = p3s + p3s, y = 0; y < 6; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM3[y] );

	return MSEC_OK;
}

int mst_string_wcs_insert_cs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t p3s = sizeof PANGRAM3 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer3;
	mst_assert( str );
	mst_assert( str->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_wcs( str, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj "Pack" na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 4 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p3s + 4 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p3s + 4 );

	/* dodaj po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 6, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p3s + 4 + p1s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p3s + 4 + p1s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 6; ++x )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[x] );
	for( x = 6, y = 0; y < p1s; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, (wchar_t)PANGRAM1[y] );
	for( x = p1s + 6, y = 6; y < p3s; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[y] );
	for( x = p1s + p3s, y = 0; y < 4; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, (wchar_t)PANGRAM1[y] );

	return MSEC_OK;
}

int mst_string_wcs_insert_mbs( MST_FUNCTION *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p2s = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p3s = sizeof PANGRAM3 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_prepare( info );
	data = info->Data;

	str = data->Pointer3;
	mst_assert( str );
	mst_assert( str->Data.Char );

	ms_string_clear( str );
	ercode = ms_string_push_wcs( str, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );
	mst_assert_uint( str->Length, ==, p3s );

	/* teraz powinno dodać - "Pchnąć" */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p3s + 6 );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( str->Length, ==, p3s + 6 );

	/* dodaj po "Pchnąć", automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 6, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( str->Length, ==, p2s + p3s + 6 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM2, sizeof PANGRAM2 - 1 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( str->Length, ==, p2s + p3s + 6 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 6; ++x )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[x] );
	for( x = 6, y = 0; y < p2s; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[y] );
	for( x = p2s + 6, y = 6; y < p3s; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[y] );
	for( x = p2s + p3s, y = 0; y < 6; ++x, ++y )
		mst_assert_sint( str->Data.Wide[x], ==, PANGRAM3[y] );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	URUCHAMIANIE TESTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Opisy funkcji testujących.
 * Wyświetlane są zaraz pod nazwą funkcji podczas testowania.
 * Uporzadkowane w kolejności rozmieszczenia funkcji w liście.
 */
#define FUNC_DESC_01 "Count characters in multibyte string."
#define FUNC_DESC_02 "Check length of string after wcstombs conversion."
#define FUNC_DESC_03 "Get info about each multibyte character in string."
#define FUNC_DESC_04 "Get info about each character in string after wcstombs conversion."
#define FUNC_DESC_05 "Create single byte string by alloc, init and return."
#define FUNC_DESC_06 "Create multibyte string by alloc, init and return."
#define FUNC_DESC_07 "Create wide string by alloc, init and return."
#define FUNC_DESC_08 "Free memory allocated for string in constructors."
#define FUNC_DESC_09 "Change string capacity by using simple realloc version."
#define FUNC_DESC_10 "Change string capacity by using minimum realloc version."
#define FUNC_DESC_11 "Insert single byte string into single byte string."
#define FUNC_DESC_12 "Insert multibyte string into single byte string."
#define FUNC_DESC_13 "Insert wide string into single byte string."
#define FUNC_DESC_14 "Insert multibyte string into multibyte string."
#define FUNC_DESC_15 "Insert single byte string into multibyte string."
#define FUNC_DESC_16 "Insert wide string into multibyte string."
#define FUNC_DESC_17 "Insert wide string into wide string."
#define FUNC_DESC_18 "Insert single byte string into wide string."
#define FUNC_DESC_19 "Insert multibyte string into wide string."

/**
 * Lista funkcji testujących moduł.
 * Każda z osobna uruchamiana jest przez odpowiednią funkcję.
 * Dzięki zbiorowi funkcji zdefiniowanemu poniżej dzieje się to automatycznie.
 */
MST_FUNCTION MSV_StringSuiteFunctions[] =
{
	{ MST_STRINGIFY(mst_string_length_mbs),      FUNC_DESC_01, NULL },
	{ MST_STRINGIFY(mst_string_length_wcstombs), FUNC_DESC_02, NULL },
	{ MST_STRINGIFY(mst_string_info_mbs),        FUNC_DESC_03, NULL },
	{ MST_STRINGIFY(mst_string_info_wcstombs),   FUNC_DESC_04, NULL },
	{ MST_STRINGIFY(mst_string_create_cs),       FUNC_DESC_05, NULL },
	{ MST_STRINGIFY(mst_string_create_mbs),      FUNC_DESC_06, NULL },
	{ MST_STRINGIFY(mst_string_create_wcs),      FUNC_DESC_07, NULL },
	{ MST_STRINGIFY(mst_string_destroy),         FUNC_DESC_08, NULL },
	{ MST_STRINGIFY(mst_string_realloc),         FUNC_DESC_09, NULL },
	{ MST_STRINGIFY(mst_string_realloc_min),     FUNC_DESC_10, NULL },
	{ MST_STRINGIFY(mst_string_cs_insert_cs),    FUNC_DESC_11, NULL },
	{ MST_STRINGIFY(mst_string_cs_insert_mbs),   FUNC_DESC_12, NULL },
	{ MST_STRINGIFY(mst_string_cs_insert_wcs),   FUNC_DESC_13, NULL },
	{ MST_STRINGIFY(mst_string_mbs_insert_mbs),  FUNC_DESC_15, NULL },
	{ MST_STRINGIFY(mst_string_mbs_insert_cs),   FUNC_DESC_14, NULL },
	{ MST_STRINGIFY(mst_string_mbs_insert_wcs),  FUNC_DESC_16, NULL },
	{ MST_STRINGIFY(mst_string_wcs_insert_wcs),  FUNC_DESC_19, NULL },
	{ MST_STRINGIFY(mst_string_wcs_insert_cs),   FUNC_DESC_17, NULL },
	{ MST_STRINGIFY(mst_string_wcs_insert_mbs),  FUNC_DESC_18, NULL },
	{ MST_LASTRECORD }
};

/**
 * Zbiór funkcji testujących moduł.
 * Przekazywany do funkcji main, pozwala na uruchomienie wszystkich testów.
 */
MST_SUITE MSV_StringSuite =
{
	">>> STRING MODULE",
	TRUE,
	NULL,
	mst_string_teardown,
	NULL,
	MSV_StringSuiteFunctions
};

/* w przypadku wszystkich zestawów na raz nie dołączaj funkcji main */
#ifndef MST_ALL_SUITES

	/**
	 * Funkcja główna dołączana tylko przy uruchomieniu tego testu.
	 * W przypadku testowania wszystkich modułów na raz, nie jest uwzględniana w kompilacji.
	 * 
	 * @param argc Ilość parametrów przekazywanych do programu.
	 * @param argv Parametry przekazane do programu z wiersza poleceń.
	 * 
	 * @return Kod błędu lub wartość 0.
	 */
	int main( int argc, char **argv )
	{
		MSTST_STRING_DATA data = { NULL, NULL, NULL };

		setlocale( LC_ALL, "pl_PL.utf8" );

		IGVAR argc;
		IGVAR argv;

		/* przypisz dane do zbioru */
		MSV_StringSuite.Data = &data;

		return mst_run_suite( &MSV_StringSuite );
	}

#endif
