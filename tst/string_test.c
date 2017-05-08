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
const char    PANGRAM1[] =  "Pack my box with five dozen liquor jugs.";                   /* angielski */
const char    PANGRAM2[] =  "Pchnąć w tę łódź jeża lub ośm skrzyń fig.";                  /* polski */
const char    PANGRAM3[] =  "Jámbor célú, öv ügyű ex-qwan ki dó-s főz, puhít.";           /* węgierski */
const wchar_t PANGRAM4[] = L"Съешь ещё этих мягких французских булок, да выпей же чаю.";  /* rosyjski */
const wchar_t PANGRAM5[] = L"키스의 고유조건은 입술끼리 만나야 하고 특별한 기술은 필요치 않다.";  /* koreański */
const char    PANGRAM6[] =  "Съешь ещё этих мягких французских булок, да выпей же чаю.";  /* rosyjski - char */
const wchar_t PANGRAM7[] = L"Pchnąć w tę łódź jeża lub ośm skrzyń fig.";                  /* polski - wchar_t */

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
/* ilość bajtów przypadająca na dany znak dla węgierskiego pangramu */
const int PANGRAM3MB[] = {
/*  J  á  m  b  o  r     c  é  l  ú  ,     ö  v     ü  g  y  ű */
	1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2,
/*     e  x  -  q  w  a  n     k  i     d  ó  -  s     f  ő  z */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1,
/*  ,     p  u  h  í  t  . */
	1, 1, 1, 1, 1, 2, 1, 1
};
/* przesunięcie indeksu bajtu względem danego znaku dla węgierskiego pangramu */
const int PANGRAM3OF[] = {
/*  J   á   m   b   o   r       c   é   l   ú   ,       ö   v       ü   g   y   ű */
	0,  1,  3,  4,  5,  6,  7,  8,  9, 11, 12, 14, 15, 16, 18, 19, 20, 22, 23, 24,
/*      e   x   -   q   w   a   n       k   i       d   ó   -   s       f   ő   z  */
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 41, 42, 43, 44, 45, 47,
/*  ,       p   u   h   í   t   . */
	48, 49, 50, 51, 52, 53, 55, 56
};
/* ilość bajtów przypadająca na dany znak dla rosyjskiego pangramu */
const int PANGRAM4MB[] = {
/*  С  ъ  е  ш  ь     е  щ  ё     э  т  и  х     м  я  г  к  и */
	2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2,
/*  х     ф  р  а  н  ц  у  з  с  к  и  х     б  у  л  о  к  , */
	2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
/*     д  а     в  ы  п  е  й     ж  е     ч  а  ю  . */
	1, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1
};
/* przesunięcie indeksu bajtu względem danego znaku dla rosyjskiego pangramu */
const int PANGRAM4OF[] = {
/*  С   ъ   е   ш   ь       е   щ   ё       э   т   и   х       м   я   г   к   и */
	0,  2,  4,  6,  8,  10, 11, 13, 15, 17, 18, 20, 22, 24, 26, 27, 29, 31, 33, 35,
/*  х       ф   р   а   н   ц   у   з   с   к   и   х       б   у   л   о   к   , */
	37, 39, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73,
/*      д   а       в   ы   п   е   й       ж   е       ч   а   ю    . */
	74, 75, 77, 79, 80, 82, 84, 86, 88, 90, 91, 93, 95, 96, 98, 100, 102
};
/* ilość bajtów przypadająca na dany znak dla koreańskiego pangramu */
const int PANGRAM5MB[] = {
/*  키 스  의    고 유 조 건  은    입 술 끼  리    만 나  야    하 */
	3, 3, 3, 1, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 1, 3, 3, 3, 1, 3,
/*  고    특 별  한    기 술 은    필  요 치     않 다 . */
	3, 1, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 3, 1, 3, 3, 1
};
/* przesunięcie indeksu bajtu względem danego znaku dla koreańskiego pangramu */
const int PANGRAM5OF[] = {
/*  키  스   의      고  유   조  건  은      입   술  끼  리      만   나  야      하 */
	0,  3,  6,  9,  10, 13, 16, 19, 22, 25, 26, 29, 32, 35, 38, 39, 42, 45, 48, 49,
/*  고      특   별  한      기  술   은      필  요   치      않  다   . */
	52, 55, 56, 59, 62, 65, 66, 69, 72, 75, 76, 79, 82, 85, 86, 89, 92
};

/**
 * Funkcja wywoływana przy kończeniu testu.
 * Zwalnia miejsce przydzielone w pamięci podczas działania programu.
 */
void mst_string_teardown( MST_TESTSUITE *suite )
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
	TWORZENIE I NISZCZENIE CIĄGU ZNAKÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

