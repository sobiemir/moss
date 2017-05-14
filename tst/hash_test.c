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
 *  Test file for "Hash" module.
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
#include <msconf.h>
#include <mstest.h>
#include <moss/hash.h>

#ifdef MSD_COMPILER_MSC
	__pragma( warning(push) )
	__pragma( warning(disable:4127))    /* conditional expression is constant */
#endif

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	DJB ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie DJB2.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_djb2( MST_FUNCTION *info )
{
	const char    atest[] =  "DJBKey";
	const wchar_t wtest[] = L"DJBKey";
	
	uint32_t result, wresult1, wresult2;

	mst_prepare( info );

	/* char */
	result = ms_hash_32_djb2( atest, strlen(atest) );
	mst_assert_uint( result, ==, 0xABDE625E );
	result = ms_hash_mbs_32_djb2( atest );
	mst_assert_uint( result, ==, 0xABDE625E );

	/* wchar_t */
	wresult1 = ms_hash_32_djb2( wtest, wcslen(wtest) * sizeof(wchar_t) );
	wresult2 = ms_hash_wcs_32_djb2( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( wresult1, ==, 0x1C78C0DE );
		mst_assert_uint( wresult2, ==, 0x1C78C0DE );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( wresult1, ==, 0x07DB25DE );
		mst_assert_uint( wresult2, ==, 0x07DB25DE );
	}

	return MSEC_OK;
}

