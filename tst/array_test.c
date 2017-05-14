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
 *  Test file for "Dynamic Array" module.
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
#include <moss/array.h>
#include <mstest.h>

/**
 * Struktura zawierająca dane używane w funkcjach testujących.
 * Dane przekazywane są z funkcji do funkcji.
 * Dzięki temu po wystąpieniu błędu i przerwaniu funkcji, pamięć może zostać w prosty sposób zwolniona.
 * To właśnie głównie dzięki tej tablicy nie można zmieniać kolejności wywoływania testów.
 * Zapobiega wyciekom pamięci.
 */
typedef struct MSSTST_ARRAY_DATA
{
	MS_ARRAY *Pointer1;   /* wskaźnik do tablicy alokowanej w pamięci */
	MS_ARRAY *Pointer2;   /* wskaźnik do drugiej tablicy alokowanej w pamięci */
	MS_ARRAY  Local1;     /* wskaźnik do tablicy lokalnej */
	MS_ARRAY  Local2;     /* wskaźnik do drugiej tablicy lokalnej */
}
MSTST_ARRAY_DATA;

/* Lista przykładowych danych do testowania */
const int LIST[] =
{
	313, 392, 403, 283, 413, 431, 463, 450, 378, 506, 384, 477, 289, 376, 346, 296, 311, 465, 436, 434, 312,
	299, 371, 482, 402, 325, 263, 448, 491, 381, 308, 512, 342, 280, 290, 417, 305, 359, 336, 498, 363, 326,
	368, 508, 279, 453, 318, 406, 271, 319, 293, 354, 374, 323, 459, 454, 507, 467, 440, 292, 470, 306, 409,
	270, 488, 393, 341, 257, 358, 416, 284, 343, 473, 495, 266, 464, 383, 460, 361, 380, 432, 304, 439, 369,
	339, 273, 348, 286, 404, 499, 272, 394, 423, 476, 382, 487, 469, 309, 366, 489, 405, 501, 267, 377, 301,
	397, 462, 412, 461, 347, 320, 474, 269, 294, 276, 445, 370, 388, 492, 430, 437, 481, 315, 387, 365, 256,
	421, 446, 410, 332, 471, 395, 265, 324, 504, 355, 295, 303, 335, 468, 351, 505, 420, 386, 458, 372, 281,
	285, 373, 344, 400, 274, 379, 398, 298, 364, 277, 327, 411, 475, 331, 362, 494, 316, 428, 288, 291, 422,
	391, 457, 287, 300, 338, 390, 389, 350, 456, 407, 356, 375, 441, 401, 333, 275, 502, 509, 511, 349, 337,
	490, 424, 414, 435, 282, 451, 496, 353, 260, 317, 314, 352, 329, 418, 302, 340, 345, 442, 447, 321, 486,
	357, 478, 503, 500, 307, 310, 367, 297, 278, 264, 268, 425, 360, 429, 443, 419, 322, 426, 497, 484, 427,
	466, 433, 334, 479, 483, 261, 455, 510, 444, 262, 415, 449, 452, 472, 328, 493, 396, 399, 259, 438, 330,
	385, 480, 258, 485
};

/**
 * Funkcja wywoływana przy kończeniu testu.
 * Zwalnia miejsce przydzielone w pamięci podczas działania programu.
 */
