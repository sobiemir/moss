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

#include <criterion/criterion.h>
#include "../inc/array.h"

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    FUNKCJE WYWOŁYWANE PO TESTACH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

ReportHook(POST_TEST)(struct criterion_test_stats *stats)
{
    printf( "===============================================================================\n" );
    printf( "[" TERMCOLORCYAN("TEST") "] %s\n", stats->test->name );
    printf( "[" TERMCOLORMAGNETA("DESC") "] %s\n", stats->test->data->description );

    if( stats->crashed )
        printf( "[" TERMCOLORRED("STAT") "] " TERMCOLORRED("Crash!") "\n" );
    else if( stats->test_status == CR_STATUS_FAILED )
        printf( "[" TERMCOLORRED("STAT") "] Asserts: [%d passed, %d failed, %d total]\n",
            stats->passed_asserts, stats->failed_asserts, stats->passed_asserts + stats->failed_asserts);
    else if( stats->test_status == CR_STATUS_PASSED )
        printf( "[" TERMCOLORGREEN("STAT") "] Asserts: [%d passed, %d failed, %d total]\n",
            stats->passed_asserts, stats->failed_asserts, stats->passed_asserts + stats->failed_asserts);

    printf( "[" TERMCOLORYELLOW("INFO") "] Time: %f, Exit Code: %d, Progress: %u\n",
        stats->elapsed_time, stats->exit_code, stats->progress );
}