/**
 * Testuje funkcje skrótu oparte na algorytmie DJB2A.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_djb2a( MST_FUNCTION *info )
{
	const char    atest[] =  "DJBKey";
	const wchar_t wtest[] = L"DJBKey";
	
	uint32_t result, wresult1, wresult2;

	mst_prepare( info );

	/* char */
	result = ms_hash_32_djb2a( atest, strlen(atest) );
	mst_assert_uint( result, ==, 0x99329EBE );
	result = ms_hash_mbs_32_djb2a( atest );
	mst_assert_uint( result, ==, 0x99329EBE );

	/* wchar_t */
	wresult1 = ms_hash_32_djb2a( wtest, wcslen(wtest) * sizeof(wchar_t) );
	wresult2 = ms_hash_wcs_32_djb2a( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( wresult1, ==, 0xB9704D7E );
		mst_assert_uint( wresult2, ==, 0xB9704D7E );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( wresult1, ==, 0x509BE3FE );
		mst_assert_uint( wresult2, ==, 0x509BE3FE );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	SDBM ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie SDBM.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_sdbm( MST_FUNCTION *info )
{
	const char    atest[] =  "SDBMKey";
	const wchar_t wtest[] = L"SDBMKey";
	
	uint32_t result, wresult1, wresult2;

	mst_prepare( info );

	/* char */
	result = ms_hash_32_sdbm( atest, strlen(atest) );
	mst_assert_uint( result, ==, 0xFA861463 );
	result = ms_hash_mbs_32_sdbm( atest );
	mst_assert_uint( result, ==, 0xFA861463 );

	/* wchar_t */
	wresult1 = ms_hash_32_sdbm( wtest, wcslen(wtest) * sizeof(wchar_t) );
	wresult2 = ms_hash_wcs_32_sdbm( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( wresult1, ==, 0x3A38E971 );
		mst_assert_uint( wresult2, ==, 0x3A38E971 );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( wresult1, ==, 0x53C918F1 );
		mst_assert_uint( wresult2, ==, 0x53C918F1 );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	JOAAT ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie JOAAT.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_joaat( MST_FUNCTION *info )
{
	const char    atest[] =  "JoaaTKey";
	const wchar_t wtest[] = L"JoaaTKey";
	
	uint32_t result, wresult1, wresult2;

	mst_prepare( info );

	/* char */
	result = ms_hash_32_joaat( atest, strlen(atest) );
	mst_assert_uint( result, ==, 0x94E6381F );
	result = ms_hash_mbs_32_joaat( atest );
	mst_assert_uint( result, ==, 0x94E6381F );

	/* wchar_t */
	wresult1 = ms_hash_32_joaat( wtest, wcslen(wtest) * sizeof(wchar_t) );
	wresult2 = ms_hash_wcs_32_joaat( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( wresult1, ==, 0xC24C203E );
		mst_assert_uint( wresult2, ==, 0xC24C203E );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( wresult1, ==, 0x311CEE84 );
		mst_assert_uint( wresult2, ==, 0x311CEE84 );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	FNV ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie FNV-1.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_fnv1( MST_FUNCTION *info )
{
	const char    atest[] =  "FNVFamilyKey";
	const wchar_t wtest[] = L"FNVFamilyKey";
	
	uint32_t result32, w32result1, w32result2;
	uint64_t result64, w64result1, w64result2;

	mst_prepare( info );

	/* char, wersja 32 bitowa */
	result32 = ms_hash_32_fnv1( atest, strlen(atest) );
	mst_assert_uint( result32, ==, 0x7119B356 );
	result32 = ms_hash_mbs_32_fnv1( atest );
	mst_assert_uint( result32, ==, 0x7119B356 );

	/* char, wersja 64 bitowa */
	result64 = ms_hash_64_fnv1( atest, strlen(atest) );
	mst_assert_uint( result64, ==, 0xC552531A28539836 );
	result64 = ms_hash_mbs_64_fnv1( atest );
	mst_assert_uint( result64, ==, 0xC552531A28539836 );

	/* wchar_t, wersja 32 bitowa */
	w32result1 = ms_hash_32_fnv1( wtest, wcslen(wtest) * sizeof(wchar_t) );
	w32result2 = ms_hash_wcs_32_fnv1( wtest );

	/* wchar_t, wersja 64 bitowa */
	w64result1 = ms_hash_64_fnv1( wtest, wcslen(wtest) * sizeof(wchar_t) );
	w64result2 = ms_hash_wcs_64_fnv1( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( w32result1, ==, 0x7AFBDF48 );
		mst_assert_uint( w32result2, ==, 0x7AFBDF48 );
		mst_assert_uint( w64result1, ==, 0x3CB9F0BF944BFFC8 );
		mst_assert_uint( w64result2, ==, 0x3CB9F0BF944BFFC8 );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( w32result1, ==, 0x20ED7910 );
		mst_assert_uint( w32result2, ==, 0x20ED7910 );
		mst_assert_uint( w64result1, ==, 0x211BBD1C5D42E1D0 );
		mst_assert_uint( w64result2, ==, 0x211BBD1C5D42E1D0 );
	}

	return MSEC_OK;
}

/**
 * Testuje funkcje skrótu oparte na algorytmie FNV-1A.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_fnv1a( MST_FUNCTION *info )
{
	const char    atest[] =  "FNVFamilyKey";
	const wchar_t wtest[] = L"FNVFamilyKey";
	
	uint32_t result32, w32result1, w32result2;
	uint64_t result64, w64result1, w64result2;

	mst_prepare( info );

	/* char, wersja 32 bitowa */
	result32 = ms_hash_32_fnv1a( atest, strlen(atest) );
	mst_assert_uint( result32, ==, 0x94A2258C );
	result32 = ms_hash_mbs_32_fnv1a( atest );
	mst_assert_uint( result32, ==, 0x94A2258C );

	/* char, wersja 64 bitowa */
	result64 = ms_hash_64_fnv1a( atest, strlen(atest) );
	mst_assert_uint( result64, ==, 0x27693918C0BB3CCC );
	result64 = ms_hash_mbs_64_fnv1a( atest );
	mst_assert_uint( result64, ==, 0x27693918C0BB3CCC );

	/* wchar_t, wersja 32 bitowa */
	w32result1 = ms_hash_32_fnv1a( wtest, wcslen(wtest) * sizeof(wchar_t) );
	w32result2 = ms_hash_wcs_32_fnv1a( wtest );

	/* wchar_t, wersja 32 bitowa */
	w64result1 = ms_hash_64_fnv1a( wtest, wcslen(wtest) * sizeof(wchar_t) );
	w64result2 = ms_hash_wcs_64_fnv1a( wtest );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( w32result1, ==, 0xFA945892 );
		mst_assert_uint( w32result2, ==, 0xFA945892 );
		mst_assert_uint( w64result1, ==, 0xBC28813FCFD3D0F2 );
		mst_assert_uint( w64result2, ==, 0xBC28813FCFD3D0F2 );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( w32result1, ==, 0x9D543B0A );
		mst_assert_uint( w32result2, ==, 0x9D543B0A );
		mst_assert_uint( w64result1, ==, 0x87D2CE8C5EFA642A );
		mst_assert_uint( w64result2, ==, 0x87D2CE8C5EFA642A );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	MURMUR ALGORITHMS
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie Murmur.
 * Sprawdza warianty: standardowy.
 * Ze względu na algorytm nie zostały utworzone warianty mbs i wcs.
 */
int mst_hash_murmur1( MST_FUNCTION *info )
{
	const char *atest[] =
	{
		"MurmurKey",
		"MurmurKey1",
		"MurmurKey12",
		"MurmurKey123"
	};
	uint32_t result;

	mst_prepare( info );

	result = ms_hash_32_murmur1( atest[0], strlen(atest[0]) );
	mst_assert_uint( result, ==, 0xAF772697 );
	result = ms_hash_32_murmur1( atest[1], strlen(atest[1]) );
	mst_assert_uint( result, ==, 0xEF00677C );
	result = ms_hash_32_murmur1( atest[2], strlen(atest[2]) );
	mst_assert_uint( result, ==, 0x66F61852 );
	result = ms_hash_32_murmur1( atest[3], strlen(atest[3]) );
	mst_assert_uint( result, ==, 0xC0AE799F );

	return MSEC_OK;
}

/**
 * Testuje funkcje skrótu oparte na algorytmie Murmur2.
 * Sprawdza warianty: standardowy.
 * Ze względu na algorytm nie zostały utworzone warianty mbs i wcs.
 */
int mst_hash_murmur2( MST_FUNCTION *info )
{
	const char *atest[] =
	{
		"MurmurKey",
		"MurmurKey1",
		"MurmurKey12",
		"MurmurKey123",
		"MurmurKey1234",
		"MurmurKey12345",
		"MurmurKey123456",
		"MurmurKey1234567"
	};
	uint32_t result32;
	uint64_t result64;

	mst_prepare( info );

	/* char, wersja 32 bitowa */
	result32 = ms_hash_32_murmur2( atest[0], strlen(atest[0]) );
	mst_assert_uint( result32, ==, 0xA6A87356 );
	result32 = ms_hash_32_murmur2( atest[1], strlen(atest[1]) );
	mst_assert_uint( result32, ==, 0xFE6E2293 );
	result32 = ms_hash_32_murmur2( atest[2], strlen(atest[2]) );
	mst_assert_uint( result32, ==, 0x5FF5020B );
	result32 = ms_hash_32_murmur2( atest[3], strlen(atest[3]) );
	mst_assert_uint( result32, ==, 0xF1D9CC0B );

	/* char, wersja 64 bitowa */
	result64 = ms_hash_64_murmur2( atest[0], strlen(atest[0]) );
	mst_assert_uint( result64, ==, 0xE725056E186EB4D6 );
	result64 = ms_hash_64_murmur2( atest[1], strlen(atest[1]) );
	mst_assert_uint( result64, ==, 0x033CB80EC8BEB4BE );
	result64 = ms_hash_64_murmur2( atest[2], strlen(atest[2]) );
	mst_assert_uint( result64, ==, 0x2EB3FB4D381F500E );
	result64 = ms_hash_64_murmur2( atest[3], strlen(atest[3]) );
	mst_assert_uint( result64, ==, 0xF68B2345BE9FAD33 );
	result64 = ms_hash_64_murmur2( atest[4], strlen(atest[4]) );
	mst_assert_uint( result64, ==, 0x18A99EA9BFB13235 );
	result64 = ms_hash_64_murmur2( atest[5], strlen(atest[5]) );
	mst_assert_uint( result64, ==, 0x03E0C9B2EBC1F413 );
	result64 = ms_hash_64_murmur2( atest[6], strlen(atest[6]) );
	mst_assert_uint( result64, ==, 0x2F972DF4971465F0 );
	result64 = ms_hash_64_murmur2( atest[7], strlen(atest[7]) );
	mst_assert_uint( result64, ==, 0xD10ECF806D50F7D9 );

	return MSEC_OK;
}

/**
 * Testuje funkcje skrótu oparte na algorytmie Murmur3.
 * Sprawdza warianty: standardowy, mbs, wcs.
 */
int mst_hash_murmur3( MST_FUNCTION *info )
{
	const char *atest[] =
	{
		"MurmurKey",
		"MurmurKey1",
		"MurmurKey12",
		"MurmurKey123"
	};
	const wchar_t *wtest[] =
	{
		L"MurmurKey",
		L"MurmurKey1",
		L"MurmurKey12",
		L"MurmurKey123"
	};
	uint32_t result;
	uint32_t wresult1[4];
	uint32_t wresult2[4];

	mst_prepare( info );

	/* char */
	result = ms_hash_32_murmur3( atest[0], strlen(atest[0]) );
	mst_assert_uint( result, ==, 0xF3D48C5C );
	result = ms_hash_mbs_32_murmur3( atest[0] );
	mst_assert_uint( result, ==, 0xF3D48C5C );
	result = ms_hash_32_murmur3( atest[1], strlen(atest[1]) );
	mst_assert_uint( result, ==, 0xAEE175B3 );
	result = ms_hash_mbs_32_murmur3( atest[1] );
	mst_assert_uint( result, ==, 0xAEE175B3 );
	result = ms_hash_32_murmur3( atest[2], strlen(atest[2]) );
	mst_assert_uint( result, ==, 0x859648EE );
	result = ms_hash_mbs_32_murmur3( atest[2] );
	mst_assert_uint( result, ==, 0x859648EE );
	result = ms_hash_32_murmur3( atest[3], strlen(atest[3]) );
	mst_assert_uint( result, ==, 0xE2050FD7 );
	result = ms_hash_mbs_32_murmur3( atest[3] );
	mst_assert_uint( result, ==, 0xE2050FD7 );

	/* wchar_t */
	wresult1[0] = ms_hash_32_murmur3( wtest[0], wcslen(wtest[0]) * sizeof(wchar_t) );
	wresult2[0] = ms_hash_wcs_32_murmur3( wtest[0] );
	wresult1[1] = ms_hash_32_murmur3( wtest[1], wcslen(wtest[1]) * sizeof(wchar_t) );
	wresult2[1] = ms_hash_wcs_32_murmur3( wtest[1] );
	wresult1[2] = ms_hash_32_murmur3( wtest[2], wcslen(wtest[2]) * sizeof(wchar_t) );
	wresult2[2] = ms_hash_wcs_32_murmur3( wtest[2] );
	wresult1[3] = ms_hash_32_murmur3( wtest[3], wcslen(wtest[3]) * sizeof(wchar_t) );
	wresult2[3] = ms_hash_wcs_32_murmur3( wtest[3] );

	if( sizeof(wchar_t) == 2 )
	{
		mst_assert_uint( wresult1[0], ==, 0xF3F54C78 );
		mst_assert_uint( wresult2[0], ==, 0xF3F54C78 );
		mst_assert_uint( wresult1[1], ==, 0x90A3A98D );
		mst_assert_uint( wresult2[1], ==, 0x90A3A98D );
		mst_assert_uint( wresult1[2], ==, 0x3849F3E5 );
		mst_assert_uint( wresult2[2], ==, 0x3849F3E5 );
		mst_assert_uint( wresult1[3], ==, 0x8AD19CEE );
		mst_assert_uint( wresult2[3], ==, 0x8AD19CEE );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		mst_assert_uint( wresult1[0], ==, 0x26384360 );
		mst_assert_uint( wresult2[0], ==, 0x26384360 );
		mst_assert_uint( wresult1[1], ==, 0x6E196D7C );
		mst_assert_uint( wresult2[1], ==, 0x6E196D7C );
		mst_assert_uint( wresult1[2], ==, 0x955C377D );
		mst_assert_uint( wresult2[2], ==, 0x955C377D );
		mst_assert_uint( wresult1[3], ==, 0x47EC00B5 );
		mst_assert_uint( wresult2[3], ==, 0x47EC00B5 );
	}

	return MSEC_OK;
}

/*
======================================================================================================================
------------------------------------------------------------------------------------------------------------------
	XXHASH ALGORITHM
------------------------------------------------------------------------------------------------------------------
======================================================================================================================
*/

/**
 * Testuje funkcje skrótu oparte na algorytmie xxHash w wersji 32 bitowej.
 * Sprawdza warianty: standardowy, mbs, wcs.
 * Wersje zostały oddzielone z racji skomplikowania algorytmu.
 */
int mst_hash_xxhash32( MST_FUNCTION *info )
{
	const char *atest[] =
	{
		"xxHashKey",
		"xxHashKey1",
		"xxHashKey12",
		"xxHashKey123",
		"xxHashKey1234",
		"xxHashKey12345",
		"xxHashKey123456",
		"xxHashKey1234567",
		"xxHashKey12345678",
		"xxHashKey123456789",
		"xxHashKey1234567890",
		"xxHashKey1234567890A",
		"xxHashKey1234567890AB",
		"xxHashKey1234567890ABC",
		"xxHashKey1234567890ABCD",
		"xxHashKey1234567890ABCDE"
	};
	const wchar_t *wtest[] =
	{
		L"xxHashKey",
		L"xxHashKey1",
		L"xxHashKey12",
		L"xxHashKey123",
		L"xxHashKey1234",
		L"xxHashKey12345",
		L"xxHashKey123456",
		L"xxHashKey1234567",
		L"xxHashKey12345678",
		L"xxHashKey123456789",
		L"xxHashKey1234567890",
		L"xxHashKey1234567890A",
		L"xxHashKey1234567890AB",
		L"xxHashKey1234567890ABC",
		L"xxHashKey1234567890ABCD",
		L"xxHashKey1234567890ABCDE",
		L"xHa"
	};
	size_t iter;

	/* bok + góra = indeks który nie przeszedł lub testowany indeks */
	uint32_t svals[] =
	{
		/*   0           1           2           3    */
		0xAA245726, 0x4CE783F9, 0x3F5CBD0B, 0x6010A46F, /* 1  */
		0xC14A8983, 0xA94A4D5D, 0x1332968C, 0x078D4FA9, /* 5  */
		0x555CC6A2, 0xF634297E, 0x9502E6D8, 0xCBAB52F8, /* 9  */
		0x6386F7BD, 0xBCBBC336, 0x94138B2E, 0x12D0EF9C  /* 13 */
	};

	mst_prepare( info );

	/* char */
	for( iter = 0; iter < 16; ++iter )
	{
		/* printf( "PASS: %zu\n", iter ); */
		mst_assert_uint( ms_hash_32_xxhash(atest[iter], strlen(atest[iter])), ==, svals[iter] );
		mst_assert_uint( ms_hash_mbs_32_xxhash(atest[iter]), ==, svals[iter] );
	}

	/* wchar_t */
	if( sizeof(wchar_t) == 2 )
	{
		/* bok + góra = indeks który nie przeszedł lub testowany indeks */
		uint32_t wvals[] =
		{
			/*   0           1           2           3    */
			0xB1727C1C, 0xE7BFFC27, 0x12EC6C22, 0x3E3E6AB6, /* 1  */
			0x1715C13C, 0xF739C79A, 0xA99CCC93, 0x6D9C4A7B, /* 5  */
			0xD6862D63, 0xDDDDE141, 0x83E080EC, 0x8C33E16E, /* 9  */
			0xDF5028E9, 0xEA6D9F73, 0xDCB2601E, 0xBFF627A8  /* 13 */
		};

		for( iter = 0; iter < 16; ++iter )
		{
			/* printf( "PASS: %zu\n", iter ); */
			mst_assert_uint( ms_hash_32_xxhash(wtest[iter], wcslen(wtest[iter]) * sizeof(wchar_t)), ==, wvals[iter] );
			mst_assert_uint( ms_hash_wcs_32_xxhash(wtest[iter]), ==, wvals[iter] );
		}
		mst_assert_uint( ms_hash_32_xxhash(wtest[16], wcslen(wtest[16]) * sizeof(wchar_t)), ==, 0xE7A5359C );
		mst_assert_uint( ms_hash_wcs_32_xxhash(wtest[16]), ==, 0xE7A5359C );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		/* bok + góra = indeks który nie przeszedł lub testowany indeks */
		uint32_t wvals[] =
		{
			/*   0           1           2           3    */
			0x81821019, 0x9C36E8D1, 0x4D41319E, 0x46627E6C, /* 1  */
			0xC834F32C, 0x19546302, 0x5AA824CB, 0xE364E8DA, /* 5  */
			0xC4ABFD9E, 0x997130A2, 0x746D3676, 0xF8B9B25F, /* 9  */
			0x796C2C10, 0x4B00CABD, 0x7AF740F1, 0x082AC455  /* 13 */
		};

		for( iter = 0; iter < 16; ++iter )
		{
			/* printf( "PASS: %zu\n", iter ); */
			mst_assert_uint( ms_hash_32_xxhash(wtest[iter], wcslen(wtest[iter]) * sizeof(wchar_t)), ==, wvals[iter] );
			mst_assert_uint( ms_hash_wcs_32_xxhash(wtest[iter]), ==, wvals[iter] );
		}
		mst_assert_uint( ms_hash_32_xxhash(wtest[16], wcslen(wtest[16]) * sizeof(wchar_t)), ==, 0xE684C0C9 );
		mst_assert_uint( ms_hash_wcs_32_xxhash(wtest[16]), ==, 0xE684C0C9 );
	}

	return MSEC_OK;
}

/**
 * 11.
 * Testuje funkcje skrótu oparte na algorytmie xxHash w wersji 64 bitowej.
 * Sprawdza warianty: standardowy, mbs, wcs.
 * Wersje zostały oddzielone z racji skomplikowania algorytmu.
 */
int mst_hash_xxhash64( MST_FUNCTION *info )
{
	const char *atest[] =
	{
		"xxHashKey",
		"xxHashKey1",
		"xxHashKey12",
		"xxHashKey123",
		"xxHashKey1234",
		"xxHashKey12345",
		"xxHashKey123456",
		"xxHashKey1234567",
		"xxHashKey12345678",
		"xxHashKey123456789",
		"xxHashKey1234567890",
		"xxHashKey1234567890A",
		"xxHashKey1234567890AB",
		"xxHashKey1234567890ABC",
		"xxHashKey1234567890ABCD",
		"xxHashKey1234567890ABCDE",
		"xxHashKey1234567890ABCDEF",
		"xxHashKey1234567890ABCDEFG",
		"xxHashKey1234567890ABCDEFGH",
		"xxHashKey1234567890ABCDEFGHI",
		"xxHashKey1234567890ABCDEFGHIJ",
		"xxHashKey1234567890ABCDEFGHIJK",
		"xxHashKey1234567890ABCDEFGHIJKL",
		"xxHashKey1234567890ABCDEFGHIJKLM",
		"xxHashKey1234567890ABCDEFGHIJKLMN",
		"xxHashKey1234567890ABCDEFGHIJKLMNO",
		"xxHashKey1234567890ABCDEFGHIJKLMNOP",
		"xxHashKey1234567890ABCDEFGHIJKLMNOPQ",
		"xxHashKey1234567890ABCDEFGHIJKLMNOPQR",
		"xxHashKey1234567890ABCDEFGHIJKLMNOPQRS",
		"xxHashKey1234567890ABCDEFGHIJKLMNOPQRST",
		"xxHashKey1234567890ABCDEFGHIJKLMNOPQRSTU"
	};
	const wchar_t *wtest[] =
	{
		L"xxHashKey",
		L"xxHashKey1",
		L"xxHashKey12",
		L"xxHashKey123",
		L"xxHashKey1234",
		L"xxHashKey12345",
		L"xxHashKey123456",
		L"xxHashKey1234567",
		L"xxHashKey12345678",
		L"xxHashKey123456789",
		L"xxHashKey1234567890",
		L"xxHashKey1234567890A",
		L"xxHashKey1234567890AB",
		L"xxHashKey1234567890ABC",
		L"xxHashKey1234567890ABCD",
		L"xxHashKey1234567890ABCDE",
		L"xxHashKey1234567890ABCDEF",
		L"xxHashKey1234567890ABCDEFG",
		L"xxHashKey1234567890ABCDEFGH",
		L"xxHashKey1234567890ABCDEFGHI",
		L"xxHashKey1234567890ABCDEFGHIJ",
		L"xxHashKey1234567890ABCDEFGHIJK",
		L"xxHashKey1234567890ABCDEFGHIJKL",
		L"xxHashKey1234567890ABCDEFGHIJKLM",
		L"xxHashKey1234567890ABCDEFGHIJKLMN",
		L"xxHashKey1234567890ABCDEFGHIJKLMNO",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOP",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOPQ",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOPQR",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOPQRS",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOPQRST",
		L"xxHashKey1234567890ABCDEFGHIJKLMNOPQRSTU",
		L"xHa"
	};
	size_t iter;

	/* bok + góra = indeks który nie przeszedł lub testowany indeks */
	uint64_t svals[] =
	{
		/*       0                   1                  2                   3         */
		0xCCB13b34381D3BC3, 0xFE9E608A92DC0052, 0x476293E945CD01CA, 0x60994333F604772E, /* 1  */
		0x106965EA9CCB1B71, 0x1DF205AB5B08F190, 0xCE09BB4F9AE1E735, 0x6DAE389F1CCE5FC1, /* 5  */
		0x5DC89520B44FAD5C, 0x4C10EC5D3E35D873, 0x4B32DA7972CFB23E, 0xBFE7A8BD4CF506EC, /* 9  */
		0x93A43287A250FD1B, 0x9A2E1C94C2CF75AB, 0x0033A2EC1091D7C6, 0x7701DB6969D456C9, /* 13 */
		0xB41C144388E63E75, 0x2422B5712245042D, 0x99843CAA64085549, 0xC22FD2E6E5702AA8, /* 17 */
		0x9F63287B9AB53247, 0x14DD70B25E8360D9, 0x2C58D4C555A1AC53, 0x54005F16E0852B1C, /* 21 */
		0x9D2B1ED629E1148C, 0x3BAFD1F48F1451A6, 0x88BDE5165D8F91C3, 0xB3141CEEB2B51949, /* 25 */
		0x137B4F591C02F472, 0xE7A8784CF718FAFA, 0x30D71242A787EF27, 0x9A3CD311BF4F0150  /* 29 */
	};

	mst_prepare( info );

	/* char */
	for( iter = 0; iter < 32; ++iter )
	{
		/* printf( "PASS: %zu\n", iter ); */
		mst_assert_uint( ms_hash_64_xxhash(atest[iter], strlen(atest[iter])), ==, svals[iter] );
		mst_assert_uint( ms_hash_mbs_64_xxhash(atest[iter]), ==, svals[iter] );
	}

	/* wchar_t */
	if( sizeof(wchar_t) == 2 )
	{
		/* bok + góra = indeks który nie przeszedł lub testowany indeks */
		uint64_t wvals[] =
		{
			/*       0                   1                  2                   3         */
			0xC4B3F6FA7DC28773, 0x7A512B6C3210BE77, 0x2FAC324BEA747D96, 0xEED2C0A835410B66, /* 1  */
			0x00953C5A4095F61D, 0x81D3478290B1F178, 0xD35FC1F6B85D13D8, 0x977D4B0FDFA8F1C1, /* 5  */
			0x7ADF94D2914CD233, 0x16C809B48FB63AB8, 0xFFE4BABA597677B6, 0xD160F19584FCAB13, /* 9  */
			0xA1C4C718619FA515, 0x483B2F9572EE18A3, 0x77205C92EB87C6AE, 0x9F164DE6454C2164, /* 13 */
			0xA3E0BFA318EF4DE9, 0x26DA2CF97AAFBC99, 0x167BF77AFDBB7EBB, 0xA8CEDEC59916E568, /* 17 */
			0xDFE3383DE455529E, 0x4EDC0ED2EF444C66, 0x0FCC0AD42C9F62A9, 0x7237B9FE45D6B848, /* 21 */
			0x27895691C9934944, 0x203324A6A23BB2BC, 0x400ADB9EC70177D6, 0x226130124B5A2E5F, /* 25 */
			0x3ABCD6C65D38E409, 0xDA9E8FBE6A3D8EF6, 0x6BAF6788CE53AC4E, 0x5777F60E5EE4569F  /* 29 */
		};

		for( iter = 0; iter < 32; ++iter )
		{
			/* printf( "PASS: %zu\n", iter ); */
			mst_assert_uint( ms_hash_64_xxhash(wtest[iter], wcslen(wtest[iter]) * sizeof(wchar_t)), ==, wvals[iter] );
			mst_assert_uint( ms_hash_wcs_64_xxhash(wtest[iter]), ==, wvals[iter] );
		}
		mst_assert_uint( ms_hash_64_xxhash(wtest[32], wcslen(wtest[32]) * sizeof(wchar_t)), ==, 0xED8644FBB587A3EA );
		mst_assert_uint( ms_hash_wcs_64_xxhash(wtest[32]), ==, 0xED8644FBB587A3EA );
	}
	else if( sizeof(wchar_t) == 4 )
	{
		/* bok + góra = indeks który nie przeszedł lub testowany indeks */
		uint64_t wvals[] =
		{
			/*       0                   1                  2                   3         */
			0x581B838C77AA7766, 0xBC7408062FDE6DD2, 0x5A430BBBA0FE5E1D, 0xCC68DEC5E8493245, /* 1  */
			0x3112B0383799B462, 0x43A1494570BA6E6A, 0x1A3C2306B7544963, 0x551183299A21D197, /* 5  */
			0x8A44A284CCAB492B, 0x5D8FEBD75CC5B1EE, 0x00AD05F19494FD1D, 0x1AA33245B637EDEF, /* 9  */
			0x6AB973CBE34DA193, 0x8ED16F3C726A46A1, 0x4FA8D00C5F8C15BD, 0x3366E4B02009E7AD, /* 13 */
			0xB654A90B1699B57E, 0x54D4B03F42E78826, 0xD51DA92BA8C5F6B6, 0x8B3F0FE2070231B2, /* 17 */
			0xE128E8BD97040514, 0x415E1A493DF6A862, 0x5F79B71714A1132D, 0x3307507BCFD7F94C, /* 21 */
			0x9D22FDD1676497F4, 0xD7169F26648AC51C, 0x6E8F919E18CD3525, 0xE61637E48BCFBAE2, /* 25 */
			0x9A63E2EDDB54DFC2, 0x47FCBD5D8CA7708D, 0x3E8D8893B37D977C, 0x7251EF739972D15A  /* 29 */
		};

		for( iter = 0; iter < 32; ++iter )
		{
			/* printf( "PASS: %zu\n", iter ); */
			mst_assert_uint( ms_hash_64_xxhash(wtest[iter], wcslen(wtest[iter]) * sizeof(wchar_t)), ==, wvals[iter] );
			mst_assert_uint( ms_hash_wcs_64_xxhash(wtest[iter]), ==, wvals[iter] );
		}
		mst_assert_uint( ms_hash_64_xxhash(wtest[32], wcslen(wtest[32]) * sizeof(wchar_t)), ==, 0x562A87F545E2D7A5 );
		mst_assert_uint( ms_hash_wcs_64_xxhash(wtest[32]), ==, 0x562A87F545E2D7A5 );
	}

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
#define FUNC_DESC_01 "Test value returned by all djb2 functions."
#define FUNC_DESC_02 "Test value returned by all djb2a functions."
#define FUNC_DESC_03 "Test value returned by all sdbm functions."
#define FUNC_DESC_04 "Test value returned by all joaat functions."
#define FUNC_DESC_05 "Test value returned by all 32 and 64 bit fnv1 functions."
#define FUNC_DESC_06 "Test value returned by all 32 and 64 bit fnv1a functions."
#define FUNC_DESC_07 "Test value returned by murmur1 function."
#define FUNC_DESC_08 "Test value returned by 32 and 64 bit murmur2 functions."
#define FUNC_DESC_09 "Test value returned by murmur3 functions."
#define FUNC_DESC_10 "Test value returned by 32 bit xxhash functions."
#define FUNC_DESC_11 "Test value returned by 64 bit xxhash functions."

/**
 * Lista funkcji testujących moduł.
 * Każda z osobna uruchamiana jest przez odpowiednią funkcję.
 * Dzięki zbiorowi funkcji zdefiniowanemu poniżej dzieje się to automatycznie.
 * Funkcje nie są ze sobą powiązane, więc mogą być wykonywane w dowolnej kolejności.
 */
MST_FUNCTION MSV_HashSuiteFunctions[] =
{
	{ MST_STRINGIFY(mst_hash_djb2),     FUNC_DESC_01, NULL },
	{ MST_STRINGIFY(mst_hash_djb2a),    FUNC_DESC_02, NULL },
	{ MST_STRINGIFY(mst_hash_sdbm),     FUNC_DESC_03, NULL },
	{ MST_STRINGIFY(mst_hash_joaat),    FUNC_DESC_04, NULL },
	{ MST_STRINGIFY(mst_hash_fnv1),     FUNC_DESC_05, NULL },
	{ MST_STRINGIFY(mst_hash_fnv1a),    FUNC_DESC_06, NULL },
	{ MST_STRINGIFY(mst_hash_murmur1),  FUNC_DESC_07, NULL },
	{ MST_STRINGIFY(mst_hash_murmur2),  FUNC_DESC_08, NULL },
	{ MST_STRINGIFY(mst_hash_murmur3),  FUNC_DESC_09, NULL },
	{ MST_STRINGIFY(mst_hash_xxhash32), FUNC_DESC_10, NULL },
	{ MST_STRINGIFY(mst_hash_xxhash64), FUNC_DESC_11, NULL },
	{ MST_LASTRECORD }
};

/**
 * Zbiór funkcji testujących moduł.
 * Przekazywany do funkcji main, pozwala na uruchomienie wszystkich testów.
 */
MST_SUITE MSV_HashSuite =
{
	">>> HASH MODULE",
	TRUE,
	NULL,
	NULL,
	NULL,
	MSV_HashSuiteFunctions
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
		IGVAR argc;
		IGVAR argv;

		mst_run_suite( &MSV_HashSuite );

		return 0;
	}

#endif
#ifdef MSD_COMPILER_MSC
	__pragma( warning(pop) )
#endif

