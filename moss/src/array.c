/*
 * Moss Library >> http://moss.aculo.pl
 *
 *    /'\_/`\                           
 *   /\      \    ___     ____    ____  
 *   \ \ \__\ \  / __`\  /',__\  /',__\ 
 *    \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *     \ \_\\ \_\ \____/\/\____/\/\____/
 *      \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 * Source file for "Dynamic Array" module...
 */

#include "../inc/array.h"

/* ================================================================================================================== */

/**
 * Modyfikuje wartość zmiennej capacity, mnożąc ją ze zmienną factor.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  factor   Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_multiply( size_t capacity, float factor );

/**
 * Modyfikuje wartość zmiennej capacity, dodając do niej wartość zmiennej factor.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  factor   Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_add( size_t capacity, float factor );

/**
 * Modyfikuje wartość zmiennej capacity, podnosząc ją do potęgi wartości zmiennej factor.
 *
 * @param  capacity Aktualna pojemność tablicy.
 * @param  factor   Dodatkowy współczynnik do obliczeń.
 * @return          Nowa pojemność tablicy do przydzielenia.
 */
static size_t msf_array_increase_power( size_t capacity, float factor );

/* ================================================================================================================== */

const struct MSS_ARRAYFUNCTIONS MSC_ArrayFunctions =
{
    msf_array_increase_multiply,    // IncMultiply
    msf_array_increase_add,         // IncAdd
    msf_array_increase_power        // IncPower
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
    MS_ARRAY *array = NULL;

    if( !(array = malloc(sizeof *array)) )
        return NULL;

    // inicjalizacja tablicy
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

    array->Capacity  = capacity;
    array->ItemSize  = size;
    array->Length    = 0;
    array->Factor    = 2.f;
    array->Destroy   = FALSE;
    array->Increator = MSC_ArrayFunctions.IncMultiply;

    // przydziel pamięć na elementy tablicy
    array->Items = malloc( array->Capacity * array->ItemSize );

    if( !array->Items )
        return
            ms_array_free( array ),
            MSEC_MEMORY_ALLOCATION;

    return MSEC_OK;
}

/* ================================================================================================================== */

MS_ARRAY ms_array_mreturn( size_t size, size_t capacity )
{
    MS_ARRAY array;
    IGRET ms_array_init( &array, size, capacity );
    return array;
}

/* ZMIANA POJEMNOŚCI */
/* ================================================================================================================== */