ReportHook(POST_ALL)(struct criterion_global_stats *stats)
{
    printf( "===============================================================================\n" );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    DANE PRZYKŁADOWE I FUNKCJE POMOCNICZE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

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
 * Przykładowa tablica.
 *
 * @param size Ilość elementów do dodania, maksymalna wartość to 256.
 */
MS_ARRAY *mstst_create_sample_array( size_t size )
{
    int       ercode;
    MS_ARRAY *array;
    
    size  = size > 256 ? 256 : size;
    array = ms_array_alloc( sizeof(int), size );

    cr_assert_not_null( array );

    ercode = ms_array_push_values( array, LIST, size );
    cr_assert_eq( ercode, MSEC_OK );

    return array;
}

/**
 * Przykładowa tablica zawierająca wskaźniki.
 *
 * @param size Ilość elementów do dodania, maksymalna wartość to 256.
 */
MS_ARRAY *mstst_create_pointer_array( size_t size )
{
    int       ercode;
    size_t    iter;
    MS_ARRAY *array;
    
    size  = size > 256 ? 256 : size;
    array = ms_array_alloc( sizeof *array->Items, size );

    cr_assert_not_null( array );

    for( iter = 0; iter < size; ++iter )
    {
        ercode = ms_array_push( array, &LIST[iter] );
        cr_assert_eq( ercode, MSEC_OK );
    }
    return array;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    TWORZENIE TABLICY
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 01.
 * Tworzy nową tablicę, zwracając do niej wskaźnik.
 * Sprawdza czy tworzenie tablicy przebiega prawidłowo.
 */
Test( moss_test, array_alloc,
    .description = "Creates array and returns pointer to it." )
{
    MS_ARRAY *array = ms_array_alloc( sizeof(long double), 2 );

    // sprawdź podstawowe pola struktury
    cr_assert_not_null( array );
    cr_assert_not_null( array->Items );

    cr_assert( array->Destroy );

    cr_assert_eq( array->Capacity,     2 );
    cr_assert_eq( array->Length,       0 );
    cr_assert_eq( array->ItemSize,     sizeof(long double) );
    cr_assert_eq( array->FuncIncrease, MSC_ArrayFunctions.IncMultiply );
    cr_assert_eq( array->Modifier,     2.f );

    // zwolnij pamięć - funkcja zwalnia wszystko
    ms_array_free( array );

    // próbuj zwolnić coś, co nie prowadzi do niczego
    ms_array_free( NULL );
}

/**
 * 02.
 * Tworzy tablicę zwracając ją jako zmienną lokalną.
 * Sprawdza czy tworzenie tablicy przebiega prawidłowo.
 */
Test( moss_test, array_return_local,
    .description = "Creates local array and returns struct." )
{
    MS_ARRAY array = ms_array_return_local( sizeof(long double), 2 );

    cr_assert_not_null( array.Items );

    cr_assert( !array.Destroy );

    cr_assert_eq( array.Capacity,     2 );
    cr_assert_eq( array.Length,       0 );
    cr_assert_eq( array.ItemSize,     sizeof(long double) );
    cr_assert_eq( array.FuncIncrease, MSC_ArrayFunctions.IncMultiply );
    cr_assert_eq( array.Modifier,     2.f );

    // zwolnij pamięć
    ms_array_free( &array );

    // sprawdź czy pamięć została zwolniona
    cr_assert_null( array.Items );
    cr_assert_eq( array.Capacity, 0 );
    cr_assert_eq( array.Items, 0 );
    cr_assert_eq( array.ItemSize, 0 );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 03.
 * Zmienia pojemność tablicy do podanej wartości.
 * W przypadku 0 do obliczenia nowej pojemności używana jest przypisana do tablicy funkcja.
 */
Test( moss_test, array_realloc,
    .description = "Allocates new memory for array, to increase capacity." )
{
    int      ercode;
    MS_ARRAY array;

    ercode = ms_array_init( &array, sizeof(int), 2 );
    cr_assert_eq( ercode, MSEC_OK );

    array.FuncIncrease = MSC_ArrayFunctions.IncPower;
    array.Modifier     = 1.1f;

    // 2^1.1 = ~2.14, co daje po zaokrągleniu 2, w takim przypadku do wartości dodawane powinno być 1
    ercode = ms_array_realloc( &array, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 3 );

    // 3^2.5 = ~16.83 co daje po zaokrągleniu 16 - zaokrąglanie następuje w dół
    array.Modifier = 2.57f;
    ercode = ms_array_realloc( &array, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 16 );

    array.FuncIncrease = MSC_ArrayFunctions.IncMultiply;
    array.Modifier     = 2.f;

    // 16*2 = 32
    ercode = ms_array_realloc( &array, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 32 );

    // 32+2 = 34
    array.FuncIncrease = MSC_ArrayFunctions.IncAdd;
    ercode = ms_array_realloc( &array, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 34 );

    // zmiana pojemności tablicy do podanej wartości, w tym przypadku powinno być 15 a nie 36 (34+2)
    ercode = ms_array_realloc( &array, 15 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 15 );

    // symulacja 15 elementów i zmiana pojemności tablicy do podanej wartości
    // powinien wystąpić błąd, ponieważ chcemy za bardzo zmniejszyć pamięć
    array.Length = 15;
    ercode = ms_array_realloc( &array, 8 );
    cr_assert_eq( array.Capacity, 15 );
    cr_assert_eq( ercode, MSEC_DATA_OVERFLOW );

    // dokładne zwiększanie, w tym przypadku spodziewamy się błędu
    // ta technika nie działa na zwykłym realloc, lecz na min_realloc
    array.FuncIncrease = NULL;
    ercode = ms_array_realloc( &array, 0 );
    cr_assert_eq( array.Capacity, 15 );
    cr_assert_eq( ercode, MSEC_INVALID_VALUE );

    ms_array_free( &array );
}

/**
 * 04.
 * Zmienia pojemność tablicy do co najmniej podanej wartości.
 * Nie zmniejsza pojemności, do obliczenia nowej używana jest przypisana do tablicy funkcja.
 */
Test( moss_test, array_realloc_min,
    .description = "Allocates minimal ammount of memory, to increase capacity." )
{
    int      ercode;
    MS_ARRAY array = ms_array_return_local( sizeof(int), 2 );

    cr_assert_not_null( array.Items );

    // 2^1.1 = 2.14 ~= 2 (+1 -> gdy lewa jest równa prawej) = 3
    array.FuncIncrease = MSC_ArrayFunctions.IncPower;
    array.Modifier     = 1.1f;

    ercode = ms_array_realloc_min( &array, 3 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 3 );

    // 3^1.5 ~= 5, 5^1.5 ~= 11, 11^1.5 ~= 36 -> STOP, wartość minimalna (20) została osiągnięta
    array.Modifier = 1.5f;
    ercode = ms_array_realloc_min( &array, 20 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 36 );

    // w tym przypadku wartość nie powinna się zmienić, wystąpiło żądanie mniejszej wartości niż jest
    // tutaj 0 nie oznacza wartości automatycznej, podawana jest wartość minimalna
    ercode = ms_array_realloc_min( &array, 20 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 36 );

    // dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna
    array.FuncIncrease = NULL;
    ercode = ms_array_realloc_min( &array, 256 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Capacity, 256 );

    ms_array_free( &array );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    DODAWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 05.
 * Dodaje element przekazany przez wskaźnik do tablicy.
 * Sprawdza czy wszystko jest w porządku.
 */
Test( moss_test, array_insert_value,
    .description = "Insers item into array to given position." )
{
    int ercode,
        number;

    MS_ARRAY array = ms_array_return_local( sizeof(int), 2 );
    cr_assert_not_null( array.Items );

    // powiększanie dokładne - tyle ile potrzeba w aktualnym momencie
    array.FuncIncrease = NULL;

    // dodaj pierwszy element
    ercode = ms_array_push_value( &array, &LIST[0] );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 1 );
    cr_assert_eq( array.Capacity, 2 );

    // dodaj drugi element
    ercode = ms_array_push_value( &array, &LIST[1] ); 
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 2 );
    cr_assert_eq( array.Capacity, 2 );

    // dodaj trzeci - tutaj powinno nastąpić zwiększenie pojemności tablicy
    ercode = ms_array_push_value( &array, &LIST[2] ); 
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 3 );
    cr_assert_eq( array.Capacity, 3 );

    // próbuj dodać czwarty poza zakres
    ercode = ms_array_insert_value( &array, 6, &LIST[3] );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array.Length, 3 );
    cr_assert_eq( array.Capacity, 3 );

    // dodaj czwarty gdzieś po pierwszym indeksie
    ercode = ms_array_insert_value( &array, 1, &LIST[3] );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 4 );
    cr_assert_eq( array.Capacity, 4 );

    // sprawdź czy wartości pobrane będą równe zapisanym
    number = ms_array_getl( array, int, 0 );
    cr_assert_eq( number, LIST[0] );
    number = ms_array_getl( array, int, 1 );
    cr_assert_eq( number, LIST[3] );
    number = ms_array_getl( array, int, 2 );
    cr_assert_eq( number, LIST[1] );
    number = ms_array_getl( array, int, 3 );
    cr_assert_eq( number, LIST[2] );

    // koniec testu
    ms_array_free( &array );
}