char *mst_string_create( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	char baddata[128];

	mst_assert( info );
	data = info->Data;

	/* jednobajtowy ciąg znaków */
	data->Pointer1 = ms_string_alloc_cs( PANGRAM1, 100 );

	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Data.Char );
	mst_assert( data->Pointer1->Length == sizeof PANGRAM1 - 1 );
	mst_assert( data->Pointer1->Capacity == 100 );
	mst_assert( data->Pointer1->Hash == 0 );
	mst_assert( data->Pointer1->Wide == FALSE );
	mst_assert( data->Pointer1->Destroy == TRUE );
	mst_assert( data->Pointer1->Hashed == FALSE );
	mst_assert( data->Pointer1->MBInfo == NULL );
	mst_assert( data->Pointer1->FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Pointer1->Modifier == 2.f );

	mst_assert( strcmp(data->Pointer1->Data.Char, PANGRAM1) == 0 );

	/* próbuj utworzyć ciąg z błędnych danych */
	IGRET strcpy( baddata, PANGRAM2 );
	baddata[7] = '\0';

	data->Pointer2 = ms_string_alloc_mbs( baddata, 5 );
	mst_assert( !data->Pointer2 );

	/* wielobajtowy ciąg znaków */
	data->Pointer2 = ms_string_alloc_mbs( PANGRAM2, 100 );

	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer2->Data.Char );
	mst_assert( data->Pointer2->Length == sizeof PANGRAM2 - 1 );
	mst_assert( data->Pointer2->Capacity == 100 );
	mst_assert( data->Pointer2->Hash == 0 );
	mst_assert( data->Pointer2->Wide == FALSE );
	mst_assert( data->Pointer2->Destroy == TRUE );
	mst_assert( data->Pointer2->Hashed == FALSE );
	mst_assert( data->Pointer2->MBInfo != NULL );
	mst_assert( data->Pointer2->FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Pointer2->Modifier == 2.f );

	/* rzeczywista długość wielobajtowego ciągu znaków */
	mst_assert( data->Pointer2->MBInfo->Length == 41 );
	mst_assert( strcmp(data->Pointer2->Data.Char, PANGRAM2) == 0 );

	/* wchar_t - UTF32 lub UTF16/UCS-2 */
	data->Pointer3 = ms_string_alloc_wcs( PANGRAM4, 100 );

	mst_assert( data->Pointer3 );
	mst_assert( data->Pointer3->Data.Wide );
	mst_assert( data->Pointer3->Length == (sizeof PANGRAM4 / sizeof(wchar_t)) - 1 );
	mst_assert( data->Pointer3->Capacity == 100 );
	mst_assert( data->Pointer3->Hash == 0 );
	mst_assert( data->Pointer3->Wide == TRUE );
	mst_assert( data->Pointer3->Destroy == TRUE );
	mst_assert( data->Pointer3->Hashed == FALSE );
	mst_assert( data->Pointer3->MBInfo == NULL );
	mst_assert( data->Pointer3->FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Pointer3->Modifier == 2.f );

	mst_assert( wcscmp(data->Pointer3->Data.Wide, PANGRAM4) == 0 );

	/* jednobajtowy ciąg znaków o pojemności takiej samej jak długość całego ciągu */
	data->Local1 = ms_string_return_cs( PANGRAM1, 0 );

	mst_assert( data->Local1.Data.Char );
	mst_assert( data->Local1.Length == sizeof PANGRAM1 - 1 );
	mst_assert( data->Local1.Capacity == sizeof PANGRAM1 );
	mst_assert( data->Local1.Hash == 0 );
	mst_assert( data->Local1.Wide == FALSE );
	mst_assert( data->Local1.Destroy == FALSE );
	mst_assert( data->Local1.Hashed == FALSE );
	mst_assert( data->Local1.MBInfo == NULL );
	mst_assert( data->Local1.FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Local1.Modifier == 2.f );

	mst_assert( strcmp(data->Local1.Data.Char, PANGRAM1) == 0 );

	/* wielobajtowy ciąg znaków o pojemności takiej samej jak długość całego ciągu */
	data->Local2 = ms_string_return_mbs( PANGRAM3, 0 );

	mst_assert( data->Local2.Data.Char );
	mst_assert( data->Local2.Length == sizeof PANGRAM3 - 1 );
	mst_assert( data->Local2.Capacity == sizeof PANGRAM3 );
	mst_assert( data->Local2.Hash == 0 );
	mst_assert( data->Local2.Wide == FALSE );
	mst_assert( data->Local2.Destroy == FALSE );
	mst_assert( data->Local2.Hashed == FALSE );
	mst_assert( data->Local2.MBInfo != NULL );
	mst_assert( data->Local2.FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Local2.Modifier == 2.f );

	/* rzeczywista długość wielobajtowego ciągu znaków */
	mst_assert( data->Local2.MBInfo->Length == 48 );
	mst_assert( strcmp(data->Local2.Data.Char, PANGRAM3) == 0 );

	/* wchar_t - UTF32 lub UTF16/UCS-2 */
	data->Local3 = ms_string_return_wcs( PANGRAM5, 0 );

	mst_assert( data->Local3.Data.Wide );
	mst_assert( data->Local3.Length == (sizeof PANGRAM5 / sizeof(wchar_t)) - 1 );
	mst_assert( data->Local3.Capacity == sizeof PANGRAM5 / sizeof(wchar_t) );
	mst_assert( data->Local3.Hash == 0 );
	mst_assert( data->Local3.Wide == TRUE );
	mst_assert( data->Local3.Destroy == FALSE );
	mst_assert( data->Local3.Hashed == FALSE );
	mst_assert( data->Local3.MBInfo == NULL );
	mst_assert( data->Local3.FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Local3.Modifier == 2.f );

	mst_assert( wcscmp(data->Local3.Data.Wide, PANGRAM5) == 0 );

	return MST_SUCCESS;
}

