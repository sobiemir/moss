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

    // w pojemności uwzględnij znak NULL na końcu tekstu
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

    // kopiuj tekst wraz ze znakiem NULL
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

    // w tym przypadku zmienna len nie przechowuje ilości znaków, które zawiera ciąg
    // ale ilość bajtów na których jest on zbudowany
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

    // w odróżnieniu od zwykłego ciągu znaków, ten zawiera informacje o nich
    // gdyż jeden znak może rozciągać się na kilka bajtów
    str->MBInfo = ms_array_alloc( sizeof(MS_MBINFO), capacity );
    if( !str->MBInfo )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    // pobierz informacje o wielobajtowym ciągu znaków
    if( (retval = ms_string_mbsinfo(mbstr, str->MBInfo, len)) )
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
    str->Wide     = TRUE;
    str->Hashed   = FALSE;
    str->MBInfo   = NULL;

    // ilość bajtów jest równa rozmiarowi wchar_t pomnożonemu przez capacity
    str->WData = malloc( sizeof( *str->WData) * capacity );
    if( !str->WData )
        return
            ms_string_free( str ),
            MSEC_MEMORY_ALLOCATION;

    // kopiowanie ciągu wchar_t
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

    // powiększ ilość pamięci - w przypadku dokładnego zwiększania, wartość musi być podana w capacity
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

    // przydziel nową ilość pamięci */
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

/* ================================================================================================================== */