/**
 * 06.
 * Dodaje kilka elementów do tablicy.
 * Elementy przekazywane są w formie standardowej tablicy języka C.
 */
Test( moss_test, array_insert_values,
    .description = "Inserts items into array to given position." )
{
    size_t x,
           y;
    int    ercode,
          *items;

    MS_ARRAY array = ms_array_return_local( sizeof(int), 2 );
    cr_assert_not_null( array.Items );
    
    // dodaj pierwszą część
    // 2 * 2 = 4 * 2 = 8 * 2 = 16 * 2 = 32 * 2 = 64 * 2 = 128 * 2 = 256
    ercode = ms_array_push_values( &array, LIST, 192 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 192 );
    cr_assert_eq( array.Capacity, 256 );

    // dodaj drugą część
    ercode = ms_array_insert_values( &array, 64, &LIST[192], 32 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 224 );
    cr_assert_eq( array.Capacity, 256 );

    // próbuj wstawić bez rozmiaru
    ercode = ms_array_push_values( &array, LIST, 0 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array.Length, 224 );
    cr_assert_eq( array.Capacity, 256 );

    // dodaj trzecią część
    ercode = ms_array_insert_values( &array, 0, &LIST[224], 32 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 256 );
    cr_assert_eq( array.Capacity, 256 );

    // próbuj wstawić poza zakres
    ercode = ms_array_insert_values( &array, 512, LIST, 32 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array.Length, 256 );
    cr_assert_eq( array.Capacity, 256 );

    // sprawdź poprawność danych
    items = (int*)array.Items;
    for( y = 224, x = 0; x < 32; ++y, ++x )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 0; y < 64; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 192; y < 224; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 64; y < 192; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );

    // zwolnij pamięć
    ms_array_free( &array );
}

