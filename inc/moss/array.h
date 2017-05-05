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
 *  Header file for "Dynamic Array" module.
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

#ifndef __MSH_ARRAY__
#define __MSH_ARRAY__

#include <msconf.h>
#include <mstpl/array_template.h>

#include <stdlib.h>
#include <math.h>

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	STRUKTURY I STAŁE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Struktura tablicy dynamicznej używanej praktycznie we wszystkich funkcjach modułu.
 * Szczegóły znajdują się w opisach poszczególnych pól struktury.
 */
typedef struct MSS_ARRAY
{
	/**
	 * Pojemność tablicy.
	 * Pole uzupełniane i kontrolowane przez funkcje.
	 */
	size_t Capacity;
	/**
	 * Rozmiar pojedynczego elementu.
	 * Pole uzupełniane i kontrolowane przez funkcje.
	 */
	size_t ItemSize;
	/**
	 * Ilość elementów w tablicy, inaczej długość tablicy.
	 */
	size_t Length;
	/**
	 * Modyfikator wartości zwracanej przez funkcję zwiększającą pojemność.
	 */
	float Modifier;
	/**
	 * Niszczenie całej tablicy.
	 * Pole uzupełniane i kontrolowane przez funkcje.
	 */
	bool Destroy;
	/**
	 * Zapisane elementy w postaci tablicy standardowej języka C.
	 * Jest to jedyne pole, którego typ zmieniany jest przez makra.
	 */
	void **Items;
	/**
	 * Wskaźnik na funkcję zwiększającą pojemność tablicy.
	 * Wartość zwracana przez funkcję jest traktowana jako nowa pojemność tablicy.
	 *
	 * @param  capacity Aktualna pojemność tablicy.
	 * @param  modifier Modyfikator wartości końcowej.
	 * @return          Nowa pojemność tablicy do przydzielenia.
	 */
	size_t (*FuncIncrease)(size_t capacity, float modifier);
}
MS_ARRAY;

/**
 * Struktura wszystkich wbudowanych funkcji używanych w module.
 */
struct MSS_ARRAYFUNCTIONS
{
	/**
	 * Wskaźnik na funkcję zwiększającą pojemność tablicy.
	 * Mnoży wartość zmiennej capacity ze zmienną modifier.
	 *
	 * @param  capacity Aktualna pojemność tablicy.
	 * @param  modifier Modyfikator wartości końcowej.
	 * @return          Nowa pojemność tablicy przekazywana do przydzielenia pamięci.
	 */
	size_t (*IncMultiply)(size_t capacity, float modifier);
	
	/**
	 * Wskaźnik na funkcję zwiększającą pojemność tablicy.
	 * Dodaje wartość zmiennej capacity do zmiennej modifier.
	 *
	 * @param  capacity Aktualna pojemność tablicy.
	 * @param  modifier Modyfikator wartości końcowej.
	 * @return          Nowa pojemność tablicy przekazywana do przydzielenia pamięci.
	 */
	size_t (*IncAdd)(size_t capacity, float modifier);
	/**
	 * Wskaźnik na funkcję zwiększającą pojemność tablicy.
	 * Podnosi wartość zmiennej capacity do potęgi o wartości zmiennej modifier.
	 *
	 * @param  capacity Aktualna pojemność tablicy.
	 * @param  modifier Modyfikator wartości końcowej.
	 * @return          Nowa pojemność tablicy przekazywana do przydzielenia pamięci.
	 */
	size_t (*IncPower)(size_t capacity, float modifier);
};

/**
 * Stała przechowująca wskaźniki do wbudowanych funkcji modułu.
 * Szczegółowy opis znajduje się w polach struktury MSS_ARRAYFUNCTIONS.
 */
extern const struct MSS_ARRAYFUNCTIONS MSC_ArrayFunctions;

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	TWORZENIE I INICJALIZACJA
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Tworzy nową tablicę oraz rezerwuje początkowe miejsce na dane.
 *
 * @param  size     Rozmiar pojedynczego elementu przechowywanego w tablicy.
 * @param  capacity Początkowa ilość miejsca do zarezerwowania na elementy tablicy.
 * @return          Wskaźnik na utworzoną tablicę lub NULL w przypadku błędu.
 *
 * Funkcja nie modyfikuje zmiennej ERRNO.
 */