char *mst_string_destroy( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;

	mst_assert( info );
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
	mst_assert( data->Local1.Length == 0 );
	ms_string_free( &data->Local1 );
	mst_assert( !data->Local1.Data.Char );

	/* tutaj clear na wielobajtowym ciągu znaków */
	ms_string_clear( &data->Local2 );
	mst_assert( data->Local2.Data.Char );
	mst_assert( data->Local2.MBInfo );
	mst_assert( data->Local2.Length == 0 );
	mst_assert( data->Local2.MBInfo->Length == 0 );
	ms_string_free( &data->Local2 );
	mst_assert( !data->Local2.Data.Char );
	mst_assert( !data->Local2.MBInfo );

	/* tutaj clear dla wchar_t */
	ms_string_clear( &data->Local3 );
	mst_assert( data->Local3.Data.Wide );
	mst_assert( data->Local3.Length == 0 );
	ms_string_free( &data->Local3 );
	mst_assert( !data->Local2.Data.Char );

	data->Pointer1 = NULL;
	data->Pointer2 = NULL;
	data->Pointer3 = NULL;

	/* próbuj zwolnić coś, co nie prowadzi do niczego */
	ms_string_free( data->Pointer1 );

	return MST_SUCCESS;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

char *mst_string_realloc( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	mst_assert( info );
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
	mst_assert( str->Capacity == 6 );

	/* 6^1.71 = ~21.41, co daje po zaokrągleniu wartość 21 */
	str->Modifier = 1.71f;
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert( str->Capacity == 21 );

	str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
	str->Modifier     = 2.f;

	/* 21 * 2 == 42 */
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert( str->Capacity == 42 );

	/* 42 + 2 = 44 */
	str->FuncIncrease = MSC_ArrayFunctions.IncAdd;
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 0 );
	mst_assert( str->Capacity == 44 );

	/* zmiana pojemności ciągu do podanej wartości, w tym przypadku powinno być 15 a nie 46 (34+2) */
	mst_assert( ercode == MSEC_OK );
	ercode = ms_string_realloc( str, 15 );
	mst_assert( str->Capacity == 15 );

	/* symulacja 8 elementów i zmiana pojemności ciągu do podanej wartości
	   powinien wystąpić błąd, ponieważ chcemy za bardzo zmniejszyć pamięć
	   dlaczego 8 a nie 9?
	   ciąg powinien przechowywać dodatkowe miejsce na wartość NULL */
	str->Length = 8;
	ercode = ms_string_realloc( str, 8 );
	mst_assert( ercode == MSEC_DATA_OVERFLOW );
	mst_assert( str->Capacity == 15 );

	/* no ale do 9 powinno się dać rady zmniejszyć */
	ercode = ms_string_realloc( str, 9 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 9 );

	/* dokładne zwiększanie, w tym przypadku spodziewamy się błędu
	 * ta technika nie działa na zwykłym realloc, lecz na min_realloc */
	str->FuncIncrease = NULL;
	ercode = ms_string_realloc( str, 0 );
	mst_assert( ercode == MSEC_INVALID_VALUE );
	mst_assert( str->Capacity == 9 );

	/* zmniejsz do 5 */
	str->Length = 3;
	ercode = ms_string_realloc( str, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 5 );

	return MST_SUCCESS;
}