int ms_string_insert_cs( MS_STRING *str, size_t index, const char *cstr, size_t count )
{
    size_t length;

    assert( str );
    assert( str->CData );
    assert( cstr );

    // gdy ilość znaków nie została podana, oblicz ją
    if( !count )
        count = strlen( cstr );
    
    // ilość znaków jest zależna od typu tekstu
    // w przypadku typu wielobajtowego znaki mogą się składać z kilku bajtów...
    length = str->MBInfo
        ? str->MBInfo->Length
        : str->Length;

    if( index > length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
    
    // zwiększ rozmiar ciągu znaków
    if( str->Length + count + 1 > str->Capacity )
    {
        int ercode = ms_string_realloc_min( str, str->Length + count + 1 );
        if( ercode )
            return ercode;
    }
    else if( !count )
        SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

    // kopiuj w przypadku gdy jest to standardowy ciąg znaków
    if( !str->MBInfo && !str->Wide )
    {
        unsigned char *ptr = (unsigned char*)(str->CData + index);

        if( index != length )
            IGRET memmove( ptr + count, ptr, length - index );

        IGRET memcpy( ptr, cstr, count );

        str->Length            += count;
        str->CData[str->Length] = '\0';
    }
    // ciąg znaków zawierający znaki, które mogą być większe niż jeden bajt
    else if( str->MBInfo )
    {
        unsigned char *ptr;
        size_t         offset;

        // zwiększ rozmiar informacji o znakach gdy zajdzie taka potrzeba
        if( length + count > str->MBInfo->Capacity )
        {
            int ercode = ms_array_realloc_min( str->MBInfo, length + count );
            if( ercode )
                return ercode;
        }

        // wprowadzanie tekstu w inne miejsce niż na koniec
        if( index != length )
            ptr    = (unsigned char*)&ms_array_get(str->MBInfo, MS_MBINFO, index),
            offset = ((MS_MBINFO*)ptr)->Offset,

            // przesuwanie informacji o znakach oraz ciągu znaków
            IGRET memmove( ptr + count * sizeof(MS_MBINFO), ptr, (length - index) * sizeof(MS_MBINFO) ),
            ptr = (unsigned char*)(str->CData + offset),
            IGRET memmove( ptr + count, ptr, str->Length - offset );

        // gdy tekst wprowadzany jest na koniec, nic nie przesuwaj
        else
            ptr    = (unsigned char*)(str->CData + str->Length),
            offset = str->Length;

        IGRET memcpy( ptr, cstr, count );

        str->MBInfo->Length    += count;
        str->Length            += count;
        str->CData[str->Length] = '\0';

        // uzupełnij informacje dla nowych znaków
        length = count;
        while( length-- )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ) = (MS_MBINFO){
                .Offset = offset++,
                .Bytes  = 1u
            };

        // aktualizuj przesunięcie starych znaków
        while( index < str->MBInfo->Length )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += count;
    }
    // rozszerzony ciąg znaków wchar_t
    else
    {
        wchar_t *ptr = str->WData + index;

        if( index != length )
            IGRET memmove( ptr + count, ptr, (length - index) * sizeof *str->WData );

        str->Length += count;

        // po prostu dodawaj, zamieniając char na wchar_t
        while( count-- )
            *ptr++ = (wchar_t)*cstr++;

        str->WData[str->Length] = L'\0';
    }

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_insert_mbs( MS_STRING *str, size_t index, const char *mbstr, size_t count )
{
    size_t   length,
             offset;
    MS_ARRAY mbinfo;
    int      ercode;

    assert( str );
    assert( str->CData );
    assert( mbstr );

    // gdy ilość znaków nie została podana, oblicz ją
    if( !count )
        count = strlen( mbstr );
    
    // ilość znaków jest zależna od typu tekstu
    length = str->MBInfo
        ? str->MBInfo->Length
        : str->Length;

    if( index > length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

    // przydziel pamięć dla tablicy z informacją o poszczególnych znakach
    if( (ercode = ms_array_init(&mbinfo, sizeof(MS_MBINFO), count)) )
        return ercode;
    
    // pobierz informacje o znakach
    if( (ercode = ms_string_mbsinfo(mbstr, &mbinfo, count)) )
        return
            ms_array_free( &mbinfo ),
            ercode;
    
    // dobierz odpowiednią długość dodawanego tekstu
    offset = str->MBInfo
        ? count
        : mbinfo.Length;

    // zwiększ rozmiar ciągu znaków
    if( str->Length + offset + 1 > str->Capacity )
        if( (ercode = ms_string_realloc_min(str, str->Length + offset + 1)) )
            return
                ms_array_free( &mbinfo ),
                ercode;
        else;
    else if( !count )
    {
        ms_array_free( &mbinfo );
        SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
    }

    // gdy jest to zwykły ciąg znaków, skracaj wielobajtowe ciągi do jednobajtowego
    if( !str->MBInfo && !str->Wide )
    {
        unsigned char *ptr = (unsigned char*)(str->CData + index);

        if( index != length )
            IGRET memmove( ptr + offset, ptr, length - index );
        
        // zapisuj tylko pierwszy bit, pomijaj pozostałe...
        length = 0;
        while( length < offset )
            *ptr++ = *mbstr,
            mbstr += ms_array_getl( mbinfo, MS_MBINFO, length++ ).Bytes;

        str->Length            += mbinfo.Length;
        str->CData[str->Length] = '\0';
    }
    // ciąg znaków zawierający znaki, które mogą być większe niż jeden bajt
    else if( str->MBInfo )
    {
        unsigned char *ptr;

        // wstaw do tablicy elementy
        if( (ercode = ms_array_insert_values(str->MBInfo, index, mbinfo.Items, mbinfo.Length)) )
            return
                ms_array_free( &mbinfo ),
                ercode;
        
        if( index != length )
            offset = ms_array_get( str->MBInfo, MS_MBINFO, index + mbinfo.Length ).Offset,
            ptr    = (unsigned char*)(str->CData + offset),
            IGRET memmove( ptr + count, ptr, str->Length - offset );
        else
            ptr    = (unsigned char*)(str->CData + str->Length),
            offset = str->Length;

        IGRET memcpy( ptr, mbstr, count );

        str->Length += count;
        str->CData[str->Length] = '\0';

        // aktualizuj przesunięcie nowych znaków
        length = count;
        while( length-- )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += offset;

        // aktualizuj przesunięcie starych znaków
        while( index < str->MBInfo->Length )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += count;
    }
    // rozszerzony ciąg znaków wchar_t
    else
    {
        wchar_t *ptr = str->WData + index;

        if( index != length )
            IGRET memmove( ptr + offset, ptr, (length - index) * sizeof *str->WData );

        str->Length += offset;
        IGRET mbtowc( NULL, NULL, 0 );

        // po prostu dodawaj, zamieniając char na wchar_t
        // tutaj raczej funkcja musi zwrócić poprawny znak, gdyż było to sprawdzane wcześniej...
        while( offset-- )
            length = mbtowc( ptr++, mbstr, count ),
            count -= length,
            mbstr += length;

        str->WData[str->Length] = L'\0';
    }

    ms_array_free( &mbinfo );
    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_insert_wcs( MS_STRING *str, size_t index, const wchar_t *wstr, size_t count )
{
    size_t         length;
    unsigned char *ptr;

    assert( str );
    assert( str->CData );
    assert( wstr );

    // gdy ilość znaków nie została podana, oblicz ją
    if( !count )
        count = wcslen( wstr );

    // ciąg znaków o zmiennej wielkości
    if( str->MBInfo )
    {
        MS_ARRAY mbinfo;
        int      ercode;
        size_t   offset,
                 bytes;

        length = str->MBInfo->Length;
        if( index > length )
            SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
        
        // przydziel pamięć dla tablicy z informacją o poszczególnych znakach
        if( (ercode = ms_array_init(&mbinfo, sizeof(MS_MBINFO), count)) )
            return ercode;
        
        // pobierz informacje o znakach
        if( (ercode = ms_string_wcstombs_info(wstr, &mbinfo, count)) )
            return
                ms_array_free( &mbinfo ),
                ercode;

        // zwiększ pojemność
        if( str->Length + mbinfo.Length + 1 > str->Capacity )
            if( (ercode = ms_string_realloc_min(str, str->Length + mbinfo.Length + 1)) )
                return
                    ms_array_free( &mbinfo ),
                    ercode;

        if( str->Length + count + 1 > str->Capacity )
            if( (ercode = ms_string_realloc_min(str, str->Length + count + 1)) )
                return
                    ms_array_free( &mbinfo ),
                    ercode;

        // wstaw do tablicy elementy
        if( (ercode = ms_array_insert_values(str->MBInfo, index, mbinfo.Items, mbinfo.Length)) )
            return
                ms_array_free( &mbinfo ),
                ercode;
        
        // ilość bajtów
        bytes = ((MS_MBINFO*)mbinfo.Items)[mbinfo.Length - 1].Offset +
                ((MS_MBINFO*)mbinfo.Items)[mbinfo.Length - 1].Bytes;

        if( index != length )
            offset = ms_array_get( str->MBInfo, MS_MBINFO, index + mbinfo.Length ).Offset,
            ptr    = (unsigned char*)(str->CData + offset),
            IGRET memmove( ptr + bytes, ptr, str->Length - offset );
        else
            ptr    = (unsigned char*)(str->CData + str->Length),
            offset = str->Length;

        IGRET wctomb( NULL, 0 );
        str->Length += bytes;
        length = count;

        while( count-- )
            ptr += wctomb( (char*)ptr, *wstr++ );

        str->CData[str->Length] = '\0';

        // aktualizuj przesunięcie nowych znaków
        while( length-- )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += offset;

        // aktualizuj przesunięcie starych znaków
        while( index < str->MBInfo->Length )
            ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += bytes;

        ms_array_free( &mbinfo );
        return MSEC_OK;
    }

    length = str->Length;
    if( index > length )
        SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

    if( str->Length + count + 1 > str->Capacity )
    {
        int ercode = ms_string_realloc_min( str, str->Length + count + 1 );
        if( ercode )
            return ercode;
    }

    // ciąg znaków wchar_t
    if( str->Wide )
    {
        ptr = (unsigned char*)(str->WData + index);

        if( index != length )
            IGRET memmove( ptr + count * sizeof(wchar_t), ptr, (length - index) * sizeof(wchar_t) );
        IGRET memcpy( ptr, wstr, count * sizeof(wchar_t) );

        str->Length            += count;
        str->WData[str->Length] = L'\0';

        return MSEC_OK;
    }

    // ciąg znaków jednobajtowych
    ptr = (unsigned char*)(str->CData + index);

    if( index != length )
        IGRET memmove( ptr + count, ptr, length - index );
    
    str->Length += count;
    while( count-- )
        *ptr++ = (char)*wstr++;
    str->CData[str->Length] = '\0';

    return MSEC_OK;
}

/* ================================================================================================================== */

size_t ms_string_hash( MS_STRING *str )
{
    assert( str );
    assert( str->CData );

    if( str->Hashed )
        return str->Hash;

    // długość jest obliczona, więc funkcji dedykowanych nie potrzeba
    if( str->Wide )
        return ms_string_hash_byte( str->WData, str->Length * sizeof(wchar_t) );

    return ms_string_hash_byte( str->CData, str->Length );
}

/* ================================================================================================================== */

size_t ms_string_mbslen_wcs( const wchar_t *wstr, size_t count )
{
    char   data[MB_LEN_MAX];
    size_t length = 0;
    int    len;

    assert( wstr );

    // gdy długość ciągu nie została podana, oblicz go
    if( !count )
        count = wcslen( wstr );

    IGRET wctomb( NULL, 0 );

    // zamieniaj pojedyncze znaki i licz długość ciągu wyjściowego w bajtach
    // w przypadku błędu zwróć wartość 0
    while( count-- )
    {
        len = wctomb( data, *wstr++ );
        if( len < 0 )
            SETERRNOANDRETURNC( MSEC_INVALID_VALUE, 0 );
        else if( !len )
            break;
        length += len;
    }

    return length;
}

/* ================================================================================================================== */

size_t ms_string_mbslen( const char *mbstr, size_t bytes )
{
    size_t length = 0;
    int    len;

    assert( mbstr );

    if( !bytes )
        bytes = strlen( mbstr );

    IGRET mblen( NULL, 0 );

    // licz długość ciągu poprzez zliczanie pojedynczych znaków
    while( bytes )
    {
        len = mblen( mbstr, bytes );

        if( len < 0 )
            SETERRNOANDRETURNC( MSEC_INVALID_VALUE, 0 );
        else if( !len )
            break;

        length += len;
        bytes  -= bytes < len ? bytes : len;
        mbstr  += len;
    }

    return length;
}

/* ================================================================================================================== */

int ms_string_mbsinfo( const char *mbstr, MS_ARRAY *info, size_t bytes )
{
    MS_MBINFO mbinfo = { 0, 0 };
    int       len,
              ercode;

    assert( info );
    assert( info->Items );
    assert( mbstr );

    // oblicz długość ciągu znaków gdy nie została ona podana
    if( !bytes )
        bytes = strlen( mbstr );

    // resetuj stan przesunięcia "shift state"
    IGRET mblen( NULL, 0 );

    // oblicz rozmiar pojedynczych znaków
    while( bytes )
    {
        len = mblen( mbstr, bytes );

        if( len < 0 )
            SETERRNOANDRETURN( MSEC_INVALID_VALUE );
        else if( !len )
            break;

        mbinfo.Bytes = len;
        if( (ercode = ms_array_push_value(info, &mbinfo)) )
            return ercode;
        mbinfo.Offset += len;

        bytes -= bytes < len ? bytes : len;
        mbstr += len;
    }

    return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_wcstombs_info( const wchar_t *wstr, MS_ARRAY *info, size_t count )
{
    MS_MBINFO mbinfo = { 0, 0 };
    char      data[MB_LEN_MAX];
    int       len,
              ercode;

    assert( info );
    assert( info->Items );
    assert( wstr );

    // gdy długość ciągu nie została podana, oblicz go
    if( !count )
        count = wcslen( wstr );

    IGRET wctomb( NULL, 0 );

    // zamieniaj pojedyncze znaki i licz długość ciągu wyjściowego w bajtach
    // w przypadku błędu zwróć wartość 0
    while( count-- )
    {
        len = wctomb( data, *wstr++ );

        if( len < 0 )
            SETERRNOANDRETURN( MSEC_INVALID_VALUE );
        else if( !len )
            break;
            
        mbinfo.Bytes = len;
        if( (ercode = ms_array_push_value(info, &mbinfo)) )
            return ercode;
        mbinfo.Offset += len;
    }

    return MSEC_OK;
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
    
    // wyczyść informacje o znakach, gdy oczywiśćie istnieją
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

    // usuń informacje o szczegółach dotyczących znaków
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