void *ms_array_alloc( size_t size, size_t capacity );

/**
 * Inicjalizuje istniejącą tablicę i rezerwuje początkowe miejsce na dane.
 * 
 * @param  aptr     Wskaźnik na tablicę.
 * @param  size     Rozmiar pojedynczego elementu przechowywanego w tablicy.
 * @param  capacity Początkowa ilość miejsca do zarezerwowania na elementy tablicy.
 * @return          Kod błędu lub wartość MSEC_OK.
 */
int ms_array_init( void *aptr, size_t size, size_t capacity );

/**
 * Tworzy tablicę lokalną oraz rezerwuje początkowe miejsce na dane.
 * 
 * @param  size     Rozmiar pojedynczego elementu przechowywanego w tablicy.
 * @param  capacity Początkowa ilość miejsca do zarezerwowania na elementy tablicy.
 * @return          Utworzona tablica lokalna.
 */
MS_ARRAY ms_array_return_local( size_t size, size_t capacity );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Zmniejsza lub zwiększa pojemność tablicy w zależności od parametru capacity.
 * Podanie wartości 0 w capacity, powoduje automatyczne obliczenie pojemności z użyciem funkcji zapisanej w tablicy.
 * Podanie wartości innej niż 0 powoduje zmianę pojemności do wybranej wartości, z czego wszystkie elementy
 * występujące w tablicy muszą się zmieścić.
 * 
 * @param  aptr     Wskaźnik na tablicę.
 * @param  capacity Nowa pojemność tablicy lub 0 w przypadku zwiększania automatycznego.
 * @return          Kod błędu lub wartość MSEC_OK.
 */
int ms_array_realloc( void *aptr, size_t capacity );

/**
 * Zwiększa pojemność tablicy do co najmniej podanej wartości w parametrze min.
 * Funkcja zwiększająca pojemność nie jest potrzebna w tym wypadku.
 * Gdy funkcja jest ustawiona, nowa pojemność może być wyższa niż wartość podana w parametrze min.
 *
 * @param  aptr Wskaźnik na tablicę.
 * @param  min  Minimalna wartość do której zwiększona ma zostać pojemność tablicy.
 * @return      Kod błędu lub wartość MSEC_OK.
 */
int ms_array_realloc_min( void *aptr, size_t min );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	KOPIOWANIE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Kopiuje elementy z pierwszej tablicy do drugiej.
 * Tablica do której dane są kopiowane musi isntnieć, jednak nie może być zainicjalizowana.
 * 
 * @param  adst Wskaźnik na tablicę która będzie kopią drugiej.
 * @param  asrc Wskaźnik na tablicę przeznaczoną do skopiowania.
 * @return      Kod błedu lub wartość MSEC_OK.
 */
int ms_array_copy( void *adst, const void *asrc );

/**
 * Tworzy nową tablicę i kopiuje do niej dane z tablicy podanej w parametrze.
 * 
 * @param  aptr Wskaźnik na tablicę przeznaczoną do skopiowania.
 * @return      Utworzona kopia tablicy lub wartość NULL w przypadku błedu.
 */
void *ms_array_copy_alloc( const void *aptr );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DODAWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Dodaje element do tablicy w wyznaczonym miejscu.
 * Przekazywany element musi być wskaźnikiem do elementu rzeczywistego.
 * 
 * @param  aptr  Wskaźnik na tablicę.
 * @param  index Indeks do którego element zostanie skopiowany.
 * @param  item  Wskaźnik na element do dodania.
 * @return       Kod błędu lub wartość MSEC_OK.
 */
int ms_array_insert_value( void *aptr, size_t index, const void *item );

/**
 * Dodaje elementy do tablicy w wyznaczonym miejscu.
 * Lista elementów do dodania musi znajdować się w tablicy standardowej języka C.
 * 
 * @param  adst   Wskaźnik na tablicę.
 * @param  index  Indeks do którego element zostanie skopiowany.
 * @param  tsrc   Wskaźnik na tablicę elementów do dodania.
 * @param  count  Ilość elementów do skopiowania.
 * @return        Kod błędu lub wartość MSEC_OK.
 */
