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
 *  Source file for "Dynamic Array" module.
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

#include <moss/array.h>

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	NAGŁÓWKI FUNKCJI WBUDOWANYCH I STAŁE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Modyfikuje wartość zmiennej capacity, mnożąc ją ze zmienną modifier.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  modifier Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_multiply( size_t capacity, float modifier );

/**
 * Modyfikuje wartość zmiennej capacity, dodając do niej wartość zmiennej modifier.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  modifier Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_add( size_t capacity, float modifier );

/**
 * Modyfikuje wartość zmiennej capacity, podnosząc ją do potęgi wartości zmiennej modifier.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  modifier Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_power( size_t capacity, float modifier );

/* ================================================================================================================== */

const struct MSS_ARRAYFUNCTIONS MSC_ArrayFunctions =
{
	msf_array_increase_multiply,    /* IncMultiply */
	msf_array_increase_add,         /* IncAdd */
	msf_array_increase_power        /* IncPower */
};

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	TWORZENIE I INICJALIZACJA
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

void *ms_array_alloc( size_t size, size_t capacity )
{
	MS_ARRAY *array;

	if( (array = malloc(sizeof *array)) == NULL )
		return NULL;

	if( ms_array_init(array, size, capacity) )
		return
			free( array ),
			NULL;

	array->Destroy = TRUE;
	return array;
}

/* ================================================================================================================== */

int ms_array_init( void *aptr, size_t size, size_t capacity )
{
	MS_ARRAY *array = aptr;

	assert( array );

	array->Capacity = capacity == 0
		? MSD_ARRAY_DEFAULT_SIZE
		: capacity;
	array->ItemSize = size;
	array->Length   = 0;
	array->Modifier = 2.f;
	array->Destroy  = FALSE;

	array->FuncIncrease = MSC_ArrayFunctions.IncMultiply;

	/* przydziel pamięć na elementy tablicy */
	array->Items = malloc( array->Capacity * array->ItemSize );

	if( !array->Items )
		return
			ms_array_free( array ),
			MSEC_MEMORY_ALLOCATION;

	return MSEC_OK;
}

/* ================================================================================================================== */

