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
 * Header file for "String" module...
 */

#ifndef __MSH_STRING__
#define __MSH_STRING__

#include <stdlib.h>

#include "configuration.h"
// #include "../tpl/string_tpl.h"

 typedef struct MSS_STRING
{
	union
	{
		char    *AData;
		wchar_t *WData;
	};
	size_t Length;
	size_t Capacity;
	size_t Hash;
	bool   Destroy;
	bool   ZeroFill;
	bool   Wide;
	bool   Hashed;
}
MS_STRING;

size_t ms_string_hash( MS_STRING *sstr );
MS_STRING *ms_string_alloc_fmbs( const char *mbs, size_t capacity, bool zerofill );
MS_STRING *ms_string_alloc_fwcs( const wchar_t *wcs, size_t capacity, bool zerofill );
int ms_string_init_fmbs( MS_STRING *str, const char *mbs, size_t capacity, bool zerofill );
int ms_string_init_fwcs( MS_STRING *str, const wchar_t *wcs, size_t capacity, bool zerofill );
MS_STRING ms_string_return_fmbs( const char *mbs, size_t capacity, bool zerofill );
MS_STRING ms_string_return_fwcs( const char *wcs, size_t capacity, bool zerofill );

int ms_string_insert_mbs( MS_STRING *str, size_t index, const char *mbs );
int ms_string_insert_wcs( MS_STRING *str, size_t index, const wchar_t *wcs );

size_t ms_string_hash_mbs( const char *mbs, size_t length );
size_t ms_string_hash_wcs( const wchar_t *wcs, size_t length );

#endif