/**
 * 07.
 * Dodaje elementy z innej tablicy z podanego zakresu.
 * Zakres podawany jest poprzez indeks początkowy i ilość elementów do skopiowania.
 */
Test( moss_test, array_join_slice,
    .description = "Adds items from second array within given range." )
{
    size_t x,
           y;
    int    ercode,
          *items;

    MS_ARRAY *array1 = mstst_create_sample_array( 256 ),
              array2,
              array3;

    ercode = ms_array_init( &array2, array1->ItemSize, 64 );
    cr_assert_eq( ercode, MSEC_OK );

    // inicjalizacja tablicy z innym rozmiarem elementów
    ercode = ms_array_init( &array3, array1->ItemSize * 2, 64 );
    cr_assert_eq( ercode, MSEC_OK );

    // dodaj wartości z podanego zakresu
    ercode = ms_array_join_slice( &array2, array1, 64, 64 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array2.Length, 64 );
    cr_assert_eq( array2.Capacity, 64 );

    // dodaj wartości z podanego zakresu - kopiuj do końca
    ercode = ms_array_join_slice( &array2, array1, 192, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array2.Length, 128 );
    cr_assert_eq( array2.Capacity, 128 );

    // błąd - indeks poza zakresem
    ercode = ms_array_join_slice( &array2, array1, 192, 256 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array2.Length, 128 );
    cr_assert_eq( array2.Capacity, 128 );

    // próbuj skopiować wartości do tablicy z innymi rozmiarami elementów
    ercode = ms_array_join( &array3, &array2 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array3.Length, 0 );
    cr_assert_eq( array3.Capacity, 64 );

    // sprawdź czy w tablicy znajdują się poprawne elementy
    items = (int*)array2.Items;
    for( x = 0, y = 64; y < 128; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 192; y < 256; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );

    ms_array_free( array1 );
    ms_array_free( &array2 );
    ms_array_free( &array3 );
}

/**
 * 08.
 * Dodaje elementy z innej tablicy spoza podanego przedziału.
 * Zakres podawany jest poprzez indeks początkowy i ilość elementów do skopiowania.
 */