MS_ARRAY ms_array_return_local( size_t size, size_t capacity )
{
	MS_ARRAY array;
	IGRET ms_array_init( &array, size, capacity );
	return array;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	ZMIANA POJEMNOŚCI
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int ms_array_realloc( void *aptr, size_t capacity )
{
	MS_ARRAY *array = aptr;
	void     *tmp;

	assert( array );
	assert( array->Items );

	/* powiększ ilość pamięci - w przypadku dokładnego zwiększania, wartość musi być podana w capacity */
	if( !capacity )
	{
		if( !array->FuncIncrease )
			SETERRNOANDRETURN( MSEC_INVALID_VALUE );

		capacity = array->FuncIncrease( array->Capacity, array->Modifier );
	
		if( capacity <= array->Capacity )
			capacity = array->Capacity + 1;
	}
	/* aby przydzielić taką ilość pamięci elementy muszą najpierw zostać usunięte */
	else if( array->Length > capacity )
		SETERRNOANDRETURN( MSEC_DATA_OVERFLOW );

	/* przydziel nową ilość pamięci */
	if( (tmp = realloc(array->Items, capacity * array->ItemSize)) == NULL )
		return MSEC_MEMORY_ALLOCATION;

	array->Items    = tmp;
	array->Capacity = capacity;

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_realloc_min( void *aptr, size_t min )
{
	MS_ARRAY *array = aptr;

	size_t capacity;
	size_t oldcap;

	assert( array );
	assert( array->Items );

	capacity = array->Capacity;

	/* osiągnij co najmniej wartość minimalną, z czego nowy rozmiar nie może być mniejszy niż stary */
	while( min > capacity )
	{
		oldcap   = capacity;
		capacity = !array->FuncIncrease
			? min
			: array->FuncIncrease( oldcap, array->Modifier );
		if( capacity <= oldcap )
			capacity = oldcap + 1;
	}

	/* przydziel nową ilość pamięci gdy pojemność się różni */
	if( capacity != array->Capacity )
		return ms_array_realloc( aptr, capacity );

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	KOPIOWANIE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int ms_array_copy( void *adst, const void *asrc )
{
	const MS_ARRAY *src = asrc;
	MS_ARRAY       *dst = adst;
	
	int ercode;

	assert( src );
	assert( src->Items );
	assert( dst );

	if( (ercode = ms_array_init(dst, src->ItemSize, src->Capacity)) != 0 )
		return ercode;

	IGRET memcpy( dst->Items, src->Items, src->Capacity * src->ItemSize );
	dst->Length = src->Length;

	return MSEC_OK;
}

/* ================================================================================================================== */

void *ms_array_copy_alloc( const void *aptr )
{
	const MS_ARRAY *array = aptr;
	MS_ARRAY       *retval;

	assert( array );
	assert( array->Items );

	if( (retval = ms_array_alloc(array->ItemSize, array->Capacity)) == NULL )
		return NULL;

	IGRET memcpy( retval->Items, array->Items, array->Capacity * array->ItemSize );
	retval->Length = array->Length;

	return retval;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DODAWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int ms_array_insert_value( void *aptr, size_t index, const void *item )
{
	MS_ARRAY *array = aptr;
	void     *ptr;

	assert( array );
	assert( array->Items );
	assert( item );

	if( index > array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
	
	/* sprawdź czy nowy element się zmieści */
	if( array->Length >= array->Capacity )
	{
		int ercode = ms_array_realloc( array, !array->FuncIncrease
			? array->Capacity + 1
			: 0 );
		if( ercode )
			return ercode;
	}
	ptr = (unsigned char*)array->Items + array->ItemSize * index;

	/* przesuń elementy */
	if( index != array->Length )
		IGRET memmove( (unsigned char*)ptr + array->ItemSize, ptr, array->ItemSize * (array->Length - index) );

	/* dodaj nowy element */
	IGRET memcpy( ptr, item, array->ItemSize );
	++array->Length;

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_insert_values( void *adst, size_t index, const void *tsrc, size_t size )
{
	MS_ARRAY *array = adst;
	void     *ptr;

	assert( array );
	assert( array->Items );
	assert( tsrc );

	if( index > array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	/* sprawdź czy nowy element się zmieści */
	if( array->Length + size > array->Capacity )
	{
		int ercode = ms_array_realloc_min( array, array->Length + size );
		if( ercode )
			return ercode;
	}
	else if( !size )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

	ptr = (unsigned char*)array->Items + array->ItemSize * index;

	/* przesuń elementy
	 * memmove potrafi radzić sobie z nakładającymi się adresami dwóch wskaźników */
	if( index != array->Length )
		IGRET memmove( (unsigned char*)ptr + array->ItemSize * size, ptr, array->ItemSize * (array->Length - index) );

	/* kopiuj elementy do tablicy */
	IGRET memcpy( ptr, tsrc, array->ItemSize * size );
	array->Length += size;

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_join_slice( void *adst, const void *asrc, size_t offset, size_t count )
{
	const MS_ARRAY *src = asrc;
	MS_ARRAY       *dst = adst;

	assert( src );
	assert( src->Items );
	assert( dst );
	assert( dst->Items );

	if( count == 0 )
		count = src->Length - offset;

	/* rozmiary muszą się zgadzać oraz indeksy nie mogą wychodzić poza ilość elementów w tablicy */
	if( src->ItemSize != dst->ItemSize || src->Length == 0 )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	if( offset >= src->Length || offset + count > src->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	return ms_array_push_values( dst, (unsigned char*)src->Items + offset * src->ItemSize, count );
}

/* ================================================================================================================== */

int ms_array_join_slice_inverse( void *adst, const void *asrc, size_t offset, size_t count )
{
	size_t          osumc = offset + count;
	const MS_ARRAY *src   = asrc;
	MS_ARRAY       *dst   = adst;

	int ercode;

	assert( src );
	assert( src->Items );
	assert( dst );
	assert( dst->Items );

	if( count == 0 )
		osumc = offset + (count = src->Length - offset);

	/* rozmiary muszą się zgadzać oraz indeksy nie mogą wychodzić poza ilość elementów w tablicy */
	if( src->ItemSize != dst->ItemSize || src->Length == 0 )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	if( offset >= src->Length || osumc > src->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	if( dst->Length < src->Length - count )
		if( (ercode = ms_array_realloc_min(dst, src->Length - count)) != 0 )
			return ercode;

	/* dodaj pierwszą część */
	if( offset > 0 && (ercode = ms_array_push_values(dst, src->Items, offset)) != 0 )
		return ercode;

	/* dodaj drugą część */
	if( osumc < src->Length )
	{
		ercode = ms_array_push_values(
			dst,
			(unsigned char*)src->Items + osumc * src->ItemSize,
			src->Length - osumc
		);
		if( ercode )
			return ercode;
	}
	
	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	USUWANIE ELEMENTÓW
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

int ms_array_slice( void *aptr, size_t offset, size_t count )
{
	MS_ARRAY *array = aptr;

	assert( array );
	assert( array->Items );

	if( count == 0 )
		count = array->Length - offset;

	if( array->Length == 0 )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	if( offset >= array->Length || offset + count > array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	if( offset != 0 )
		IGRET memmove(
			array->Items,
			(unsigned char*)array->Items + offset * array->ItemSize,
			count * array->ItemSize
		);

	array->Length = count;
	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_remove_range( void *aptr, size_t offset, size_t count )
{
	MS_ARRAY      *array = aptr;
	unsigned char *ptr1;
	unsigned char *ptr2;

	assert( array );
	assert( array->Items );

	/* do końca... */
	if( count == 0 )
		count = array->Length - offset;

	if( array->Length == 0 )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	if( offset >= array->Length || offset + count > array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	if( count + offset == array->Length )
		array->Length = offset;
	else
		ptr1 = (unsigned char*)array->Items + offset * array->ItemSize,
		ptr2 = ptr1 + count * array->ItemSize,

		IGRET memmove( ptr1, ptr2, (array->Length - (count + offset)) * array->ItemSize ),
		array->Length -= count;
	
	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_remove( void *aptr, size_t index )
{
	MS_ARRAY      *array = aptr;
	unsigned char *ptr1;
	unsigned char *ptr2;

	assert( array );
	assert( array->Items );

	/* indeks poza zakresem, nic nie rób... */
	if( array->Length == 0 )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	else if( index >= array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	--array->Length;

	/* przenoś tylko gdy usuwany element nie jest ostatni */
	if( index != array->Length )
		/* przenieś wszystkie elementy znajdujące się za indeksem */
		ptr1 = (unsigned char*)array->Items + array->ItemSize * index,
		ptr2 = ptr1 + array->ItemSize,

		IGRET memmove( ptr1, ptr2, array->ItemSize * (array->Length - index) );
	
	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	CZYSZCZENIE DANYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

void ms_array_clear( void *aptr )
{
	MS_ARRAY *array = aptr;

	assert( array );
	assert( array->Items );

	array->Length = 0;
}

/* ================================================================================================================== */

void ms_array_free( void *aptr )
{
	MS_ARRAY *array = aptr;

	if( !array )
		return;

	/* usuń wszystkie elementy z tablicy */
	if( array->Items )
		ms_array_clear( array );

	free( array->Items );
	array->Items = NULL;

	array->Capacity = 0;
	array->ItemSize = 0;
	array->Length   = 0;
	array->Modifier = 2.f;

	if( array->Destroy )
		free( array );
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	BAZA FUNKCJI POCHODNYCH
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

MS_ARRAY ms_array_return( size_t capacity )
{
	MS_ARRAY array;
	IGRET ms_array_init( &array, sizeof(*array.Items), capacity );
	return array;
}

/* ================================================================================================================== */

MS_ARRAY ms_array_copy_return( const MS_ARRAY *array )
{
	MS_ARRAY copy;
	IGRET ms_array_copy( &copy, array );
	return copy;
}

/* ================================================================================================================== */

int ms_array_push( MS_ARRAY *array, const void *item )
{
	assert( array );
	assert( array->Items );

	return ms_array_insert( array, array->Length, item );
}

/* ================================================================================================================== */

int ms_array_insert( MS_ARRAY *array, size_t index, const void *item )
{
	assert( array );
	assert( array->Items );

	/* sprawdź czy indeks nie wyjdzie poza granice */
	if( index > array->Length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	/* sprawdź czy nowy element się zmieści */
	if( array->Length >= array->Capacity )
	{
		int ercode = ms_array_realloc( array, !array->FuncIncrease
			? array->Capacity + 1
			: 0 );
		if( ercode )
			return ercode;
	}
	/* przesuń elementy */
	if( index != array->Length )
		IGRET memmove( &array->Items[index + 1], &array->Items[index], array->ItemSize * (array->Length - index) ); 

	/* dodaj nowy element */
	array->Items[index] = (void*)item;
	++array->Length;

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	FUNKCJE WEWNĘTRZNE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

static size_t msf_array_increase_multiply( size_t capacity, float factor )
{
	return (size_t)(factor * (float)capacity);
}

/* ================================================================================================================== */

static size_t msf_array_increase_add( size_t capacity, float factor )
{
	return capacity + (size_t)factor;
}

/* ================================================================================================================== */

static size_t msf_array_increase_power( size_t capacity, float factor )
{
	return (size_t)pow( (double)capacity, factor );
}
