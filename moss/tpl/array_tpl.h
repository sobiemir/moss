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
 *  Template file for "Dynamic Array" module.
 *  See LICENSE file for copyright information.
 */

#ifndef __MSH_ARRAY_TEMPLATE__
#define __MSH_ARRAY_TEMPLATE__

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    STRUKTURA DLA TABLICY DEDYKOWANEJ DLA KONKRETNEGO TYPU
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#define __MST_ARRAY_STRUCT(_Postfix, _Type) \
    typedef struct MSS_ARRAY ## _Postfix \
    { \
        size_t Capacity; \
        size_t ItemSize; \
        size_t Length; \
        float Modifier; \
        bool Destroy; \
        _Type *Items; \
        size_t (*FuncIncrease)(size_t capacity, float modifier); \
    } \
    MS_ARRAY ## _Postfix;

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    FUNKCJA TWORZĄCA ZMIENNĄ LOKALNĄ TABLICY
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#define __MST_ARRAY_RETURN_H(_SPostfix, _FPostfix) \
    MS_ARRAY ## _SPostfix ms_array_return ## _FPostfix \
        ( size_t capacity );

#define __MST_ARRAY_RETURN(_SPostfix, _FPostfix) \
    MS_ARRAY ## _SPostfix ms_array_return ## _FPostfix \
        ( size_t capacity ) \
    { \
        MS_ARRAY ## _SPostfix array; \
        IGRET ms_array_init( &array, sizeof(*array.Items), capacity ); \
        return array; \
    }

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    FUNKCJA ZWRACAJĄCA KOPIĘ LOKALNĄ TABLICY
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#define __MST_ARRAY_COPY_RETURN_H(_SPostfix, _FPostfix) \
    MS_ARRAY ## _SPostfix ms_array_copy_return ## _FPostfix \
        ( const MS_ARRAY ## _SPostfix *array );

#define __MST_ARRAY_COPY_RETURN(_SPostfix, _FPostfix) \
    MS_ARRAY ## _SPostfix ms_array_copy_return ## _FPostfix \
        ( const MS_ARRAY ## _SPostfix *array ) \
    { \
        MS_ARRAY ## _SPostfix copy; \
        IGRET ms_array_copy( &copy, array ); \
        return copy; \
    }

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    FUNKCJA POZWALAJĄCA NA DODANIE ELEMENTU DO TABLICY NA KONIEC
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#define __MST_ARRAY_PUSH_H(_Type, _SPostfix, _FPostfix) \
    int ms_array_push ## _FPostfix \
        ( MS_ARRAY ## _SPostfix *array, _Type item );

#define __MST_ARRAY_PUSH(_Type, _SPostfix, _FPostfix) \
    int ms_array_push ## _FPostfix \
        ( MS_ARRAY ## _SPostfix *array, _Type item ) \
    { \
        assert( array ); \
        assert( array->Items ); \
        return ms_array_insert ## _FPostfix( array, array->Length, item ); \
    }

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    FUNKCJA POZWALAJĄCA NA DODANIE ELEMENTU DO TABLICY W WYBRANE MIEJSCE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

#define __MST_ARRAY_INSERT_H(_Type, _SPostfix, _FPostfix) \
    int ms_array_insert ## _FPostfix \
        ( MS_ARRAY ## _SPostfix *array, size_t index, _Type item );

#define __MST_ARRAY_INSERT(_Type, _SPostfix, _FPostfix) \
    int ms_array_insert ## _FPostfix \
        ( MS_ARRAY ## _SPostfix *array, size_t index, _Type item ) \
    { \
        assert( array ); \
        assert( array->Items ); \
        if( index > array->Length ) \
            SETERRNOANDRETURN( MSEC_OUT_OF_RANGE ); \
        if( array->Length >= array->Capacity ) \
        { \
            int ercode = !array->FuncIncrease \
                ? array->Capacity + 1 \
                : 0; \
            if( (ercode = ms_array_realloc(array, ercode)) ) \
                return ercode; \
        } \
        if( index != array->Length ) \
            IGRET memmove( &array->Items[index + 1], &array->Items[index], \
                array->ItemSize * (array->Length - index) ); \
        array->Items[index] = item; \
        ++array->Length; \
        return MSEC_OK; \
    }

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
    MAKRA TWORZĄCE NAGŁÓWKI I FUNKCJE
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Makro tworzące nagłówki funkcji z funkcji bazowych.
 * Dodatkowo tworzy strukturę tablicy operującą na podanym typie.
 * Makro tworzy tylko nagłówki, ciała funkcji tworzy makro MST_ARRAY_BODY.
 *
 * @param type type Typ, który ma przyjmować tablica.
 * @param literal spfix Przyrostek dla struktury tablicy.
 * @param literal fpfix Przyrostek dla funkcji operujących na strukturze.
 */
#define MST_ARRAY_HEADER(type, spfix, fpfix) \
    __MST_ARRAY_STRUCT( spfix, type ); \
    __MST_ARRAY_COPY_RETURN_H( spfix, fpfix ); \
    __MST_ARRAY_INSERT_H( type, spfix, fpfix ); \
    __MST_ARRAY_PUSH_H( type, spfix, fpfix ); \
    __MST_ARRAY_RETURN_H( spfix, fpfix )

/**
 * Makro tworzące ciała funkcji z funkcji bazowych.
 * Warto przed tym wywołać makro tworzące nagłówki funkcji.
 *
 * @param type type Typ, który ma przyjmować tablica.
 * @param literal spfix Przyrostek dla struktury tablicy.
 * @param literal fpfix Przyrostek dla funkcji operujących na strukturze.
 */
#define MST_ARRAY_BODY(type, spfix, fpfix) \
    __MST_ARRAY_COPY_RETURN( spfix, fpfix ); \
    __MST_ARRAY_INSERT( type, spfix, fpfix ); \
    __MST_ARRAY_PUSH( type, spfix, fpfix ); \
    __MST_ARRAY_RETURN( spfix, fpfix )

#endif