void mst_array_teardown( MST_SUITE *suite )
{
	MSTST_ARRAY_DATA *data;

	if( !suite->Data )
		return;

	data = suite->Data;

	ms_array_free( data->Pointer1 );
	data->Pointer1 = NULL;

	ms_array_free( data->Pointer2 );
	data->Pointer2 = NULL;

	ms_array_free( &data->Local1 );
	ms_array_free( &data->Local2 );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	TWORZENIE I NISZCZENIE TABLICY
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Test tworzący tablicę na trzy tradycyjne sposoby.
 * Pierwszy sposób to zwracanie wskaźnika na funkcję alokowaną do pamięci.
 * Drugi sposób to bezpośrednie zwracanie tablicy lokalnej.
 * Trzeci sposób to zwykła inicjalizacja, zwracająca kod błędu.
 */
int mst_array_create( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	int               ercode = 0;

	mst_prepare( info );
	data = info->Data;

	/* tworzenie dwoma sposobami */
	data->Pointer1 = ms_array_alloc( sizeof(long double), 2 );
	data->Local1   = ms_array_return_local( sizeof(long double), 2 );

	/* inicjalizacja */
	ercode = ms_array_init( &data->Local2, sizeof(long double), 2 );
	mst_assert( ercode == MSEC_OK );

	/* tworzenie z domyślnym rozmiarem tablicy
	   i tak wszystko przelatuje przez ms_array_init, więc wystarczy sprawdzić
	   wartość tylko w jednej funkcji */
	data->Pointer2 = ms_array_alloc( sizeof(long double), 0 );

	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Items );
	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer2->Items );
	mst_assert( data->Local1.Items );
	mst_assert( data->Local2.Items );
	
	/* tylko tablice utworzone w pamięci mogą być usuwane w całości */
	mst_assert(  data->Pointer1->Destroy );
	mst_assert(  data->Pointer2->Destroy );
	mst_assert( !data->Local1.Destroy );
	mst_assert( !data->Local2.Destroy );

	/* taki rozmiar został ustawiony przy tworzeniu */
	mst_assert_uint( data->Pointer1->Capacity, ==, 2 );
	mst_assert_uint( data->Pointer2->Capacity, ==, MSD_ARRAY_DEFAULT_SIZE );
	mst_assert_uint( data->Local1.Capacity,    ==, 2 );
	mst_assert_uint( data->Local2.Capacity,    ==, 2 );

	/* długość jest 0, gdyż nie ma jeszcze żadnego elementu */
	mst_assert_uint( data->Pointer1->Length, ==, 0 );
	mst_assert_uint( data->Pointer2->Length, ==, 0 );
	mst_assert_uint( data->Local1.Length,    ==, 0 );
	mst_assert_uint( data->Local2.Length,    ==, 0 );

	/* rozmiar elementu został podany przy tworzeniu, ma być taki sam */
	mst_assert_uint( data->Pointer1->ItemSize, ==, sizeof(long double) );
	mst_assert_uint( data->Pointer2->ItemSize, ==, sizeof(long double) );
	mst_assert_uint( data->Local1.ItemSize,    ==, sizeof(long double) );
	mst_assert_uint( data->Local2.ItemSize,    ==, sizeof(long double) );

	/* początkowa funkcja odpowiadająca za zwiększanie */
	mst_assert( data->Pointer1->FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Pointer2->FuncIncrease == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Local1.FuncIncrease    == MSC_ArrayFunctions.IncMultiply );
	mst_assert( data->Local2.FuncIncrease    == MSC_ArrayFunctions.IncMultiply );

	/* początkowy modyfikator */
	mst_assert_float( data->Pointer1->Modifier, ==, 2.f );
	mst_assert_float( data->Pointer2->Modifier, ==, 2.f );
	mst_assert_float( data->Local1.Modifier,    ==, 2.f );
	mst_assert_float( data->Local2.Modifier,    ==, 2.f );

	return MSEC_OK;
}

/**
 * Niszczenie danych tablicy na dwa sposoby.
 * Pierwszym jest zwalnianie wszystkich zasobów które przydzieliła funkcja (ms_array_free).
 * Drugi sposób to usuwanie elementów z tablicy bez ingerencji w zasoby przydzielone dla tablicy.
 */
int mst_array_destroy( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	mst_prepare( info );
	data = info->Data;

	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer1->Items );
	mst_assert( data->Local2.Items );
	mst_assert( data->Local1.Items );

	/* tutaj widać różnicę pomiędzy clear a free */
	ms_array_clear( &data->Local1 );
	mst_assert( data->Local1.Items );
	mst_assert_uint( data->Local1.Length, ==, 0 );

	ms_array_free( &data->Local1 );
	mst_assert( !data->Local1.Items );

	/* zwolnij pamięć - funkcja zwalnia wszystko */
	ms_array_free( data->Pointer1 );
	ms_array_free( data->Pointer2 );
	/* nie można sprawdzić danych po usunięciu struktury */

	ms_array_free( &data->Local2 );
	mst_assert( !data->Local2.Items );
	mst_assert( !data->Local2.Destroy );

	mst_assert_uint( data->Local2.Capacity, ==, 0 );
	mst_assert_uint( data->Local2.Length,   ==, 0 );
	mst_assert_uint( data->Local2.ItemSize, ==, 0 );

	data->Pointer1 = NULL;
	data->Pointer2 = NULL;

	/* próbuj zwolnić coś, co nie prowadzi do niczego */
	ms_array_free( data->Pointer1 );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Standardowa zmiana pojemności tablicy.
 * Używana głównie w funkcjach, które dodają do tablicy po jednym elemencie.
 * Wykorzystuje odpowiednią funkcję do obliczania nowej pojemności, ustawianą bezpośrednio w strukturze.
 */
int mst_array_realloc( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	int               ercode;
	MS_ARRAY         *array;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Local1.Items );

	ercode = ms_array_init( &data->Local1, sizeof(int), 2 );
	array  = &data->Local1;
	mst_assert( ercode == MSEC_OK );

	array->FuncIncrease = MSC_ArrayFunctions.IncPower;
	array->Modifier     = 1.1f;

	/* 2^1.1 = ~2.14, co daje po zaokrągleniu 2
	 * w takim przypadku do wartości dodawane powinno być 1 */
	ercode = ms_array_realloc( array, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 3 );

	/* 3^2.5 = ~16.83 co daje po zaokrągleniu 16 - zaokrąglanie następuje w dół */
	array->Modifier = 2.57f;
	ercode = ms_array_realloc( array, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 16 );

	array->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
	array->Modifier     = 2.f;

	/* 16*2 = 32 */
	ercode = ms_array_realloc( array, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 32 );

	/* 32+2 = 34 */
	array->FuncIncrease = MSC_ArrayFunctions.IncAdd;
	ercode = ms_array_realloc( array, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 34 );

	/* zmiana pojemności tablicy do podanej wartości, w tym przypadku powinno być 15 a nie 36 (34+2) */
	ercode = ms_array_realloc( array, 15 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 15 );

	/* symulacja 7 elementów i zmiana pojemności tablicy do podanej wartości
	   powinien wystąpić błąd, ponieważ chcemy za bardzo zmniejszyć pamięć */
	array->Length = 8;
	ercode = ms_array_realloc( array, 7 );
	mst_assert( ercode == MSEC_DATA_OVERFLOW );
	mst_assert_uint( array->Capacity, ==, 15 );

	/* no ale do 8 powinno się zminejszyć - najmniejsza możliwa aktualnie pojemność */
	ercode = ms_array_realloc( array, 8 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 8 );

	/* dokładne zwiększanie, w tym przypadku spodziewamy się błędu
	   ta technika nie działa na zwykłym realloc, lecz na min_realloc */
	array->FuncIncrease = NULL;
	ercode = ms_array_realloc( array, 0 );
	mst_assert( ercode == MSEC_INVALID_VALUE );
	mst_assert_uint( array->Capacity, ==, 8 );

	/* zmniejsz do 2 */
	array->Length = 0;
	ercode = ms_array_realloc( array, 2 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 2 );

	return MSEC_OK;
}