int ms_array_insert_values( void *adst, size_t index, const void *tsrc, size_t count );

/**
 * Dodaje elementy z podanego przedziału z drugiej tablicy do pierwszej.
 * 
 * @param  adst   Wskaźnik na tablicę do której kopiowane będą elementy.
 * @param  asrc   Wskaźnik na tablicę z której dane będą kopiowane.
 * @param  offset Indeks od którego elementy będą kopiowane.
 * @param  count  Ilość elementów do skopiowania.
 * @return        Kod błędu lub wartość MSEC_OK.
 */
int ms_array_join_slice( void *adst, const void *asrc, size_t offset, size_t count );

/**
 * Dodaje elementy znajdujące się poza podanym przedziałem z drugiej tablicy do pierwszej.
 * 
 * @param  adst   Wskaźnik na tablicę do której kopiowane będą elementy.
 * @param  asrc   Wskaźnik na tablicę z której dane będą kopiowane.
 * @param  offset Indeks od którego elementy będą ignorowane.
 * @param  count  Ilość ignorowanych elementów.
 * @return        Kod błędu lub wartość MSEC_OK.
 */
int ms_array_join_slice_inverse( void *adst, const void *asrc, size_t offset, size_t count );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	USUWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Pozostawia w tablicy elementy umieszczone w podanym zakresie.
 *
 * @param aptr   Wskaźnik na tablicę.
 * @param offset Indeks od którego elementy nie mogą być usuwane.
 * @param count  Ilość pozostawionych elementów.
 */
int ms_array_slice( void *aptr, size_t offset, size_t count );

/**
 * Usuwa z tablicy elementy znajdujące się w podanym zakresie.
 * Aliasem tej funkcji jest ms_array_slice_inverse.
 * 
 * @param aptr   Wskaźnik na tablicę.
 * @param offset Indeks od którego elementy mają być usuwane.
 * @param count  Ilość usuwanych elementów.
 */
int ms_array_remove_range( void *aptr, size_t offset, size_t count );

/**
 * Usuwa element o podanym indeksie z tablicy.
 * 
 * @param aptr  Wskaźnik do tablicy z której element ma zostać usunięty.
 * @param index Indeks elementu do usunięcia.
 */
int ms_array_remove( void *aptr, size_t index );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	CZYSZCZENIE DANYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Czyści tablicę usuwając jej wszystkie elementy.
 * Funkcja nie zwalnia pamięci po elementach i nie zmniejsza jej.
 * 
 * @param aptr Wskaźnik do tablicy przeznaczonej do wyczyszczenia.
 */
void ms_array_clear( void *aptr );

/**
 * Zwalnia zasoby przydzielone zarówno do tablicy jak i jej elementów.
 * 
 * @param aptr Wskaźnik do tablicy przeznaczonej do usunięcia.
 */
void ms_array_free( void *aptr );

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	MAKRA
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Pobiera element z tablicy o podanym typie i indeksie.
 * Działa tylko na zmiennej zawierającej wskaźnik na tablicę.
 *
 * @param  array Wskaźnik na tablicę.
 * @param  type  Typ pobieranego elementu.
 * @param  index Indeks elementu do pobrania.
 * @return       Element pobrany z tablicy o typie podanym w parametrze.
 */
#define ms_array_get(array, type, index) \
	((type*)array->Items)[index]

/**
 * Pobiera element z tablicy o podanym typie i indeksie.
 * Działa tylko na zmiennej lokalnej tablicy.
 *
 * @param  array Tablica lokalna.
 * @param  type  Typ pobieranego elementu.
 * @param  index Indeks elementu do pobrania.
 * @return       Element pobrany z tablicy o typie podanym w parametrze.
 */
#define ms_array_getl(array, type, index) \
	((type*)array.Items)[index]

/**
 * Alias funkcji ms_array_remove_range.
 * 
 * @param aptr   Wskaźnik na tablicę.
 * @param offset Indeks od którego elementy mają być usuwane.
 * @param count  Ilość usuwanych elementów.
 */