char *mst_string_realloc_min( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	mst_assert( info );
	data = info->Data;
	mst_assert( data->Local1.Data.Char );
	str = &data->Local1;

	str->FuncIncrease = MSC_ArrayFunctions.IncPower;
	str->Modifier     = 1.1f;

	/* 5^1.1 = ~5.87 ~= 5 (+1 -> gdy lewa jest równa prawej) = 6 */
	ercode = ms_string_realloc_min( str, 6 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 6 );

	/* 6^1.5 ~= 14.70, 14^1.5 ~= 52.38 -> STOP, wartość minimalna (20) została osiągnięta */
	str->Modifier = 1.5f;
	ercode = ms_string_realloc_min( str, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 52 );

	/* w tym przypadku wartość nie powinna się zmienić, wystąpiło żądanie mniejszej wartości niż jest
	 * tutaj 0 nie oznacza wartości automatycznej, podawana jest wartość minimalna */
	ercode = ms_string_realloc_min( str, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 52 );

	/* dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna */
	str->FuncIncrease = NULL;
	ercode = ms_string_realloc_min( str, 256 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 256 );

	/* zmniejsz ilość elementów do 2 */
	ercode = ms_string_realloc( str, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == 5 );

	return MST_SUCCESS;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DŁUGOŚĆ CIĄGU / INFORMACJE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

char *mst_string_length( MST_TESTFUNC *info )
{
	size_t  length;
	wchar_t errsim[5] = { '\0', '\0', '\0', '\0', '\0' };

	/* długość całego tekstu - 41 znaków, 50 bajtów */
	length = ms_string_length_mbs( PANGRAM2, 0 );
	mst_assert( length == 41 );

	/* słowo: Pchnąć - 6 znaków, 8 bajtów */
	length = ms_string_length_mbs( PANGRAM2, 8 );
	mst_assert( length == 6 );

	/* słowo: łódź - 4 znaki, 7 bajtów */
	length = ms_string_length_mbs( &PANGRAM2[15], 7 );
	mst_assert( length == 4 );

	/* powinien być błąd - błędny znak po literze d w słowie łódź
	   dlatego, że ź nie zostało podane w całości, ale w połowie. */
	length = ms_string_length_mbs( &PANGRAM2[15], 6 );
	mst_assert( length == 0 );
	/* można sprawdzić jeszcze ERRNO */

	/* a teraz próba podania za dużej ilości bajtów (sam wyraz ma 50)
	   o ile ciąg zakończony jest znakiem \0, to wszystko powinno być w porządku. */
	length = ms_string_length_mbs( PANGRAM2, 100 );
	mst_assert( length == 41 );

	/* obliczanie długości wielobajtowego ciągu z wchar_t
	   obliczana jest ilość bajtów, z których będzie się składał ciąg */
	length = ms_string_length_wcstombs( PANGRAM5, 0 );
	mst_assert( length == 93 );

	/* słowo: 고유조건은 - 5 znaków, ale 15 bajtów */
	length = ms_string_length_wcstombs( &PANGRAM5[4], 5 );
	mst_assert( length == 15 );

	/* próba symulacji błędu */
	errsim[0] = PANGRAM5[4];
	errsim[1] = PANGRAM5[5];
	errsim[2] = WCHAR_MIN; /* symulacja błędu (być może tylko na linuksie) */
	errsim[3] = PANGRAM5[7];

	length = ms_string_length_wcstombs( errsim, 4 );
	mst_assert( length == 0 );

	/* próba podania za dużej ilości znaków
	   jeżeli ciąg zakończony jest znakiem L\0, to wszystko powinno być w porządku */
	length = ms_string_length_wcstombs( PANGRAM5, 100 );
	mst_assert( length == 93 );

	return MST_SUCCESS;
}

char *mst_string_info_mbs( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	size_t             x, length;
	MS_ARRAY          *array;

	mst_assert( info );
	data = info->Data;

	data->Array = ms_array_return_local( sizeof(MS_MBINFO), 0 );
	array = &data->Array;

	/* pobierz tablicę z informacjami o znakach */
	ercode = ms_string_info_mbs( PANGRAM2, 0, array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź jej długość */
	length = ms_string_length_mbs( PANGRAM2, 0 );
	mst_assert( length == array->Length );

	/* i dane czy są poprawne */
	for( x = 0; x < array->Length; ++x )
	{
		mst_assert( ms_array_get(array, MS_MBINFO, x).Bytes  == PANGRAM2MB[x] );
		mst_assert( ms_array_get(array, MS_MBINFO, x).Offset == PANGRAM2OF[x] );
	}

	/* wyczyść tablicę, dane nie są potrzebne w tym momencie */
	ms_array_clear( array );

	/* spróbuj jeszcze stworzyć tablicę dla danych, zawierających błędny znak */
	ercode = ms_string_info_mbs( PANGRAM2, 7, array );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* dane są dodawane do momentu wystąpienia błędu */
	mst_assert( array->Length == 5 );

	/* wyczyść tablicę po raz kolejny */
	ms_array_clear( array );

	/* pobierz tablicę z informacjami dla języka węgierskiego,
	   podając zbyt dużą wielkość ciągu znaków - funkcja powinna sobie z tym poradzić */
	ercode = ms_string_info_mbs( PANGRAM3, 100, array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź jej długość */
	length = ms_string_length_mbs( PANGRAM3, 0 );
	mst_assert( length == array->Length );

	/* oraz dane, jak wcześniej */
	for( x = 0; x < array->Length; ++x )
	{
		mst_assert( ms_array_get(array, MS_MBINFO, x).Bytes  == PANGRAM3MB[x] );
		mst_assert( ms_array_get(array, MS_MBINFO, x).Offset == PANGRAM3OF[x] );
	}

	/* sprawdźmy jeszcze dla ciągu jednobajtowego, powinien mieć same 1 */
	ms_array_clear( array );

	ercode = ms_string_info_mbs( PANGRAM1, 0, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert( array->Length == sizeof PANGRAM1 - 1 );

	for( x = 0; x < array->Length; ++x )
	{
		mst_assert( ms_array_get(array, MS_MBINFO, x).Bytes  == 1 );
		mst_assert( ms_array_get(array, MS_MBINFO, x).Offset == x );
	}

	return MST_SUCCESS;
}

char *mst_string_info_wcstombs( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	size_t             x;
	MS_ARRAY          *array;
	wchar_t            errsim[5] = { '\0', '\0', '\0', '\0', '\0' };

	mst_assert( info );
	data = info->Data;

	/* tablica powinna być zainicjalizowana we wcześniejszej funkcji */
	mst_assert( data->Array.Items );
	array = &data->Array;

	/* ale trzeba wyczyścić */
	ms_array_clear( array );

	/* pobierz strukturę dla wielobajtowego ciągu znaków wprost z typu wchar_t */
	ercode = ms_string_info_wcstombs( PANGRAM4, 0, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert( array->Length == sizeof PANGRAM4 / sizeof(wchar_t) - 1 );

	/* porównaj dane z poprawnymi */
	for( x = 0; x < array->Length; ++x )
	{
		mst_assert( ms_array_get(array, MS_MBINFO, x).Bytes  == PANGRAM4MB[x] );
		mst_assert( ms_array_get(array, MS_MBINFO, x).Offset == PANGRAM4OF[x] );
	}

	/* wyczyść tablicę, dane nie są potrzebne w tym momencie */
	ms_array_clear( array );

	/* próba symulacji błędu */
	errsim[0] = PANGRAM4[4];
	errsim[1] = PANGRAM4[5];
	errsim[2] = WCHAR_MIN; /* symulacja błędu (być może tylko na linuksie) */
	errsim[3] = PANGRAM4[7];

	/* spróbuj jeszcze stworzyć tablicę dla danych, zawierających błędny znak */
	ercode = ms_string_info_wcstombs( errsim, 4, array );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* dane są dodawane do momentu wystąpienia błędu */
	mst_assert( array->Length == 2 );

	ms_array_clear( array );

	/* teraz to samo dla koreańskiego - przekaż za dużą ilość znaków w ciągu,
	   powinno wszystko być w porządku z powodu ograniczenia */
	ercode = ms_string_info_wcstombs( PANGRAM5, 100, array );
	mst_assert( ercode == MSEC_OK );
	mst_assert( array->Length == sizeof PANGRAM5 / sizeof(wchar_t) - 1 );

	/* porównaj dane z poprawnymi */
	for( x = 0; x < array->Length; ++x )
	{
		mst_assert( ms_array_get(array, MS_MBINFO, x).Bytes  == PANGRAM5MB[x] );
		mst_assert( ms_array_get(array, MS_MBINFO, x).Offset == PANGRAM5OF[x] );
	}

	return MST_SUCCESS;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	WSTAWIANIE ZNAKÓW / CIĄGU ZNAKÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

char *mst_string_cs_insert_string( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t p2s = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p4s = sizeof PANGRAM4 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_assert( info );
	data = info->Data;
	mst_assert( !data->Pointer1 );

	/* przypadek 1 - wstawiany ciąg znaków jest ciągiem jednobajtowym */
	str = data->Pointer1 = ms_string_alloc_cs( PANGRAM1, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* dodaj część na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p1s + 5 );
	mst_assert( str->Capacity == (p1s+1) * 2 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p1s + 5 );
	mst_assert( str->Capacity == p1s * 2 + 2 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 5, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p1s + 5 + p1s );
	mst_assert( str->Capacity == (p1s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p1s + 5 + p1s );
	mst_assert( str->Capacity == (p1s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 5; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM1[x] );
	for( x = 5, y = 0; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );
	for( x = p1s + 5, y = 5; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );
	for( x = p1s + p1s, y = 0; y < 5; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );

	/* przypadek 2 - wstawiany ciąg znaków jest ciągiem wielobajtowym */
	ms_string_clear( str );
	ercode = ms_string_push_cs( str, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* teraz powinno dodać */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p1s + 6 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 5, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p1s + p2s + 6 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM2, sizeof PANGRAM2 - 1 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p1s + p2s + 6 );

	/* pobierz informacje o wielobajtowym ciągu znaków dla sprawdzenia */
	ms_array_clear( &data->Array );
	ercode = ms_string_info_mbs( PANGRAM2, p2s, &data->Array );
	mst_assert( ercode == MSEC_OK );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 5; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM1[x] );
	for( x = 5, y = 0; y < data->Array.Length; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM2[ms_array_getl(data->Array, MS_MBINFO, y).Offset] );
	for( x = p2s + 5, y = 5; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );
	for( x = p2s + p1s, y = 0; y < 6; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM2[ms_array_getl(data->Array, MS_MBINFO, y).Offset] );

	/* przypadek 3 - wstawiany ciąg znaków jest ciągiem o typie wchar_t */
	ms_string_clear( str );
	ercode = ms_string_push_cs( str, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj część na koniec */
	ercode = ms_string_push_wcs( str, PANGRAM4, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p1s + 5 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p1s + 5 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 5, PANGRAM4, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p1s + 5 + p4s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM4, p4s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p1s + 5 + p4s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 5; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM1[x] );
	for( x = 5, y = 0; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == (char)PANGRAM4[y] );
	for( x = p4s + 5, y = 5; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );
	for( x = p1s + p4s, y = 0; y < 5; ++x, ++y )
		mst_assert( str->Data.Char[x] == (char)PANGRAM4[y] );

	return MST_SUCCESS;
}

char *mst_string_mbs_insert_string( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s  = sizeof PANGRAM1 - 1;
	size_t p2sm = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p2s  = sizeof PANGRAM2 - 1;
	size_t p3s  = sizeof PANGRAM3 - 1;
	size_t p3sm = ms_string_length_mbs( PANGRAM3, 0 );
	size_t p4sm = sizeof PANGRAM4 / sizeof(wchar_t) - 1;
	size_t p4s  = ms_string_length_wcstombs( PANGRAM4, 0 );
	size_t x, y, offset;

	mst_assert( info );
	data = info->Data;
	mst_assert( !data->Pointer2 );

	/* przypadek 1 - wstawiany ciąg znaków jest ciągiem wielobajtowym */
	str = data->Pointer2 = ms_string_alloc_mbs( PANGRAM2, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* teraz powinno dodać */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Capacity == (p2s+1) * 2 );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p2s + 8 );
	mst_assert( str->MBInfo->Length == p2sm + 6 );
	mst_assert( str->Capacity == (p2s+1) * 2 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 6, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p2s + p3s + 8 );
	mst_assert( str->MBInfo->Length == p2sm + 6 + p3sm );
	mst_assert( str->Capacity == (p2s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM3, p3s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p2s + p3s + 8 );
	mst_assert( str->MBInfo->Length == p2sm + 6 + p3sm );
	mst_assert( str->Capacity == (p2s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 8; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM2[x] );
	for( x = 8, y = 0; y < p3s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM3[y] );
	for( x = p3s + 8, y = 8; y < p2s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM2[y] );
	for( x = p2s + p3s, y = 0; y < 8; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM2[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 6; ++x )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM2MB[x] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM2OF[x] );
	}
	for( offset = PANGRAM2OF[x], x = 6, y = 0; y < p3sm; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM3MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM3OF[y] + offset );
	}
	for( offset = PANGRAM3OF[y-1] + PANGRAM3MB[y-1], x = p3sm + 6, y = 6; y < p2sm; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM2MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM2OF[y] + offset );
	}
	for( offset += PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p3sm + p2sm, y = 0; y < 6; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM2MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM2OF[y] + offset );
	}

	/* przypadek 2 - wstawiany ciąg znaków jest ciągiem jednobajtowym */
	ms_string_clear( str );
	ercode = ms_string_push_mbs( str, PANGRAM3, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj część na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p3s + 5 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p3s + 5 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 6, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p3s + 5 + p1s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p3s + 5 + p1s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 7; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM3[x] );
	for( x = 7, y = 0; y < p1s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );
	for( x = p1s + 7, y = 7; y < p3s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM3[y] );
	for( x = p3s + p1s, y = 0; y < 5; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM1[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 6; ++x )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM3MB[x] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM3OF[x] );
	}
	for( offset = PANGRAM3OF[x], x = 6, y = 0; y < p1s; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == 1 );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == y + offset );
	}
	for( offset = y, x = p1s + 6, y = 6; y < p3sm; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM3MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM3OF[y] + offset );
	}
	for( offset += PANGRAM3OF[y-1] + PANGRAM3MB[y-1], x = p3sm + p1s, y = 0; y < 5; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == 1 );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == y + offset );
	}

	/* przypadek 3 - wstawiany ciąg znaków jest ciągiem o typie wchar_t */
	ms_string_clear( str );
	ercode = ms_string_push_mbs( str, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj część na koniec */
	ercode = ms_string_push_wcs( str, PANGRAM4, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p2s + 10 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p2s + 10 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 6, PANGRAM4, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p2s + 10 + p4s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM4, p4s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p2s + 10 + p4s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 8; ++x )
		mst_assert( str->Data.Char[x] == PANGRAM2[x] );
	for( x = 8, y = 0; y < p4s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM6[y] );
	for( x = p4s + 8, y = 8; y < p2s; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM2[y] );
	for( x = p2s + p4s, y = 0; y < 10; ++x, ++y )
		mst_assert( str->Data.Char[x] == PANGRAM6[y] );

	/* sprawdź MBInfo */
	for( offset = 0, x = 0; x < 6; ++x )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM2MB[x] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM2OF[x] );
	}
	for( offset = PANGRAM2OF[x], x = 6, y = 0; y < p4sm; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM4MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM4OF[y] + offset );
	}
	for( offset = PANGRAM4OF[y-1] + PANGRAM4MB[y-1], x = p4sm + 6, y = 6; y < p2sm; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM2MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM2OF[y] + offset );
	}
	for( offset += PANGRAM2OF[y-1] + PANGRAM2MB[y-1], x = p2sm + p4sm, y = 0; y < 5; ++x, ++y )
	{
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Bytes  == PANGRAM4MB[y] );
		mst_assert( ms_array_get(str->MBInfo, MS_MBINFO, x).Offset == PANGRAM4OF[y] + offset );
	}

	return MST_SUCCESS;
}

