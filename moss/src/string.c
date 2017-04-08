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
 *  Source file for "String" module.
 *  See LICENSE file for copyright information.
 */

#include "../inc/string.h"

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_cs( const char *cstr, size_t capacity )
{
    MS_STRING *str;

    assert( cstr );

    if( !(str = malloc(sizeof *str)) )
        return NULL;

    if( ms_string_init_cs(str, cstr, capacity) )
        return NULL;

    str->Destroy = TRUE;
    return str;
}

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_mbs( const char *mbstr, size_t capacity )
{
    MS_STRING *str;
    
    assert( mbstr );

    if( !(str = malloc(sizeof *str)) )
        return NULL;

    if( ms_string_init_mbs(str, mbstr, capacity) )
        return NULL;

    str->Destroy = TRUE;
    return str;
}

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_wcs( const wchar_t *wstr, size_t capacity )
{
    MS_STRING *str;
    
    assert( wstr );

    if( !(str = malloc(sizeof *str)) )
        return NULL;

    if( ms_string_init_wcs(str, wstr, capacity) )
        return NULL;

    str->Destroy = TRUE;
    return str;
}

/* ================================================================================================================== */

int ms_string_init_cs( MS_STRING *str, const char *cstr, size_t capacity )
{
    size_t len;

    assert( str );
    assert( cstr );

    len = strlen( cstr );

    /* w pojemności uwzględnij znak NULL na końcu tekstu */
    if( len >= capacity )
        capacity = len + 1;

    str->Length   = len;
    str->Capacity = capacity;
    str->Hash     = 0;
    str->Modifier = 2.f;
    str->Destroy  = FALSE;
    str->Wide     = FALSE;
    str->Hashed   = FALSE;
    str->MBInfo   = NULL;

    str->CData = malloc( sizeof *str->CData * capacity );
    if( !str->CData )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    /* kopiuj tekst wraz ze znakiem NULL */
    IGRET memcpy( str->CData, cstr, len + 1 );

    str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_init_mbs( MS_STRING *str, const char *mbstr, size_t capacity )
{
    size_t len;
    int    retval;

    assert( str );
    assert( mbstr );

    len = strlen( mbstr );

    /* w tym przypadku zmienna len nie przechowuje ilości znaków, które zawiera ciąg
       ale ilość bajtów na których jest on zbudowany */
    if( len >= capacity )
        capacity = len + 1;

    str->Length   = len;
    str->Capacity = capacity;
    str->Hash     = 0;
    str->Modifier = 2.f;
    str->Destroy  = FALSE;
    str->Wide     = FALSE;
    str->Hashed   = FALSE;
    str->MBInfo   = NULL;

    str->CData = malloc( sizeof *str->CData * capacity );
    if( !str->CData )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    IGRET memcpy( str->CData, mbstr, len + 1 );

    /* w odróżnieniu od zwykłego ciągu znaków, ten zawiera informacje o nich,
       gdyż jeden znak może rozciągać się na kilka bajtów */
    str->MBInfo = ms_array_alloc( sizeof(size_t), capacity );
    if( !str->MBInfo )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    /* pobierz informacje o wielobajtowym ciągu znaków */
    if( (retval = ms_string_mbs_info(str->MBInfo, mbstr, len)) )
        return
            ms_string_free( str ),
            retval;

    str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_init_wcs( MS_STRING *str, const wchar_t *wstr, size_t capacity )
{
    size_t len;

    assert( str );
    assert( wstr );

    len = wcslen( wstr );

    if( len >= capacity )
        capacity = len + 1;

    str->Length   = len;
    str->Capacity = capacity;
    str->Hash     = 0;
    str->Modifier = 2.f;
    str->Destroy  = FALSE;
    str->Wide     = FALSE;
    str->Hashed   = FALSE;
    str->MBInfo   = NULL;

    /* ilość bajtów jest równa rozmiarowi wchar_t pomnożonemu przez capacity */
    str->WData = malloc( sizeof( *str->WData) * capacity );
    if( !str->WData )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    /* kopiowanie ciągu wchar_t */
    IGRET memcpy( str->CData, wstr, (len + 1) * sizeof *str->WData );

    str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
    return MSEC_OK;
}

/* ================================================================================================================== */

MS_STRING ms_string_return_cs( const char *cstr, size_t capacity )
{
    MS_STRING str;
    assert( cstr );
    IGRET ms_string_init_cs( &str, cstr, capacity );
    return str;
}

/* ================================================================================================================== */

MS_STRING ms_string_return_mbs( const char *mbstr, size_t capacity )
{
    MS_STRING str;
    assert( mbstr );
    IGRET ms_string_init_mbs( &str, mbstr, capacity );
    return str;
}

/* ================================================================================================================== */

MS_STRING ms_string_return_wcs( const wchar_t *wstr, size_t capacity )
{
    MS_STRING str;
    assert( wstr );
    IGRET ms_string_init_wcs( &str, wstr, capacity );
    return str;
}

/* ================================================================================================================== */

int ms_string_realloc( MS_STRING *str, size_t capacity )
{
    void *tmp;

    assert( str );
    assert( str->CData );

    // // powiększ ilość pamięci - w przypadku dokładnego zwiększania, wartość musi być podana w capacity
    if( !capacity )
    {
        if( !str->FuncIncrease )
            SETERRNOANDRETURN( MSEC_INVALID_VALUE );

        capacity = str->FuncIncrease( str->Capacity, str->Modifier );
    
        if( capacity <= str->Capacity )
            capacity = str->Capacity + 1;
    }
    // aby przydzielić taką ilość pamięci elementy muszą najpierw zostać usunięte
    else if( str->Length > capacity )
        SETERRNOANDRETURN( MSEC_DATA_OVERFLOW );

    // przydziel nową ilość pamięci
    if( !(tmp = realloc(str->CData, capacity * (str->Wide ? sizeof(wchar_t) : 1))) )
        return MSEC_MEMORY_ALLOCATION;

    str->CData    = tmp;
    str->Capacity = capacity;

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_realloc_min( MS_STRING *str, size_t min )
{
    size_t capacity,
           oldcap;

    assert( str );
    assert( str->CData );

    capacity = str->Capacity;

    // osiągnij co najmniej wartość minimalną, z czego nowy rozmiar nie może być mniejszy niż stary
    while( min > capacity )
    {
        oldcap   = capacity;
        capacity = !str->FuncIncrease
            ? min
            : str->FuncIncrease( oldcap, str->Modifier );
        if( capacity <= oldcap )
            capacity = oldcap + 1;
    }

    // przydziel nową ilość pamięci gdy pojemność się różni
    if( capacity != str->Capacity )
        return ms_string_realloc( str, capacity );

    return MSEC_OK;
}

// ========================================================================================================================================

size_t ms_string_hash( MS_STRING *str )
{
    assert( str );
    assert( str->CData );

    if( str->Hashed )
        return str->Hash;

    /* długość jest obliczona, więc funkcji dedykowanych nie potrzeba */
    if( str->Wide )
        return ms_string_hash_byte( str->WData, str->Length * sizeof(wchar_t) );

    return ms_string_hash_byte( str->CData, str->Length );
}

/* ================================================================================================================== */

void ms_string_clear( MS_STRING *str )
{
    assert( str );
    assert( str->CData );

    if( str->Capacity == 0 )
        return;

    str->Length = 0;

    if( str->Wide )
        str->WData[0] = L'\0';
    else
        str->CData[0] = '\0';
    
    /* wyczyść informacje o znakach, gdy oczywiśćie istnieją */
    if( str->MBInfo )
        ms_array_clear( str->MBInfo );
}

/* ================================================================================================================== */

void ms_string_clean( MS_STRING *str )
{
    assert( str );

    free( str->CData );
    str->CData = NULL;

    str->Length   = 0;
    str->Capacity = 0;
    str->Hash     = 0;
    str->Modifier = 2.f;
    str->Wide     = FALSE;
    str->Hashed   = FALSE;

    /* usuń informacje o szczegółach dotyczących znaków */
    if( str->MBInfo )
        ms_array_free( str->MBInfo ),
        str->MBInfo = NULL;

    str->FuncIncrease = MSC_ArrayFunctions.IncMultiply;
}

/* ================================================================================================================== */

void ms_string_free( MS_STRING *str )
{
    if( !str )
        return;

    ms_string_clean( str );

    if( str->Destroy )
        free( str );
}

/* ================================================================================================================== */

int ms_string_mbs_info( MS_ARRAY *info, const char *mbs, size_t bytes )
{
    int len,
        retval;

    assert( info );
    assert( info->Items );

    /* oblicz długość ciągu znaków gdy nie została ona podana */
    if( bytes == 0 )
        bytes = strlen( mbs );

    /* resetuj stan przesunięcia "shift state" */
    mblen( NULL, 0 );

    /* oblicz rozmiar pojedynczych znaków */
    while( bytes > 0 )
    {
        len = mblen( mbs, bytes );

        if( len < 0 )
            return MSEC_INVALID_VALUE;
        else if( len == 0 )
            break;
        if( (retval = ms_array_push_value(info, &len)) )
            return retval;
        
        bytes -= len;
        mbs   += len;
    }

    return MSEC_OK;
}