Test( moss_test, array_join_slice_inverse,
    .description = "Adds items from second array beyond given range." )
{
    size_t x,
           y;
    int    ercode,
          *items;

    MS_ARRAY *array1 = mstst_create_sample_array( 256 ),
              array2,
              array3;

    ercode = ms_array_init( &array2, array1->ItemSize, 64 );
    cr_assert_eq( ercode, MSEC_OK );

    // inicjalizacja tablicy z innym rozmiarem elementów
    ercode = ms_array_init( &array3, array1->ItemSize * 2, 64 );
    cr_assert_eq( ercode, MSEC_OK );

    // dodaj wartości z innej tablicy z podanego zakresu (0:64, 192:256)
    ercode = ms_array_join_slice_inverse( &array2, array1, 64, 128 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array2.Length, 128 );
    cr_assert_eq( array2.Capacity, 128 );

    // próbuj dodać wartości spoza zakresu (0-64, 320-OVERFLOW)
    ercode = ms_array_join_slice_inverse( &array2, array1, 64, 256 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array2.Length, 128 );
    cr_assert_eq( array2.Capacity, 128 );

    // zakres od wartości do końca (0:128, 256:256)
    ercode = ms_array_join_slice_inverse( &array2, array1, 128, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array2.Length, 256 );
    cr_assert_eq( array2.Capacity, 256 );

    // próbuj skopiować wartości do tablicy z innymi rozmiarami elementów
    ercode = ms_array_join_slice_inverse( &array3, &array2, 128, 64 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array3.Length, 0 );
    cr_assert_eq( array3.Capacity, 64 );

    // sprawdź czy w tablicy znajdują się poprawne elementy
    items = (int*)array2.Items;
    for( x = 0, y = 0; y < 64; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 192; y < 256; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );
    for( y = 0; y < 128; ++x, ++y )
        cr_assert_eq( items[x], LIST[y] );

    ms_array_free( array1 );
    ms_array_free( &array2 );
    ms_array_free( &array3 );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    KOPIOWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 09.
 * Kopiuje całą tablicę z drugiej podanej w parametrze do pierwszej.
 * Tablica nie może być zainicjalizowana, jednak musi być zadeklarowana.
 */
Test( moss_test, array_copy,
    .description = "Copies array from second given to first." )
{
    int    ercode;
    size_t iter;

    MS_ARRAY *array1 = mstst_create_sample_array( 256 ),
              array2;

    ercode = ms_array_copy( &array2, array1 );
    cr_assert_eq( ercode, MSEC_OK );

    cr_assert_eq ( array1->Capacity, array2.Capacity );
    cr_assert_eq ( array1->Length,   array2.Length );
    cr_assert_neq( array1->Destroy,  array2.Destroy );
    cr_assert_neq( array1->Items,    array2.Items );

    // sprawdź czy elementy są takie same
    for( iter = 0; iter < array1->Length; ++iter )
        cr_assert_eq( ms_array_get(array1, int, iter), ms_array_getl(array2, int, iter) );

    ms_array_free( array1 );
    ms_array_free( &array2 );
}

/**
 * 10.
 * Kopiuje całą tablicę podaną w parametrze do nowej tablicy.
 * Zwraca wskaźnik do nowej tablicy.
 */
Test( moss_test, array_copy_alloc,
    .description = "Copies array from given array and returns pointer to new array." )
{
    size_t    iter;
    MS_ARRAY *array1 = mstst_create_sample_array( 256 ),
             *array2;

    array2 = ms_array_copy_alloc( array1 );
    cr_assert_not_null( array2 );

    cr_assert_eq ( array1->Capacity, array2->Capacity );
    cr_assert_eq ( array1->Length,   array2->Length );
    cr_assert_eq ( array1->Destroy,  array2->Destroy );
    cr_assert_neq( array1->Items,    array2->Items );

    // sprawdź czy elementy są takie same
    for( iter = 0; iter < array1->Length; ++iter )
        cr_assert_eq( ms_array_get(array1, int, iter), ms_array_get(array2, int, iter) );

    ms_array_free( array1 );
    ms_array_free( array2 );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    USUWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 11.
 * Obcina tablicę do elementów w podanym zakresie.
 * Zakres podawany jest poprzez indeks początkowy i ilość elementów do skopiowania.
 */
Test( moss_test, array_slice,
    .description = "Cuts elements from array beyond given range." )
{
    int    ercode;
    size_t x,
           y;

    MS_ARRAY *array = mstst_create_sample_array( 256 );

    // pozostaw elementy od 32 do 160
    ercode = ms_array_slice( array, 32, 128 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 128 );
    cr_assert_eq( array->Capacity, 256 );

    // sprawdź czy wartości się zgadzają
    for( x = 0, y = 32; x < 128; ++y, ++x )
        cr_assert_eq( ms_array_get(array, int, x), LIST[y] );

    // obcinanie poza zakres (32:160, elementów jest 128)
    ercode = ms_array_slice( array, 32, 128 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array->Length, 128 );
    cr_assert_eq( array->Capacity, 256 );

    // obcinanie od indeksu do końca
    ercode = ms_array_slice( array, 64, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 64 );
    cr_assert_eq( array->Capacity, 256 );

    // sprawdź czy wartości się zgadzają po przesunięciu
    for( x = 0, y = 96; x < 64; ++y, ++x )
        cr_assert_eq( ms_array_get(array, int, x), LIST[y] );

    // obcinanie od indeksu od początku - bez przesuwania elementów w pamięci
    ercode = ms_array_slice( array, 0, 32 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 32 );
    cr_assert_eq( array->Capacity, 256 );

    // wyczyść tablicę zostawiając elementy
    ms_array_clear( array );
    cr_assert_eq( array->Length, 0 );
    cr_assert_not_null( array->Items );

    // próbuj obciąć pustą tablicę
    ercode = ms_array_slice( array, 32, 128 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array->Length, 0 );
    cr_assert_eq( array->Capacity, 256 );

    ms_array_free( array );
}

/**
 * 12.
 * Usuwa elementy z tablicy w podanym zakresie.
 * Zakres podawany jest poprzez indeks początkowy i ilość elementów do skopiowania.
 */
Test( moss_test, array_remove_range,
    .description = "Cuts elements from array within given range." )
{
    int    ercode;
    size_t x,
           y;

    MS_ARRAY *array = mstst_create_sample_array( 256 );

    // usuń elementy od 64 do 128 (64+64)
    ercode = ms_array_remove_range( array, 64, 64 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 192 );
    cr_assert_eq( array->Capacity, 256 );

    // sprawdź czy wartości się zgadzają
    for( x = 0, y = 0; x < 64; ++y, ++x )
        cr_assert_eq( ms_array_get(array, int, x), LIST[y] );
    for( y = 128; y < 256; ++y, ++x )
        cr_assert_eq( ms_array_get(array, int, x), LIST[y] );

    // usuwaj od indeksu 128 do końca - usuwanie bez przesuwania
    ercode = ms_array_remove_range( array, 128, 0 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 128 );
    cr_assert_eq( array->Capacity, 256 );

    // próbuj usunąć spoza zakresu
    ercode = ms_array_remove_range( array, 128, 128 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array->Length, 128 );
    cr_assert_eq( array->Capacity, 256 );

    // wyczyść
    ms_array_clear( array );
    cr_assert_eq( array->Length, 0 );
    cr_assert_not_null( array->Items );

    // próbuj teraz coś usunąć
    ercode = ms_array_remove_range( array, 0, 5 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array->Length, 0 );
    cr_assert_eq( array->Capacity, 256 );

    ms_array_free( array );
}

/**
 * 13.
 * Usuwa element znajdujący się w tablicy w podanym miejscu.
 * Po usunięciu przesuwa wszystkie elementy znajdujące się po indeksie.
 */
Test( moss_test, array_remove,
    .description = "Removes element from array, located on given index." )
{
    MS_ARRAY *array = mstst_create_sample_array( 256 );
    int       ercode,
             *items;

    // usuń ostatni element
    ercode = ms_array_remove_last( array );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 255 );
    cr_assert_eq( array->Capacity, 256 );

    items = (int*)array->Items;
    cr_assert_eq( items[64], LIST[64] );

    // usuń z wybranej pozycji
    ercode = ms_array_remove( array, 64 );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array->Length, 254 );
    cr_assert_eq( array->Capacity, 256 );
    cr_assert_eq( items[64], LIST[65] );

    // próbuj usunąć spoza zakresu
    ercode = ms_array_remove( array, 255 );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array->Length, 254 );
    cr_assert_eq( array->Capacity, 256 );

    // próbuj usunąć z pustej tablicy
    ms_array_clear( array );
    cr_assert_eq( array->Length, 0 );
    cr_assert_not_null( array->Items );

    ercode = ms_array_remove( array, 255 );
    cr_assert_eq( ercode, MSEC_INVALID_ARGUMENT );
    cr_assert_eq( array->Length, 0 );
    cr_assert_eq( array->Capacity, 256 );

    ms_array_free( array );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    TESTY FUNKCJI BAZOWYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * 14.
 * Tworzy lokalną kopie tablicy.
 * Funkcja jest podstawą do tworzenia pochodnych o wybranym typie.
 */