/**
 * Zmiana pojemności tablicy do co najmniej podanej wartości minimalnej.
 * Ta technika używana jest w przypadku wstawiania kilku elementów do tablicy.
 * Pojemność obliczana jest w taki sam sposób jak w przypadku zwykłej ms_array_realloc, jednak
 * operacja ta powtarzana jest dopóty, dopóki nie zostanie osiągnięty satysfakcjonujący wynik.
 */
int mst_array_realloc_min( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	int               ercode;
	MS_ARRAY         *array;

	mst_prepare( info );
	data = info->Data;

	mst_assert( data->Local1.Items );
	array = &data->Local1;

	/* 2^1.1 = 2.14 ~= 2 (+1 -> gdy lewa jest równa prawej) = 3 */
	array->FuncIncrease = MSC_ArrayFunctions.IncPower;
	array->Modifier     = 1.1f;

	ercode = ms_array_realloc_min( array, 3 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 3 );

	/* 3^1.5 ~= 5, 5^1.5 ~= 11, 11^1.5 ~= 36 -> STOP, wartość minimalna (20) została osiągnięta */
	array->Modifier = 1.5f;
	ercode = ms_array_realloc_min( array, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 36 );

	/* w tym przypadku wartość nie powinna się zmienić, wystąpiło żądanie mniejszej wartości niż jest
	 * tutaj 0 nie oznacza wartości automatycznej, podawana jest wartość minimalna */
	ercode = ms_array_realloc_min( array, 20 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 36 );

	/* dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna */
	array->FuncIncrease = NULL;
	ercode = ms_array_realloc_min( array, 256 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* zmniejsz ilość elementów do 2 */
	ercode = ms_array_realloc( array, 2 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Capacity, ==, 2 );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DODAWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Test funkcji wstawiającej element do tablicy.
 * Wstawia cztery elementy z listy w różne miejsca i porównuje je z oryginałami.
 * Funkcja przyjmuje jako parametr wskaźnik na zmienną, która ma być wstawiona.
 */
int mst_array_insert_value( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	int               ercode, number;
	MS_ARRAY         *array;

	mst_prepare( info );
	data = info->Data;

	mst_assert( data->Local1.Items );
	array = &data->Local1;

	/* powiększanie dokładne - tyle ile potrzeba w aktualnym momencie */
	array->FuncIncrease = NULL;

	/* dodaj pierwszy element */
	ercode = ms_array_push_value( array, &LIST[0] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 1 );
	mst_assert_uint( array->Capacity, ==, 2 );

	/* dodaj drugi element */
	ercode = ms_array_push_value( array, &LIST[1] ); 
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 2 );
	mst_assert_uint( array->Capacity, ==, 2 );

	/* dodaj trzeci - tutaj powinno nastąpić zwiększenie pojemności tablicy */
	ercode = ms_array_push_value( array, &LIST[2] ); 
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 3 );
	mst_assert_uint( array->Capacity, ==, 3 );

	/* próbuj dodać czwarty poza zakres */
	ercode = ms_array_insert_value( array, 6, &LIST[3] );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array->Length,   ==, 3 );
	mst_assert_uint( array->Capacity, ==, 3 );

	/* dodaj czwarty zaraz po pierwszym indeksie */
	ercode = ms_array_insert_value( array, 1, &LIST[3] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 4 );
	mst_assert_uint( array->Capacity, ==, 4 );

	/* sprawdź czy wartości pobrane będą równe zapisanym
	 * tutaj przetestuj pobieranie z lokalnej tablicy - getl - więc data->Local1 a nie array */
	number = ms_array_getl( data->Local1, int, 0 );
	mst_assert_sint( number, ==, LIST[0] );
	number = ms_array_getl( data->Local1, int, 1 );
	mst_assert_sint( number, ==, LIST[3] );
	number = ms_array_getl( data->Local1, int, 2 );
	mst_assert_sint( number, ==, LIST[1] );
	number = ms_array_getl( data->Local1, int, 3 );
	mst_assert_sint( number, ==, LIST[2] );

	return MSEC_OK;
}