char *mst_string_wcs_insert_string( MST_TESTFUNC *info )
{
	MSTST_STRING_DATA *data;
	int                ercode;
	MS_STRING         *str;

	size_t p1s = sizeof PANGRAM1 - 1;
	size_t p2s = ms_string_length_mbs( PANGRAM2, 0 );
	size_t p4s = sizeof PANGRAM4 / sizeof(wchar_t) - 1;
	size_t p5s = sizeof PANGRAM5 / sizeof(wchar_t) - 1;
	size_t x, y;

	mst_assert( info );
	data = info->Data;
	mst_assert( !data->Pointer3 );

	/* przypadek 1 - wstawiany ciąg znaków jest ciągiem znaków wchar_t */
	str = data->Pointer3 = ms_string_alloc_wcs( PANGRAM5, 0 );
	mst_assert( str );
	mst_assert( str->Data.Char );

	/* dodaj część na koniec */
	ercode = ms_string_push_wcs( str, PANGRAM4, 5 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p5s + 5 );
	mst_assert( str->Capacity == (p5s+1) * 2 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_wcs( str, 4, L"", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p5s + 5 );
	mst_assert( str->Capacity == p5s * 2 + 2 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_wcs( str, 3, PANGRAM4, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p5s + 5 + p4s );
	mst_assert( str->Capacity == (p5s+1) * 4 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_wcs( str, 512, PANGRAM4, p4s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p5s + 5 + p4s );
	mst_assert( str->Capacity == (p5s+1) * 4 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 3; ++x )
		mst_assert( str->Data.Wide[x] == PANGRAM5[x] );
	for( x = 3, y = 0; y < p4s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM4[y] );
	for( x = p4s + 3, y = 3; y < p5s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM5[y] );
	for( x = p4s + p5s, y = 0; y < 5; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM4[y] );

	/* przypadek 2 - wstawiany ciąg znaków jest ciągiem jednobajtowym */
	ms_string_clear( str );
	ercode = ms_string_push_wcs( str, PANGRAM5, 0 );
	mst_assert( ercode == MSEC_OK );

	/* dodaj część na koniec */
	ercode = ms_string_push_cs( str, PANGRAM1, 4 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p5s + 4 );

	/* próbuj wstawić coś pustego */
	ercode = ms_string_insert_cs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p5s + 4 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_cs( str, 3, PANGRAM1, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p5s + 4 + p1s );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_cs( str, 512, PANGRAM1, p1s );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p5s + 4 + p1s );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 3; ++x )
		mst_assert( str->Data.Wide[x] == PANGRAM5[x] );
	for( x = 3, y = 0; y < p1s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == (wchar_t)PANGRAM1[y] );
	for( x = p1s + 3, y = 3; y < p5s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM5[y] );
	for( x = p1s + p5s, y = 0; y < 4; ++x, ++y )
		mst_assert( str->Data.Wide[x] == (wchar_t)PANGRAM1[y] );

	/* przypadek 3 - wstawiany ciąg znaków jest ciągiem wielobajtowym */
	ms_string_clear( str );
	ercode = ms_string_push_wcs( str, PANGRAM5, 0 );
	mst_assert( ercode == MSEC_OK );

	/* powinien być błąd - ostatni znak nie jest pełny */
	ercode = ms_string_push_mbs( str, PANGRAM2, 5 );
	mst_assert( ercode == MSEC_INVALID_VALUE );

	/* teraz powinno dodać */
	ercode = ms_string_push_mbs( str, PANGRAM2, 8 );
	mst_assert( ercode == MSEC_OK );

	/* próbuj wstawić coś pustego
	   przy sprawdzaniu jest + 6, gdyż wstawiono 6 znaków znajdujących się na 8 bajtach */
	ercode = ms_string_insert_mbs( str, 4, "", 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert( str->Length == p5s + 6 );

	/* dodaj gdzieś na początku, automatycznie obliczając długość ciągu */
	ercode = ms_string_insert_mbs( str, 3, PANGRAM2, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert( str->Length == p2s + p5s + 6 );

	/* próbuj wstawić poza zakres */
	ercode = ms_string_insert_mbs( str, 512, PANGRAM2, sizeof PANGRAM2 - 1 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert( str->Length == p2s + p5s + 6 );

	/* sprawdź poprawność danych końcowych */
	for( x = 0; x < 3; ++x )
		mst_assert( str->Data.Wide[x] == PANGRAM5[x] );
	for( x = 3, y = 0; y < p2s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM7[y] );
	for( x = p2s + 3, y = 3; y < p5s; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM5[y] );
	for( x = p2s + p5s, y = 0; y < 6; ++x, ++y )
		mst_assert( str->Data.Wide[x] == PANGRAM7[y] );

	return MST_SUCCESS;
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
#define FUNC_DESC_01 "Create string by alloc, init and return functions."
#define FUNC_DESC_02 "Destroy string, created by alloc, init and return."
#define FUNC_DESC_03 "Change string capacity by using simple realloc version."
#define FUNC_DESC_04 "Change string capacity by using minimum realloc version."
#define FUNC_DESC_05 "Check length of multibyte characters string."
#define FUNC_DESC_06 "Check structure about multibyte characters in string."
#define FUNC_DESC_07 "Detect structure about multibyte characters after wcstombs conversion."
#define FUNC_DESC_08 "Insert string array into single byte string at given position."
#define FUNC_DESC_09 "Insert string array into multi byte string at given position."
#define FUNC_DESC_10 "Insert string array into wide string at given position."
#define FUNC_DESC_11 "Test string hash for given data using SDBM algorithm."

/**
 * Lista funkcji testujących moduł.
 * Każda z osobna uruchamiana jest przez odpowiednią funkcję.
 * Dzięki zbiorowi funkcji zdefiniowanemu poniżej dzieje się to automatycznie.
 */
MST_TESTFUNC MSV_StringSuiteFunctions[] =
{
	{ MST_TFSTRINGIFY(mst_string_create),            FUNC_DESC_01, NULL },
	{ MST_TFSTRINGIFY(mst_string_destroy),           FUNC_DESC_02, NULL },
	{ MST_TFSTRINGIFY(mst_string_realloc),           FUNC_DESC_03, NULL },
	{ MST_TFSTRINGIFY(mst_string_realloc_min),       FUNC_DESC_04, NULL },
	{ MST_TFSTRINGIFY(mst_string_length),            FUNC_DESC_05, NULL },
	{ MST_TFSTRINGIFY(mst_string_info_mbs),          FUNC_DESC_06, NULL },
	{ MST_TFSTRINGIFY(mst_string_info_wcstombs),     FUNC_DESC_07, NULL },
	{ MST_TFSTRINGIFY(mst_string_cs_insert_string),  FUNC_DESC_08, NULL },
	{ MST_TFSTRINGIFY(mst_string_mbs_insert_string), FUNC_DESC_09, NULL },
	{ MST_TFSTRINGIFY(mst_string_wcs_insert_string), FUNC_DESC_10, NULL },
	// { MST_TFSTRINGIFY(mst_string_hash),              FUNC_DESC_11, NULL },
	{ MST_TFLASTRECORD }
};

/**
 * Zbiór funkcji testujących moduł.
 * Przekazywany do funkcji main, pozwala na uruchomienie wszystkich testów.
 */
MST_TESTSUITE MSV_StringSuite =
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