Test( moss_test, array_base_return,
    .description = "Creates local array, returns struct." )
{
    MS_ARRAY array = ms_array_return( 2 );

    // sprawdź podstawwe pola struktury
    cr_assert( !array.Destroy );
    cr_assert_eq( array.Capacity, 2 );
    cr_assert_eq( array.Length, 0 );
    cr_assert_eq( array.ItemSize, (sizeof *array.Items) );
    cr_assert_not_null( array.Items );

    // zwolnij pamięć
    ms_array_free( &array );
}

/**
 * 15.
 * Dodaje elementy do tablicy we wskazane miejsce.
 * Testuje poprawność wprowadzonych danych.
 */
Test( moss_test, array_base_insert,
    .description = "Inserts item to array into given index." )
{
    int      ercode;
    MS_ARRAY array = ms_array_return( 2 );

    cr_assert_not_null( array.Items );

    // powiększanie dokładne - tyle ile potrzeba w aktualnym momencie
    array.FuncIncrease = NULL;

    // dodaj pierwszy element
    ercode = ms_array_push( &array, &LIST[0] );
    cr_assert_eq( ercode, MSEC_OK );    
    cr_assert_eq( array.Length, 1 );
    cr_assert_eq( array.Capacity, 2 );

    // dodaj drugi element
    ercode = ms_array_insert( &array, 0, &LIST[1] );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 2 );
    cr_assert_eq( array.Capacity, 2 );

    // dodaj trzeci element - tutaj powinno nastąpić zwiększenie pojemności tablicy
    ercode = ms_array_insert( &array, 1, &LIST[2] );
    cr_assert_eq( ercode, MSEC_OK );
    cr_assert_eq( array.Length, 3 );
    cr_assert_eq( array.Capacity, 3 );

    // dodaj czwarty - powinien wystąpić bład, próba dodania elementu poza zakres
    ercode = ms_array_insert( &array, 6, &LIST[1] );
    cr_assert_eq( ercode, MSEC_OUT_OF_RANGE );
    cr_assert_eq( array.Length, 3 );
    cr_assert_eq( array.Capacity, 3 );

    // sprawdź czy wartości pobrane będą równe zapisanym
    cr_assert_eq( array.Items[0], &LIST[1] );
    cr_assert_eq( array.Items[1], &LIST[2] );
    cr_assert_eq( array.Items[2], &LIST[0] );

    // koniec testu
    ms_array_free( &array );
}

/**
 * 16.
 * Tworzy lokalną kopię podanej tablicy.
 * Testuje czy kopia wykonana została poprawnie
 */
Test( moss_test, array_copy_return,
    .description = "Copies array from given array to local copy." )
{
    size_t iter;

    MS_ARRAY *array1 = mstst_create_pointer_array( 256 ),
              array2;

    array2 = ms_array_copy_return( array1 );
    cr_assert_not_null( array2.Items );

    cr_assert_eq ( array1->Capacity, array2.Capacity );
    cr_assert_eq ( array1->Length,   array2.Length );
    cr_assert_neq( array1->Destroy,  array2.Destroy );
    cr_assert_neq( array1->Items,    array2.Items );

    // sprawdź czy elementy są takie same
    for( iter = 0; iter < array1->Length; ++iter )
        cr_assert_eq( ms_array_get(array1, int, iter), ms_array_getl(array2, int, iter) );

    ms_array_free( array1 );
    ms_array_free( &array2 );
}