int ms_array_realloc( void *aptr, size_t capacity )
{
    MS_ARRAY *array = aptr;
    void     *tmp;

    assert( array );
    assert( array->Items );

    // powiększ ilość pamięci - w przypadku dokładnego zwiększania, wartość musi być podana w capacity
    if( !capacity )
    {
        if( !array->Increator )
            SETERRNOANDRETURN( MSEC_INVALID_VALUE );

        capacity = array->Increator( array->Capacity, array->Factor );
    
        if( capacity <= array->Capacity )
            capacity = array->Capacity + 1;
    }
    // aby przydzielić taką ilość pamięci elementy muszą najpierw zostać usunięte
    else if( array->Length > capacity )
        SETERRNOANDRETURN( MSEC_DATA_OVERFLOW );

    // przydziel nową ilość pamięci
    if( !(tmp = realloc(array->Items, capacity * array->ItemSize)) )
        return MSEC_MEMORY_ALLOCATION;

    array->Items    = tmp;
    array->Capacity = capacity;

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_min_realloc( void *aptr, size_t min )
{
    MS_ARRAY *array = aptr;

    void  *tmp;
    size_t capacity,
           oldcap;

    assert( array );
    assert( array->Items );

    capacity = array->Capacity;

    // osiągnij co najmniej wartość minimalną, z czego nowy rozmiar nie może być mniejszy niż stary
    while( min > capacity )
    {
        oldcap   = capacity;
        capacity = !array->Increator
            ? min
            : array->Increator( oldcap, array->Factor );
        if( capacity <= oldcap )
            capacity = oldcap + 1;
    }

    // przydziel nową ilość pamięci gdy pojemność się różni
    if( capacity != array->Capacity )
        return ms_array_realloc( aptr, capacity );

    return MSEC_OK;
}

/* KOPIOWANIE */
/* ================================================================================================================== */

int ms_array_copy( void *adst, const void *asrc )
{
    const MS_ARRAY *src = asrc;
    
    MS_ARRAY *dst = adst;
    int retval;

    assert( src );
    assert( src->Items );
    assert( dst );

    if( (retval = ms_array_init(dst, src->ItemSize, src->Capacity)) )
        return retval;

    IGRET memcpy( dst->Items, src->Items, src->Capacity * src->ItemSize );
    dst->Length = src->Length;

    return MSEC_OK;
}

/* ================================================================================================================== */

void *ms_array_copy_alloc( void *aptr )
{
    MS_ARRAY *retval,
             *array = aptr;

    assert( array );
    assert( array->Items );

    // utwórz nową tablicę
    if( !(retval = ms_array_alloc(array->ItemSize, array->Capacity)) )
        return NULL;
    
    // kopiuj zawartość
    IGRET memcpy( retval->Items, array->Items, array->Capacity * array->ItemSize );
    retval->Length = array->Length;

    return retval;
}

/* DODAWANIE ELEMENTÓW */
/* ================================================================================================================== */

int ms_array_mpush( void *aptr, void *item )
{
    MS_ARRAY      *array = aptr;
    unsigned char *mem   = item;

    assert( array );
    assert( array->Items );
    assert( mem );

    // sprawdź czy nowy element się zmieści
    if( array->Length >= array->Capacity )
    {
        int retval = !array->Increator
            ? array->Capacity + 1
            : 0;
        if( (retval = ms_array_realloc(array, retval)) )
            return retval;
    }

    // kopiuj element do tablicy
    memcpy( ((unsigned char*)array->Items) + array->ItemSize * array->Length, item, array->ItemSize );
    ++array->Length;

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_minsert( void *aptr, size_t index, void *item )
{
    MS_ARRAY *array = aptr;
    size_t    iter;
    void     *ptr;

    assert( array );
    assert( array->Items );

    // sprawdź czy indeks nie wyjdzie poza granice
    if( index > array->Length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
    
    // sprawdź czy nowy element się zmieści
    if( array->Length >= array->Capacity )
    {
        int retval = !array->Increator
            ? array->Capacity + 1
            : 0;
        if( (retval = ms_array_realloc(array, retval)) )
            return retval;
    }
    ptr = (unsigned char*)array->Items + array->ItemSize * index;

    // przesuń elementy
    if( index != array->Length )
        memmove( (unsigned char*)ptr + array->ItemSize, ptr, array->ItemSize * (array->Length - index) );

    // dodaj nowy element
    memcpy( ptr, item, array->ItemSize );
    ++array->Length;

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_apush_slice( void *aptrt, void *aptrf, size_t start, size_t count )
{
    MS_ARRAY *array1 = aptrt,
             *array2 = aptrf;

    assert( array1 );
    assert( array1->Items );
    assert( array2 );
    assert( array2->Items );

    // rozmiary muszą się zgadzać oraz indeksy nie mogą wychodzić poza ilość elementów w tablicy
    if( array1->ItemSize != array2->ItemSize || start >= array2->Length || start + count > array2->Length )
        SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

    // wstawiaj do końca
    if( count == 0 )
        count = array2->Length - start;

    return ms_array_tpush( aptrt, (unsigned char*)array2->Items + start * array2->ItemSize, count );
}

/* ================================================================================================================== */

int ms_array_ainsert_slice( void *aptrt, size_t index, void *aptrf, size_t start, size_t count )
{
    MS_ARRAY *array1 = aptrt,
             *array2 = aptrf;

    assert( array1 );
    assert( array1->Items );
    assert( array2 );
    assert( array2->Items );

    // rozmiary muszą się zgadzać oraz indeksy nie mogą wychodzić poza ilość elementów w tablicy
    if( array1->ItemSize != array2->ItemSize || start >= array2->Length || start + count > array2->Length )
        SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

    // wstawiaj do końca
    if( count == 0 )
        count = array2->Length - start;

    return ms_array_tinsert( aptrt, index, (unsigned char*)array2->Items + start * array2->ItemSize, count );
}

/* ================================================================================================================== */

int ms_array_tpush( void *aptr, void *tab, size_t size )
{
    MS_ARRAY *array = aptr;

    assert( array );
    assert( array->Items );
    assert( tab );

    // sprawdź czy nowy element się zmieści
    if( array->Length + size > array->Capacity )
    {
        int retval;
        if( (retval = ms_array_min_realloc(array, array->Length + size)) )
            return retval;
    }
    // brak rozmiaru, nie przechodź do wstawiania
    else if( !size )
        return MSEC_OK;

    // kopiuj elementy do tablicy
    memcpy( ((unsigned char*)array->Items) + array->ItemSize * array->Length, tab, array->ItemSize * size );
    array->Length += size;

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_tinsert( void *aptr, size_t index, void *tab, size_t size )
{
    MS_ARRAY *array = aptr;
    void     *ptr;

    assert( array );
    assert( array->Items );
    assert( tab );

    // indeks poza zasięgiem
    if( index > array->Length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

    // sprawdź czy nowy element się zmieści
    if( array->Length + size > array->Capacity )
    {
        int retval;
        if( (retval = ms_array_min_realloc(array, array->Length + size)) )
            return retval;
    }
    // brak rozmiaru, nie przechodź do kopiowania
    else if( !size )
        return MSEC_OK;

    ptr = (unsigned char*)array->Items + array->ItemSize * index;

    // przesuń elementy
    // memmove potrafi radzić sobie z nakładającymi się adresami dwóch wskaźników
    if( index != array->Length )
        memmove( (unsigned char*)ptr + array->ItemSize * size, ptr, array->ItemSize * (array->Length - index) );

    // kopiuj elementy do tablicy
    memcpy( ptr, tab, array->ItemSize * size );
    array->Length += size;

    return MSEC_OK;
}

/* USUWANIE ELEMENTÓW */
/* ================================================================================================================== */

void *ms_array_mpop( void *aptr )
{
    MS_ARRAY *array = aptr;
    void *tmp;

    assert( array );
    assert( array->Items );

    if( !array->Length )
        return NULL;

    if( !(tmp = malloc(array->ItemSize)) )
        return NULL;

    memcpy( tmp, (unsigned char*)array->Items + (array->Length - 1) * array->ItemSize, array->ItemSize );
    --array->Length;

    return tmp;
}

/* ================================================================================================================== */

void ms_array_nrpop( void *aptr )
{
    MS_ARRAY *array = aptr;

    assert( array );
    assert( array->Items );

    if( !array->Length )
        return;

    --array->Length;
}

/* ================================================================================================================== */

void ms_array_remove( void *aptr, size_t index )
{
    MS_ARRAY      *array = aptr;
    unsigned char *ptr1,
                  *ptr2;

    assert( array );
    assert( array->Items );

    // indeks poza zakresem, nic nie rób...
    if( index >= array->Length )
        return;
    --array->Length;

    // w przypadku gdy jest to ostatni element, nie przenoś nic...
    if( index == array->Length )
        return;

    // przenieś wszystkie elementy
    ptr1 = (unsigned char*)array->Items + array->ItemSize * index;
    ptr2 = ptr1 + array->ItemSize;

    memcpy( ptr1, ptr2, array->ItemSize * (array->Length - index) );
}

/* CZYSZCZENIE DANYCH */
/* ================================================================================================================== */

void ms_array_clear( void *aptr )
{
    MS_ARRAY *array = aptr;

    assert( array );
    assert( array->Items );

    array->Length = 0;
}

/* ================================================================================================================== */

void ms_array_clean( void *aptr )
{
    MS_ARRAY *array = aptr;

    assert( array );

    // zwolnij pamięć przydzieloną dla elementów
    free( array->Items );
    array->Items = NULL;

    // resetuj wszystkie wartości oprócz "Destroy"
    array->Capacity  = 0;
    array->ItemSize  = 0;
    array->Length    = 0;
    array->Factor    = 2.f;
    array->Increator = MSC_ArrayFunctions.IncMultiply;
}

/* ================================================================================================================== */

void ms_array_free( void *aptr )
{
    MS_ARRAY *array = aptr;

    if( !array )
        return;

    ms_array_clean( aptr );

    if( array->Destroy )
    {
        free( array );
        array = NULL;
    }
}

/* BAZA FUNKCJI POCHODNYCH */
/* ================================================================================================================== */

MS_ARRAY ms_array_return( size_t capacity )
{
    MS_ARRAY array;
    IGRET ms_array_init( &array, sizeof(*array.Items), capacity );
    return array;
}

/* ================================================================================================================== */

MS_ARRAY ms_array_copy_return( MS_ARRAY *array )
{
    MS_ARRAY copy;
    IGRET ms_array_copy( array, &copy );
    return copy;
}

/* ================================================================================================================== */

int ms_array_push( MS_ARRAY *array, void *item )
{
    assert( array );
    assert( array->Items );

    // sprawdź czy nowy element się zmieści
    if( array->Length >= array->Capacity )
    {
        int retval = !array->Increator
            ? array->Capacity + 1
            : 0;
        if( (retval = ms_array_realloc(array, retval)) )
            return retval;
    }
    array->Items[array->Length++] = item;
    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_array_insert( MS_ARRAY *array, size_t index, void *item )
{
    size_t iter;

    assert( array );
    assert( array->Items );

    // sprawdź czy indeks nie wyjdzie poza granice
    if( index > array->Length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

    // sprawdź czy nowy element się zmieści
    if( array->Length >= array->Capacity )
    {
        int retval = !array->Increator
            ? array->Capacity + 1
            : 0;
        if( (retval = ms_array_realloc(array, retval)) )
            return retval;
    }
    // przesuń elementy
    memmove( &array->Items[index + 1], &array->Items[index], array->ItemSize * (array->Length - index) ); 

    // dodaj nowy element
    array->Items[index] = item;
    ++array->Length;

    return MSEC_OK;
}

/* ================================================================================================================== */

void *ms_array_pop( MS_ARRAY *array )
{
    assert( array );
    assert( array->Items );

    if( !array->Length )
        return NULL;

    // zwróć ostatni element i zmniejsz długość
    return array->Items[--array->Length];
}

/* FUNKCJE WEWNĘTRZNE */
/* ================================================================================================================== */

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
