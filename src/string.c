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

#include <moss/string.h>

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_cs( const char *cstr, size_t capacity )
{
	MS_STRING *str;

	assert( cstr );

	if( (str = malloc(sizeof *str)) == NULL )
		return NULL;

	if( ms_string_init_cs(str, cstr, capacity) )
		return
			free( str ),
			NULL;

	str->Destroy = TRUE;
	return str;
}

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_mbs( const char *mbstr, size_t capacity )
{
	MS_STRING *str;
	
	assert( mbstr );

	if( (str = malloc(sizeof *str)) == NULL )
		return NULL;

	if( ms_string_init_mbs(str, mbstr, capacity) )
		return
			free( str ),
			NULL;

	str->Destroy = TRUE;
	return str;
}

/* ================================================================================================================== */

MS_STRING *ms_string_alloc_wcs( const wchar_t *wstr, size_t capacity )
{
	MS_STRING *str;
	
	assert( wstr );

	if( (str = malloc(sizeof *str)) == NULL )
		return NULL;

	if( ms_string_init_wcs(str, wstr, capacity) )
		return
			free( str ),
			NULL;

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

	str->Data.Char = malloc( sizeof *str->Data.Char * capacity );
	if( !str->Data.Char )
		return
			ms_string_free( str ),
			MSEC_MEMORY_ALLOCATION;

	/* kopiuj tekst wraz ze znakiem NULL */
	IGRET memcpy( str->Data.Char, cstr, len + 1 );

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
	 * ale ilość bajtów na których jest on zbudowany */
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

	str->Data.Char = malloc( sizeof *str->Data.Char * capacity );
	if( !str->Data.Char )
		return
			ms_string_free( str ),
			MSEC_MEMORY_ALLOCATION;

	IGRET memcpy( str->Data.Char, mbstr, len + 1 );

	/* w odróżnieniu od zwykłego ciągu znaków, ten zawiera informacje o nich
	 * gdyż jeden znak może rozciągać się na kilka bajtów */
	str->MBInfo = ms_array_alloc( sizeof(MS_MBINFO), capacity );
	if( !str->MBInfo )
		return
			ms_string_free( str ),
			MSEC_MEMORY_ALLOCATION;

	/* pobierz informacje o wielobajtowym ciągu znaków */
	if( (retval = ms_string_info_mbs(mbstr, len, str->MBInfo)) != 0 )
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

	/* ilość bajtów jest równa rozmiarowi wchar_t pomnożonemu przez capacity */
	str->Data.Wide = malloc( sizeof( *str->Data.Wide) * capacity );
	if( !str->Data.Wide )
		return
			ms_string_free( str ),
			MSEC_MEMORY_ALLOCATION;

	/* kopiowanie ciągu wchar_t */
	IGRET memcpy( str->Data.Char, wstr, (len + 1) * sizeof *str->Data.Wide );

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
	assert( str->Data.Char );

	/* powiększ ilość pamięci - w przypadku dokładnego zwiększania, wartość musi być podana w capacity */
	if( !capacity )
	{
		if( !str->FuncIncrease )
			SETERRNOANDRETURN( MSEC_INVALID_VALUE );

		capacity = str->FuncIncrease( str->Capacity, str->Modifier );
	
		if( capacity <= str->Capacity )
			capacity = str->Capacity + 1;
	}
	/* aby przydzielić taką ilość pamięci elementy muszą najpierw zostać usunięte
	   funkcja musi trzymać dodatkowe miejsce na znak NULL */
	else if( str->Length >= capacity )
		SETERRNOANDRETURN( MSEC_DATA_OVERFLOW );

	/* przydziel nową ilość pamięci */
	if( (tmp = realloc(str->Data.Char, capacity * (str->Wide ? sizeof(wchar_t) : 1))) == NULL )
		return MSEC_MEMORY_ALLOCATION;

	str->Data.Char    = tmp;
	str->Capacity = capacity;

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_realloc_min( MS_STRING *str, size_t min )
{
	size_t capacity, oldcap;

	assert( str );
	assert( str->Data.Char );

	capacity = str->Capacity;

	/* osiągnij co najmniej wartość minimalną, z czego nowy rozmiar nie może być mniejszy niż stary */
	while( min > capacity )
	{
		oldcap   = capacity;
		capacity = !str->FuncIncrease
			? min
			: str->FuncIncrease( oldcap, str->Modifier );
		if( capacity <= oldcap )
			capacity = oldcap + 1;
	}

	/* przydziel nową ilość pamięci gdy pojemność się różni */
	if( capacity != str->Capacity )
		return ms_string_realloc( str, capacity );

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_insert_cs( MS_STRING *str, size_t index, const char *cstr, size_t count )
{
	size_t length;

	assert( str );
	assert( str->Data.Char );
	assert( cstr );

	/* gdy ilość znaków nie została podana, oblicz ją */
	if( !count )
		count = strlen( cstr );
	
	/* ilość znaków jest zależna od typu tekstu
	   w przypadku typu wielobajtowego znaki mogą się składać z kilku bajtów... */
	length = str->MBInfo
		? str->MBInfo->Length
		: str->Length;

	if( index > length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
	
	/* zwiększ rozmiar ciągu znaków */
	if( str->Length + count + 1 > str->Capacity )
	{
		int ercode = ms_string_realloc_min( str, str->Length + count + 1 );
		if( ercode )
			return ercode;
	}
	else if( !count )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

	/* kopiuj w przypadku gdy jest to standardowy ciąg znaków */
	if( !str->MBInfo && !str->Wide )
	{
		unsigned char *ptr = (unsigned char*)(str->Data.Char + index);

		if( index != length )
			IGRET memmove( ptr + count, ptr, length - index );

		IGRET memcpy( ptr, cstr, count );

		str->Length            += count;
		str->Data.Char[str->Length] = '\0';
	}
	/* ciąg znaków zawierający znaki, które mogą być większe niż jeden bajt */
	else if( str->MBInfo )
	{
		unsigned char *ptr;
		size_t         offset;
		MS_MBINFO     *elem;

		/* zwiększ rozmiar informacji o znakach gdy zajdzie taka potrzeba */
		if( length + count > str->MBInfo->Capacity )
		{
			int ercode = ms_array_realloc_min( str->MBInfo, length + count );
			if( ercode )
				return ercode;
		}

		/* wprowadzanie tekstu w inne miejsce niż na koniec */
		if( index != length )
			ptr    = (unsigned char*)&ms_array_get(str->MBInfo, MS_MBINFO, index),
			offset = ((MS_MBINFO*)ptr)->Offset,

			/* przesuwanie informacji o znakach oraz ciągu znaków */
			IGRET memmove( ptr + count * sizeof(MS_MBINFO), ptr, (length - index) * sizeof(MS_MBINFO) ),
			ptr = (unsigned char*)(str->Data.Char + offset),
			IGRET memmove( ptr + count, ptr, str->Length - offset );

		/* gdy tekst wprowadzany jest na koniec, nic nie przesuwaj */
		else
			ptr    = (unsigned char*)(str->Data.Char + str->Length),
			offset = str->Length;

		IGRET memcpy( ptr, cstr, count );

		str->MBInfo->Length    += count;
		str->Length            += count;
		str->Data.Char[str->Length] = '\0';

		/* uzupełnij informacje dla nowych znaków */
		length = count;
		while( length-- )
			elem = &ms_array_get( str->MBInfo, MS_MBINFO, index++ ),
			elem->Offset = offset++,
			elem->Bytes  = 1u;

		/* aktualizuj przesunięcie starych znaków */
		while( index < str->MBInfo->Length )
			ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += count;
	}
	/* rozszerzony ciąg znaków wchar_t */
	else
	{
		wchar_t *ptr = str->Data.Wide + index;

		if( index != length )
			IGRET memmove( ptr + count, ptr, (length - index) * sizeof *str->Data.Wide );

		str->Length += count;

		/* po prostu dodawaj, zamieniając char na wchar_t */
		while( count-- )
			*ptr++ = (wchar_t)*cstr++;

		str->Data.Wide[str->Length] = L'\0';
	}

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_insert_mbs( MS_STRING *str, size_t index, const char *mbstr, size_t count )
{
	size_t   length, offset;
	MS_ARRAY mbinfo;
	int      ercode;

	assert( str );
	assert( str->Data.Char );
	assert( mbstr );

	/* gdy ilość znaków nie została podana, oblicz ją */
	if( !count )
		count = strlen( mbstr );
	
	/* ilość znaków jest zależna od typu tekstu */
	length = str->MBInfo
		? str->MBInfo->Length
		: str->Length;

	if( index > length )
		SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );

	/* przydziel pamięć dla tablicy z informacją o poszczególnych znakach */
	if( (ercode = ms_array_init(&mbinfo, sizeof(MS_MBINFO), count)) != 0 )
		return ercode;
	
	/* pobierz informacje o znakach */
	if( (ercode = ms_string_info_mbs(mbstr, count, &mbinfo)) != 0 )
		return
			ms_array_free( &mbinfo ),
			ercode;
	
	/* dobierz odpowiednią długość dodawanego tekstu */
	offset = str->MBInfo
		? count
		: mbinfo.Length;

	/* zwiększ rozmiar ciągu znaków */
	if( str->Length + offset + 1 > str->Capacity )
		if( (ercode = ms_string_realloc_min(str, str->Length + offset + 1)) != 0 )
			return
				ms_array_free( &mbinfo ),
				ercode;
		else;
	else if( !count )
	{
		ms_array_free( &mbinfo );
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );
	}

	/* gdy jest to zwykły ciąg znaków, skracaj wielobajtowe ciągi do jednobajtowego */
	if( !str->MBInfo && !str->Wide )
	{
		unsigned char *ptr = (unsigned char*)(str->Data.Char + index);

		if( index != length )
			IGRET memmove( ptr + offset, ptr, length - index );
		
		/* zapisuj tylko pierwszy bit, pomijaj pozostałe... */
		length = 0;
		while( length < offset )
			*ptr++ = *mbstr,
			mbstr += ms_array_getl( mbinfo, MS_MBINFO, length++ ).Bytes;

		str->Length += mbinfo.Length;
		str->Data.Char[str->Length] = '\0';
	}
	/* char - multibyte */
	else if( str->MBInfo )
	{
		unsigned char *ptr;

		/* wstaw do tablicy elementy */
		if( (ercode = ms_array_insert_values(str->MBInfo, index, mbinfo.Items, mbinfo.Length)) != 0 )
			return
				ms_array_free( &mbinfo ),
				ercode;

		if( index != length )
			offset = ms_array_get( str->MBInfo, MS_MBINFO, index + mbinfo.Length ).Offset,
			ptr    = (unsigned char*)(str->Data.Char + offset),
			IGRET memmove( ptr + count, ptr, str->Length - offset );
		else
			ptr    = (unsigned char*)(str->Data.Char + str->Length),
			offset = str->Length;

		IGRET memcpy( ptr, mbstr, count );

		str->Length += count;
		str->Data.Char[str->Length] = '\0';

		/* aktualizuj przesunięcie nowych znaków */
		length = mbinfo.Length;
		while( length-- )
			ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += offset;

		/* aktualizuj przesunięcie starych znaków */
		while( index < str->MBInfo->Length )
			ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += count;
	}
	/* wchar_t */
	else
	{
		wchar_t *ptr = str->Data.Wide + index;

		if( index != length )
			IGRET memmove( ptr + offset, ptr, (length - index) * sizeof *str->Data.Wide );

		str->Length += offset;
		IGRET mbtowc( NULL, NULL, 0 );

		// po prostu dodawaj, zamieniając char na wchar_t
		// tutaj raczej funkcja musi zwrócić poprawny znak, gdyż było to sprawdzane wcześniej...
		while( offset-- )
			length = mbtowc( ptr++, mbstr, count ),
			count -= length,
			mbstr += length;

		str->Data.Wide[str->Length] = L'\0';
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
	assert( str->Data.Char );
	assert( wstr );

	/* gdy ilość znaków nie została podana, oblicz ją */
	if( !count )
		count = wcslen( wstr );
	if( !count )
		SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

	/* ciąg znaków o zmiennej wielkości */
	if( str->MBInfo )
	{
		MS_ARRAY mbinfo;
		int      ercode;
		size_t   offset;
		size_t   bytes;

		length = str->MBInfo->Length;
		if( index > length )
			SETERRNOANDRETURN( MSEC_OUT_OF_RANGE );
		
		/* przydziel pamięć dla tablicy z informacją o poszczególnych znakach */
		if( (ercode = ms_array_init(&mbinfo, sizeof(MS_MBINFO), count)) != 0 )
			return ercode;
		
		/* pobierz informacje o znakach */
		if( (ercode = ms_string_info_wcstombs(wstr, count, &mbinfo)) != 0 )
			return
				ms_array_free( &mbinfo ),
				ercode;

		/* zwiększ pojemność */
		if( str->Length + mbinfo.Length + 1 > str->Capacity )
			if( (ercode = ms_string_realloc_min(str, str->Length + mbinfo.Length + 1)) != 0 )
				return
					ms_array_free( &mbinfo ),
					ercode;

		if( str->Length + count + 1 > str->Capacity )
			if( (ercode = ms_string_realloc_min(str, str->Length + count + 1)) != 0 )
				return
					ms_array_free( &mbinfo ),
					ercode;

		/* wstaw do tablicy elementy */
		if( (ercode = ms_array_insert_values(str->MBInfo, index, mbinfo.Items, mbinfo.Length)) != 0 )
			return
				ms_array_free( &mbinfo ),
				ercode;
		
		/* ilość bajtów */
		bytes = ((MS_MBINFO*)mbinfo.Items)[mbinfo.Length - 1].Offset +
		        ((MS_MBINFO*)mbinfo.Items)[mbinfo.Length - 1].Bytes;

		if( index != length )
			offset = ms_array_get( str->MBInfo, MS_MBINFO, index + mbinfo.Length ).Offset,
			ptr    = (unsigned char*)(str->Data.Char + offset),
			IGRET memmove( ptr + bytes, ptr, str->Length - offset );
		else
			ptr    = (unsigned char*)(str->Data.Char + str->Length),
			offset = str->Length;

		IGRET wctomb( NULL, 0 );
		str->Length += bytes;
		length = count;

		while( count-- )
			ptr += wctomb( (char*)ptr, *wstr++ );

		str->Data.Char[str->Length] = '\0';

		/* aktualizuj przesunięcie nowych znaków */
		while( length-- )
			ms_array_get( str->MBInfo, MS_MBINFO, index++ ).Offset += offset;

		/* aktualizuj przesunięcie starych znaków */
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

	/* wchar_t */
	if( str->Wide )
	{
		ptr = (unsigned char*)(str->Data.Wide + index);

		if( index != length )
			IGRET memmove( ptr + count * sizeof(wchar_t), ptr, (length - index) * sizeof(wchar_t) );
		IGRET memcpy( ptr, wstr, count * sizeof(wchar_t) );

		str->Length            += count;
		str->Data.Wide[str->Length] = L'\0';

		return MSEC_OK;
	}

	/* ciąg znaków jednobajtowych */
	ptr = (unsigned char*)(str->Data.Char + index);

	if( index != length )
		IGRET memmove( ptr + count, ptr, length - index );
	
	str->Length += count;
	while( count-- )
		*ptr++ = (char)*wstr++;
	str->Data.Char[str->Length] = '\0';

	return MSEC_OK;
}

/* ================================================================================================================== */

// size_t ms_string_hash( MS_STRING *str )
// {
// 	assert( str );
// 	assert( str->Data.Char );

// 	if( str->Hashed )
// 		return str->Hash;

// 	/* długość jest obliczona, więc funkcji dedykowanych nie potrzeba */
// 	if( str->Wide )
// 		return ms_string_hash_byte( str->Data.Wide, str->Length * sizeof(wchar_t) );

// 	return ms_string_hash_byte( str->Data.Char, str->Length );
// }

/* ================================================================================================================== */

size_t ms_string_length_wcstombs( const wchar_t *wstr, size_t count )
{
	char   data[MB_LEN_MAX];
	size_t length = 0;
	int    len;
	wint_t chr;

	assert( wstr );

	/* gdy długość ciągu nie została podana, oblicz go */
	if( !count )
		count = wcslen( wstr );

	IGRET wctomb( NULL, 0 );

	/* zamieniaj pojedyncze znaki i licz długość ciągu wyjściowego w bajtach
	 * w przypadku błędu zwróć wartość 0 */
	while( count-- )
	{
		chr = *wstr++;
		len = wctomb( data, chr );
		if( len < 0 )
			SETERRNOANDRETURNC( MSEC_INVALID_VALUE, 0 );
		else if( !len || chr == L'\0' )
			break;
		length += len;
	}

	return length;
}

/* ================================================================================================================== */

size_t ms_string_length_mbs( const char *mbstr, size_t bytes )
{
	size_t length = 0;
	int    len;

	assert( mbstr );

	if( !bytes )
		bytes = strlen( mbstr );

	IGRET mblen( NULL, 0 );

	/* licz długość ciągu poprzez zliczanie pojedynczych znaków */
	while( bytes )
	{
		len = mblen( mbstr, bytes );

		if( len < 0 )
			SETERRNOANDRETURNC( MSEC_INVALID_VALUE, 0 );
		else if( !len )
			break;

		length++;
		bytes -= bytes < (size_t)len ? bytes : len;
		mbstr += len;
	}

	return length;
}

/* ================================================================================================================== */

int ms_string_info_mbs( const char *mbstr, size_t bytes, MS_ARRAY *info )
{
	MS_MBINFO mbinfo = { 0, 0 };
	int       len;
	int       ercode;

	assert( info );
	assert( info->Items );
	assert( mbstr );

	/* oblicz długość ciągu znaków gdy nie została ona podana */
	if( !bytes )
		bytes = strlen( mbstr );

	/* resetuj stan przesunięcia "shift state" */
	IGRET mblen( NULL, 0 );

	/* oblicz rozmiar pojedynczych znaków */
	while( bytes )
	{
		len = mblen( mbstr, bytes );

		if( len < 0 )
			SETERRNOANDRETURN( MSEC_INVALID_VALUE );
		else if( !len )
			break;

		mbinfo.Bytes = (char)len;
		if( (ercode = ms_array_push_value(info, &mbinfo)) != 0 )
			return ercode;
		mbinfo.Offset += len;

		bytes -= bytes < (size_t)len ? bytes : len;
		mbstr += len;
	}

	return MSEC_OK;
}

/* ================================================================================================================== */

int ms_string_info_wcstombs( const wchar_t *wstr, size_t count, MS_ARRAY *info )
{
	MS_MBINFO mbinfo = { 0, 0 };
	char      data[MB_LEN_MAX];
	int       len, ercode;
	wint_t    chr;

	assert( info );
	assert( info->Items );
	assert( wstr );

	/* gdy długość ciągu nie została podana, oblicz go */
	if( !count )
		count = wcslen( wstr );

	IGRET wctomb( NULL, 0 );

	/* zamieniaj pojedyncze znaki i licz długość ciągu wyjściowego w bajtach
	 * w przypadku błędu zwróć wartość 0 */
	while( count-- )
	{
		chr = *wstr++;
		len = wctomb( data, chr );

		if( len < 0 )
			SETERRNOANDRETURN( MSEC_INVALID_VALUE );
		else if( !len || chr == L'\0' )
			break;
			
		mbinfo.Bytes = (char)len;
		if( (ercode = ms_array_push_value(info, &mbinfo)) != 0 )
			return ercode;
		mbinfo.Offset += len;
	}

	return MSEC_OK;
}

/* ================================================================================================================== */

void ms_string_clear( MS_STRING *str )
{
	assert( str );
	assert( str->Data.Char );

	str->Length = 0;

	if( str->Wide )
		str->Data.Wide[0] = L'\0';
	else
		str->Data.Char[0] = '\0';
	
	/* wyczyść informacje o znakach, gdy oczywiśćie istnieją */
	if( str->MBInfo )
		ms_array_clear( str->MBInfo );
}

/* ================================================================================================================== */

void ms_string_free( MS_STRING *str )
{
	if( !str )
		return;

	free( str->Data.Char );
	str->Data.Char = NULL;

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

	if( str->Destroy )
		free( str );
}