/**
 * Test funkcji wstawiającej elementy do tablicy.s
 * Wstawia całą listę elementów i porównuje je z oryginałami.
 * Lista przekazywana do funkcji musi być w formacie standardowej tablicy języka C.
 * Wstawiane wartości można ograniczać poprzez uzupełnienie odpowiednich argumentów funkcji.
 */
int mst_array_insert_values( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	MS_ARRAY         *array;

	size_t x, y;
	int    ercode, *items;

	mst_prepare( info );
	data = info->Data;
	mst_assert( !data->Local2.Items );

	data->Local2 = ms_array_return_local( sizeof(int), 2 );
	mst_assert( data->Local2.Items );
	array = &data->Local2;

	/* dodaj pierwszą część
	 * 2 * 2 = 4 * 2 = 8 * 2 = 16 * 2 = 32 * 2 = 64 * 2 = 128 * 2 = 256 */
	ercode = ms_array_push_values( array, LIST, 192 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 192 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* dodaj drugą część */
	ercode = ms_array_insert_values( array, 64, &LIST[192], 32 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 224 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* próbuj wstawić bez rozmiaru */
	ercode = ms_array_push_values( array, LIST, 0 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array->Length,   ==, 224 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* dodaj trzecią część */
	ercode = ms_array_insert_values( array, 0, &LIST[224], 32 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 256 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* próbuj wstawić poza zakres */
	ercode = ms_array_insert_values( array, 512, LIST, 32 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array->Length,   ==, 256 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* sprawdź poprawność danych */
	items = (int*)array->Items;
	for( y = 224, x = 0; x < 32; ++y, ++x )
		mst_assert_sint( items[x], ==, LIST[y] );
	for( y = 0; y < 64; ++x, ++y )
		mst_assert_sint( items[x], ==, LIST[y] );
	for( y = 192; y < 224; ++x, ++y )
		mst_assert_sint( items[x], ==, LIST[y] );
	for( y = 64; y < 192; ++x, ++y )
		mst_assert_sint( items[x], ==, LIST[y] );

	return MSEC_OK;
}

/**
 * Test wstawiania tablicy do tablicy.
 * Ta wersja funkcji pozwala na wstawienie konkretnego odciętego kawałka tablicy do innej tablicy.
 * W przypadku podania zer jako zakresu, kopiowana jest cała tablica.
 * Warto zaznaczyć, że wszystkie dane wstawiane są na sam koniec tablicy.
 */
int mst_array_join_slice( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	size_t    x, y;
	int       ercode, *items1, *items2;
	MS_ARRAY *array1, *array2, *array3;

	mst_prepare( info );
	data = info->Data;

	mst_assert(  data->Local2.Items );
	mst_assert( !data->Pointer1 );
	mst_assert( !data->Pointer2 );
	array1 = &data->Local2;

	array2 = data->Pointer1 = ms_array_alloc( array1->ItemSize, 64 );
	mst_assert( array2->Items );

	array3 = data->Pointer2 = ms_array_alloc( array1->ItemSize * 2, 64 );
	mst_assert( array3->Items );

	/* dodaj wartości z podanego zakresu - idealnie do 64 elementów
	 * wielkość tablicy nie powinna się zmienic */
	ercode = ms_array_join_slice( array2, array1, 64, 64 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array2->Length,   ==, 64 );
	mst_assert_uint( array2->Capacity, ==, 64 );

	/* dodaj wartości z podanego zakresu - kopiuj do końca */
	ercode = ms_array_join_slice( array2, array1, 192, 0 );
	mst_assert( ercode == MSEC_OK );

	mst_assert_uint( array2->Length,   ==, 128 );
	mst_assert_uint( array2->Capacity, ==, 128 );

	/* błąd - indeks poza zakresem */
	ercode = ms_array_join_slice( array2, array1, 192, 256 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array2->Length,   ==, 128 );
	mst_assert_uint( array2->Capacity, ==, 128 );

	/* próbuj skopiować wartości do tablicy z innymi rozmiarami elementów */
	ercode = ms_array_join( array3, array2 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array3->Length,   ==, 0 );
	mst_assert_uint( array3->Capacity, ==, 64 );

	/* sprawdź czy w tablicy znajdują się poprawne elementy */
	items2 = (int*)array2->Items;
	items1 = (int*)array1->Items;
	for( x = 0, y = 64; y < 128; ++x, ++y )
		mst_assert_sint( items2[x], ==, items1[y] );
	for( y = 192; y < 256; ++x, ++y )
		mst_assert_sint( items2[x], ==, items1[y] );

	return MSEC_OK;
}

/**
 * Drugi test wstawiania tablicy do tablicy.
 * Ta wersja funkcji pozwala na wstawienie do tablicy elementów z innej tablicy spoza podanego zakresu.
 * W przypadku 256 elementów i ustawieniu zakresu 128 elementów od 64 indeksu, skopiowane zostaną wartości 
 * od 0-64 oraz od 192 do 256.
 * Zakres oznaczany jest przez indeks początkowy i ilość elementów do skopiowania.
 */
int mst_array_join_slice_inverse( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	size_t    x, y;
	int       ercode, *items1, *items2;
	MS_ARRAY *array1, *array2, *array3;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local2.Items );
	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer2 );
	array1 = &data->Local2;

	/* wyczyść poprzednio uzupełnianą tablicę
	 * przy okazji sprawdź, czy czyszczenie działa w porządku */
	array2 = data->Pointer1;
	ms_array_clear( array2 );
	mst_assert( array2->Items );
	mst_assert( array2->Destroy );

	mst_assert_uint( array2->ItemSize, ==, sizeof(int) );
	mst_assert_uint( array2->Length,   ==, 0 );
	mst_assert_uint( array2->Capacity, ==, 128 );

	array3 = data->Pointer2;
	mst_assert_uint( array3->ItemSize, ==, sizeof(int) * 2 );

	/* dodaj wartości z innej tablicy z podanego zakresu (0:64, 192:256) */
	ercode = ms_array_join_slice_inverse( array2, array1, 64, 128 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array2->Length,   ==, 128 );
	mst_assert_uint( array2->Capacity, ==, 128 );

	/* próbuj dodać wartości spoza zakresu (0-64, 320-OVERFLOW) */
	ercode = ms_array_join_slice_inverse( array2, array1, 64, 256 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array2->Length,   ==, 128 );
	mst_assert_uint( array2->Capacity, ==, 128 );

	/* zakres od wartości do końca (0:128, 256:256) */
	ercode = ms_array_join_slice_inverse( array2, array1, 128, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array2->Length,   ==, 256 );
	mst_assert_uint( array2->Capacity, ==, 256 );

	/* próbuj skopiować wartości do tablicy z innymi rozmiarami elementów */
	ercode = ms_array_join_slice_inverse( array3, array2, 128, 64 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array3->Length,   ==, 0 );
	mst_assert_uint( array3->Capacity, ==, 64 );

	/* sprawdź czy w tablicy znajdują się poprawne elementy */
	items1 = (int*)array1->Items;
	items2 = (int*)array2->Items;
	for( x = 0, y = 0; y < 64; ++x, ++y )
		mst_assert_sint( items2[x], ==, items1[y] );
	for( y = 192; y < 256; ++x, ++y )
		mst_assert_sint( items2[x], ==, items1[y] );
	for( y = 0; y < 128; ++x, ++y )
		mst_assert_sint( items2[x], ==, items1[y] );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	KOPIOWANIE TABLICY
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Kopiowanie tablicy dwoma sposobami.
 * Pierwszy sposób to kopiowanie do istniejącej już w pamięci tablicy (ms_array_copy).
 * Drugi sposób to kopiowanie do nowej tablicy i zwrócenie wskazania na nią (ms_array_copy_alloc).
 */
int mst_array_copy( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	size_t    iter;
	int       ercode;
	MS_ARRAY *array1, *array2, *array3;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local2.Items );
	mst_assert( data->Local1.Items );
	mst_assert( data->Pointer2 );

	array1 = &data->Local2;
	array2 = &data->Local1;

	/* wyczyść tablice */
	ms_array_free( array2 );
	mst_assert( !array2->Items );

	/* kopiuj wartości */
	ercode = ms_array_copy( array2, array1 );
	mst_assert( ercode == MSEC_OK );

	/* obie tablice są lokalne, więc o polu Destroy ustawionym na false nie ma mowy */
	mst_assert_uint( array1->Capacity, ==, array2->Capacity );
	mst_assert_uint( array1->Length,   ==, array2->Length );

	mst_assert( array1->Destroy == array2->Destroy );
	mst_assert( array1->Items != array2->Items );

	/* sprawdź czy elementy są takie same */
	for( iter = 0; iter < array1->Length; ++iter )
		mst_assert_uint( ms_array_get(array1, int, iter), ==, ms_array_get(array2, int, iter) );

	/* tą tablicę należy usunąć gdyż jest to wskaźnik - inaczej mogą być wycieki pamięci */
	ms_array_free( data->Pointer2 );

	/* a teraz kopiuj zwracając wskaźnik do nowej tablicy */
	array3 = data->Pointer2 = ms_array_copy_alloc( array1 );

	/* przyrównaj do drugiej tablicy, wartości muszą być te same */
	mst_assert( array3 );
	mst_assert_uint( array2->Capacity, ==, array3->Capacity );
	mst_assert_uint( array2->Length,   ==, array3->Length );

	/* oprócz destroy, pamięć przydzielona na strukturę musi być zawsze niszczona
	 * więc w tym przypadku array3 musi mieć pole Destroy ustawione zawsze na true */
	mst_assert( array2->Destroy != array3->Destroy );
	mst_assert( array2->Items != array3->Items );

	/* sprawdź czy elementy są takie same */
	for( iter = 0; iter < array2->Length; ++iter )
		mst_assert_sint( ms_array_get(array2, int, iter), ==, ms_array_get(array3, int, iter) );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	USUWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Test usuwania elementów tablicy znajdujących się poza podanym zakresem.
 * Zmiany wykonane przez funkcje są trwałe, kopia tablicy nie jest tworzona.
 * Aby zrobić kopię z podanych wartości należy użyć ms_array_join_slice na nowej tablicy.
 */
int mst_array_slice( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	size_t    x, y;
	int       ercode;
	MS_ARRAY *array;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer2->Items );
	mst_assert( data->Local2.Items );
	array = data->Pointer2;

	/* pozostaw elementy od 32 do 160 -> 128 elementów po indeksie 32 */
	ercode = ms_array_slice( array, 32, 128 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 128 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* sprawdź czy wartości się zgadzają */
	for( x = 0, y = 32; x < 128; ++y, ++x )
		mst_assert_sint( ms_array_get(array, int, x), ==, ms_array_getl(data->Local2, int, y) );

	/* obcinanie poza zakres (32:160, elementów jest 128) */
	ercode = ms_array_slice( array, 32, 128 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array->Length,   ==, 128 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* obcinanie od indeksu do końca */
	ercode = ms_array_slice( array, 64, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 64 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* sprawdź czy wartości się zgadzają po przesunięciu */
	for( x = 0, y = 96; x < 64; ++y, ++x )
		mst_assert_sint( ms_array_get(array, int, x), ==, ms_array_getl(data->Local2, int, y) );

	/* obcinanie od indeksu od początku - bez przesuwania elementów w pamięci */
	ercode = ms_array_slice( array, 0, 32 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 32 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* sprawdź czy wartości się zgadzają po usunięciu ogona od podanego indeksu */
	for( x = 0, y = 96; x < 32; ++y, ++x )
		mst_assert_sint( ms_array_get(array, int, x), ==, ms_array_getl(data->Local2, int, y) );

	/* wyczyść tablicę nie zwalniając jednak pamięci przechowującej elementy */
	ms_array_clear( array );
	mst_assert_uint( array->Length, ==, 0 );
	mst_assert( array->Items );

	/* próbuj obciąć pustą tablicę */
	ercode = ms_array_slice( array, 32, 128 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array->Length,   ==, 0 );
	mst_assert_uint( array->Capacity, ==, 256 );

	return MSEC_OK;
}

/**
 * Test usuwania elementów tablicy z podanego zakresu.
 * Aliasem tej funkcji jest makro o nazwie ms_array_slice_inverse.
 * Zmiany wykonywane na tablicy są trwałe.
 */
int mst_array_remove_range( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	size_t    x, y;
	int       ercode;
	MS_ARRAY *array;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local1.Items );
	mst_assert( data->Local2.Items );
	array = &data->Local1;

	/* usuń elementy od 64 do 128 (64+64) */
	ercode = ms_array_remove_range( array, 64, 64 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 192 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* sprawdź czy wartości się zgadzają */
	for( x = 0, y = 0; x < 64; ++y, ++x )
		mst_assert_sint( ms_array_get(array, int, x), ==, ms_array_getl(data->Local2, int, y) );
	for( y = 128; y < 256; ++y, ++x )
		mst_assert_sint( ms_array_get(array, int, x), ==, ms_array_getl(data->Local2, int, y) );

	/* usuwaj od indeksu 128 do końca - usuwanie bez przesuwania */
	ercode = ms_array_remove_range( array, 128, 0 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 128 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* próbuj usunąć spoza zakresu */
	ercode = ms_array_remove_range( array, 128, 128 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array->Length,   ==, 128 );
	mst_assert_uint( array->Capacity, ==, 256 );

	/* wyczyść */
	ms_array_clear( array );
	mst_assert_uint( array->Length, ==, 0 );
	mst_assert( array->Items );

	/* próbuj teraz coś usunąć */
	ercode = ms_array_remove_range( array, 0, 5 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array->Length,   ==, 0 );
	mst_assert_uint( array->Capacity, ==, 256 );

	return MSEC_OK;
}

/**
 * Test usuwania elementu tablicy o podanym indeksie.
 * Istnieje alias do wywołania funkcji, który usuwa ostani element tablicy (ms_array_remove_last).
 * Funkcja nie zmniejsza pojemności tablicy, aby to zrobić należy wywołać funkcję ms_array_realloc.
 */
int mst_array_remove( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	int       ercode;
	MS_ARRAY *array1, *array2;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Pointer1 );
	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer1->Items );
	mst_assert( data->Pointer2->Items );
	mst_assert_uint( data->Pointer1->Capacity, ==, 256 );
	array1 = data->Pointer1;

	ms_array_free( data->Pointer2 );

	/* utwórz kopię tablicy aby można było porównać wyniki */
	data->Pointer2 = ms_array_copy_alloc( data->Pointer1 );
	mst_assert( data->Pointer2 );
	mst_assert( data->Pointer2->Items );
	array2 = data->Pointer2;

	/* sprawdź czy ten konkretny element jest taki sam - musi być, skoro tablica była kopiowana */
	mst_assert_sint( ms_array_get(array1, int, 64), ==, ms_array_get(array2, int, 64) );

	/* usuń ostatni element */
	ercode = ms_array_remove_last( array1 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array1->Length,   ==, 255 );
	mst_assert_uint( array1->Capacity, ==, 256 );

	/* nadal musi być taki sam, skoro usuwany był tylko ostatni element */
	mst_assert_sint( ms_array_get(array1, int, 64), ==, ms_array_get(array2, int, 64) );

	/* usuń z wybranej pozycji */
	ercode = ms_array_remove( array1, 64 );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array1->Length,   ==, 254 );
	mst_assert_uint( array1->Capacity, ==, 256 );

	/* sprawdź czy element został usunięty poprawnie */
	mst_assert_sint( ms_array_get(array1, int, 64), !=, ms_array_get(array2, int, 64) );
	mst_assert_sint( ms_array_get(array1, int, 64), ==, ms_array_get(array2, int, 65) );

	/* próbuj usunąć spoza zakresu */
	ercode = ms_array_remove( array1, 255 );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array1->Length,   ==, 254 );
	mst_assert_uint( array1->Capacity, ==, 256 );

	/* wyczyść tablicę */
	ms_array_clear( array1 );
	mst_assert( array1->Items );
	mst_assert_uint( array1->Length,   ==, 0 );
	mst_assert_uint( array1->Capacity, ==, 256 );

	/* próbuj usunąć z pustej tablicy */
	ercode = ms_array_remove( array1, 255 );
	mst_assert( ercode == MSEC_INVALID_ARGUMENT );
	mst_assert_uint( array1->Length,   ==, 0 );
	mst_assert_uint( array1->Capacity, ==, 256 );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	TESTY FUNKCJI BAZOWYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Test tworzenia tablicy o wybranym typie.
 * Tutaj typem jest typ standardowy - wskaźnik na typ void.
 * Jest to funkcja bazowa, co oznacza że istnie makro, pozwalające na rozszerzenie jej dla innych typów.
 */
int mst_array_base_return( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local1.Items );

	/* usuń dane */
	ms_array_free( &data->Local1 );
	mst_assert( !data->Local1.Items );

	data->Local1 = ms_array_return( 2 );
	mst_assert( data->Local1.Items );

	/* sprawdź podstawwe pola struktury */
	mst_assert( !data->Local1.Destroy );
	mst_assert_uint( data->Local1.Capacity, ==, 2 );
	mst_assert_uint( data->Local1.Length,   ==, 0 );
	mst_assert_uint( data->Local1.ItemSize, ==, (sizeof *data->Local1.Items) );

	return MSEC_OK;
}

/**
 * Test wstawiania elementu o wybranym typie do tablicy.
 * Wstawianie wielu elementów odbywa się normalnie, a więc funkcją ms_array_insert_values.
 * Tutaj typem jest typ standardowy - wskaźnik na typ void.
 * Jest to funkcja bazowa, co oznacza że istnie makro, pozwalające na rozszerzenie jej dla innych typów.
 */
int mst_array_base_insert( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	MS_ARRAY         *array;
	int               ercode;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local1.Items );
	array = &data->Local1;

	/* powiększanie dokładne - tyle ile potrzeba w aktualnym momencie */
	array->FuncIncrease = NULL;

	/* dodaj pierwszy element */
	ercode = ms_array_push( array, &LIST[0] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 1 );
	mst_assert_uint( array->Capacity, ==, 2 );

	/* dodaj drugi element */
	ercode = ms_array_insert( array, 0, &LIST[1] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 2 );
	mst_assert_uint( array->Capacity, ==, 2 );

	/* dodaj trzeci element - tutaj powinno nastąpić zwiększenie pojemności tablicy */
	ercode = ms_array_insert( array, 1, &LIST[2] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 3 );
	mst_assert_uint( array->Capacity, ==, 3 );

	/* dodaj czwarty element - tutaj również zwiększenie tablicy */
	ercode = ms_array_insert( array, 1, &LIST[3] );
	mst_assert( ercode == MSEC_OK );
	mst_assert_uint( array->Length,   ==, 4 );
	mst_assert_uint( array->Capacity, ==, 4 );

	/* dodaj piąty - powinien wystąpić bład, próba dodania elementu poza zakres */
	ercode = ms_array_insert( array, 6, &LIST[1] );
	mst_assert( ercode == MSEC_OUT_OF_RANGE );
	mst_assert_uint( array->Length,   ==, 4 );
	mst_assert_uint( array->Capacity, ==, 4 );

	/* sprawdź czy wartości pobrane będą równe zapisanym */
	mst_assert( array->Items[0] == &LIST[1] );
	mst_assert( array->Items[1] == &LIST[3] );
	mst_assert( array->Items[2] == &LIST[2] );
	mst_assert( array->Items[3] == &LIST[0] );

	return MSEC_OK;
}

/**
 * Test kopiowania tablicy o wybranym typie do tablicy lokalnej.
 * Kopiowanie do tablicy alokowanej w pamięci odbywa się normalnie, a więc funkcją ms_array_copy_alloc.
 * Tutaj typem jest typ standardowy - wskaźnik na typ void.
 * Jest to funkcja bazowa, co oznacza że istnie makro, pozwalające na rozszerzenie jej dla innych typów.
 */
int mst_array_base_copy_return( MST_FUNCTION *info )
{
	MSTST_ARRAY_DATA *data;
	MS_ARRAY         *array1, *array2;
	size_t            iter;

	mst_prepare( info );
	data = info->Data;
	mst_assert( data->Local1.Items );
	mst_assert( data->Local2.Items );
	array1 = &data->Local1;

	/* wyczyść tablicę ze starych śmieci */
	ms_array_free( &data->Local2 );
	mst_assert( !data->Local2.Items );

	data->Local2 = ms_array_copy_return( array1 );
	mst_assert( data->Local2.Items );
	array2 = &data->Local2;

	mst_assert_uint( array1->Capacity, ==, array2->Capacity );
	mst_assert_uint( array1->Length,   ==, array2->Length );

	/* w tym momencie destroy powinien być taki sam, jednak w przypadku kopiowania
	 * z wskaźnika powinien być inny */
	mst_assert( array1->Destroy == array2->Destroy );
	mst_assert( array1->Items != array2->Items );

	/* sprawdź czy elementy są takie same */
	for( iter = 0; iter < array1->Length; ++iter )
		mst_assert( ms_array_get(array1, int, iter) == ms_array_get(array2, int, iter) );

	mst_assert( data->Pointer1->Items );
	ms_array_free( data->Pointer1 );

	/* no to teraz kopiuj tablicę starą metodą */
	data->Pointer1 = ms_array_copy_alloc( array1 );

	/* wyczyść drugą tablicę */
	ms_array_free( array2 );
	mst_assert( !array2->Items );
	mst_assert_uint( array2->Length, ==, 0 );

	/* kopiuj nową metodą */
	data->Local2 = ms_array_copy_return( data->Pointer1 );
	mst_assert( data->Local2.Items );
	array2 = &data->Local2;

	/* i sprawdź to nieszczęsne Destroy */
	mst_assert( data->Pointer1->Destroy != array2->Destroy );

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
#define FUNC_DESC_01 "Create array by alloc, init and return functions."
#define FUNC_DESC_02 "Destroy array, created by alloc and init or return."
#define FUNC_DESC_03 "Change array capacity by using simple realloc version."
#define FUNC_DESC_04 "Change array capacity by using minimum realloc version."
#define FUNC_DESC_05 "Insert single value into array at given index."
#define FUNC_DESC_06 "Insert values from table into array at given index."
#define FUNC_DESC_07 "Join one table from given range to the end of another table."
#define FUNC_DESC_08 "Join one table from outside given range to the end of another table."
#define FUNC_DESC_09 "Copy data from one array to another new array."
#define FUNC_DESC_10 "Remove data from array that lie outside of given range."
#define FUNC_DESC_11 "Remove data from array that lie inside of given range."
#define FUNC_DESC_12 "Remove element from array placed in given index."
#define FUNC_DESC_13 "Create local array with specified type."
#define FUNC_DESC_14 "Insert single value of specified type into array at given index."
#define FUNC_DESC_15 "Copy data of specified type from one array to another."

/**
 * Lista funkcji testujących moduł.
 * Każda z osobna uruchamiana jest przez odpowiednią funkcję.
 * Dzięki zbiorowi funkcji zdefiniowanemu poniżej dzieje się to automatycznie.
 * Wszystkie funkcje są ze sobą powiązane.
 * 
 * ZMIANA KOLEJNOŚCI WYWOŁYWANIA FUNKCJI MOŻE ZMIENIĆ WYNIKI A NAWET WYWOŁAĆ
 * BŁĄD NARUSZENIA OCHRONY PAMIĘCI.
 */
MST_FUNCTION MSV_ArraySuiteFunctions[] =
{
	{ MST_STRINGIFY(mst_array_create),             FUNC_DESC_01, NULL },
	{ MST_STRINGIFY(mst_array_destroy),            FUNC_DESC_02, NULL },
	{ MST_STRINGIFY(mst_array_realloc),            FUNC_DESC_03, NULL },
	{ MST_STRINGIFY(mst_array_realloc_min),        FUNC_DESC_04, NULL },
	{ MST_STRINGIFY(mst_array_insert_value),       FUNC_DESC_05, NULL },
	{ MST_STRINGIFY(mst_array_insert_values),      FUNC_DESC_06, NULL },
	{ MST_STRINGIFY(mst_array_join_slice),         FUNC_DESC_07, NULL },
	{ MST_STRINGIFY(mst_array_join_slice_inverse), FUNC_DESC_08, NULL },
	{ MST_STRINGIFY(mst_array_copy),               FUNC_DESC_09, NULL },
	{ MST_STRINGIFY(mst_array_slice),              FUNC_DESC_10, NULL },
	{ MST_STRINGIFY(mst_array_remove_range),       FUNC_DESC_11, NULL },
	{ MST_STRINGIFY(mst_array_remove),             FUNC_DESC_12, NULL },
	{ MST_STRINGIFY(mst_array_base_return),        FUNC_DESC_13, NULL },
	{ MST_STRINGIFY(mst_array_base_insert),        FUNC_DESC_14, NULL },
	{ MST_STRINGIFY(mst_array_base_copy_return),   FUNC_DESC_15, NULL },
	{ MST_LASTRECORD }
};

/**
 * Zbiór funkcji testujących moduł.
 * Przekazywany do funkcji main, pozwala na uruchomienie wszystkich testów.
 */
MST_SUITE MSV_ArraySuite =
{
	">>> ARRAY MODULE",
	TRUE,
	NULL,
	mst_array_teardown,
	NULL,
	MSV_ArraySuiteFunctions
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
		MSTST_ARRAY_DATA data = { NULL, NULL };
		
		IGVAR argc;
		IGVAR argv;

		/* przypisz dane do zbioru */
		MSV_ArraySuite.Data = &data;

		return mst_run_suite( &MSV_ArraySuite );
	}

#endif