#define ms_array_slice_inverse(aptr, offset, count) \
	ms_array_remove_range( aptr, offset, count )

/**
 * Kopiuje wszystkie elementy z drugiej tablicy do pierwszej.
 * Alias funkcji ms_array_join_slice uzupełniający dwa ostatnie parametry wartościami 0.
 *
 * @param  adst Wskaźnik na tablicę do której kopiowane będą elementy.
 * @param  asrc Wskaźnik na tablicę z której dane będą kopiowane.
 * @return      Kod błędu lub wartość MSEC_OK.
 */
#define ms_array_join(adst, asrc) \
	ms_array_join_slice(adst, asrc, 0, 0)

/**
 * Kopiuje wszystkie podane elementy do tablicy.
 * Alias funkcji ms_array_insert_values, uzupełniający indeks aktualną długością tablicy.
 * 
 * @param  adst  Wskaźnik na tablicę do której mają być kopiowane elementy.
 * @param  tsrc  Wskaźnik na elementy do skopiowania.
 * @param  count Ilość elementów do skopiowania.
 * @return       Kod błędu lub wartość MSEC_OK.
 */
#define ms_array_push_values(adst, tsrc, count) \
	ms_array_insert_values( adst, (adst)->Length, tsrc, count )

/**
 * Dodaje element do tablicy w wyznaczonym miejscu.
 * Przekazywany element musi być wskaźnikiem do elementu rzeczywistego.
 * 
 * @param  aptr Wskaźnik na tablicę.
 * @param  item Wskaźnik na wstawiany element.
 * @return      Kod błedu lub wartość MSEC_OK.
 */
#define ms_array_push_value(aptr, item) \
	ms_array_insert_value( aptr, (aptr)->Length, item )

/**
 * Usuwa ostatni element z tablicy.
 * Alias funkcji ms_array_remove uzupełniający indeks długością tablicy pomniejszoną o 1.
 * 
 * @param  aptr Wskaźnik do tablicy.
 */
#define ms_array_remove_last(aptr) \
	ms_array_remove( aptr, (aptr)->Length - 1 )

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	BAZA FUNKCJI POCHODNYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Wbudowana odmiana funkcji zwracającej tablicę lokalną bez podawania rozmiaru elementu.
 * Działa na takiej samej zasadzie jak funkcja ms_array_return_size.
 * 
 * @param  capacity Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
 * @return          Utworzona tablica lokalna.
 */
MS_ARRAY ms_array_return( size_t capacity );

/**
 * Wbudowana odmiana funkcji kopiującej podaną tablicę, tworząc jej lokalny odpowiednik.
 * Działa na takiej samej zasadzie jak funkcja ms_array_copy_alloc z tą różnicą iż nie przydziela miejsca
 * na nową tablicę, ale tworzy ją lokalnie.
 * 
 * @param  src Wskaźnik na tablicę do skopiowania.
 * @return     Utworzona lokalna kopia tablicy.
 */
MS_ARRAY ms_array_copy_return( const MS_ARRAY *src );

/**
 * Wbudowana odmiana funkcji dodającej element w wybrane miejsce do tablicy.
 * W tej wersji nie trzeba podawać wskaźnika do elementu, który zostanie umieszczony w tablicy.
 * 
 * @param  array Wskaźnik na tablicę.
 * @param  index Indeks w tablicy do którego wstawiony ma być element.
 * @param  item  Element do wstawienia.
 * @return       Kod błędu lub wartość MSEC_OK.
 */
int ms_array_insert( MS_ARRAY *array, size_t index, const void *item );

/**
 * Dodaje element na koniec tablicy.
 * Alias do funkcji ms_array_insert, uzupełniający indeks aktualną długością tablicy.
 * Dla konkretnych typów tworzone będą funkcję zamiast makr z racji tego, iż preprocesor nie
 * poradzi sobie z tworzeniem makra w dyrektywie #define.
 * 
 * @param  array Wskaźnik na tablicę.
 * @param  item  Element do wstawienia.
 * @return       Kod błędu lub wartość MSEC_OK.
 */
int ms_array_push( MS_ARRAY *array, const void *item );

#endif
