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

#include <moss.h>

#include <stdio.h>
#include <locale.h>

#define FPS 60

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

int main( int argc, char **argv )
{
	int retval;
	char chr[MB_LEN_MAX + 1];

	MS_STRING str1;
	MS_STRING str2;
	MS_STRING str3;

	setlocale( LC_ALL, "pl_PL.utf8" );

	if( ms_string_init_cs(&str1, "TestCSTRc", 0) )
		printf( "Błąd podczas tworzenia CSTR\n" );
	if( (retval = ms_string_init_mbs(&str2, "Zażółć gęślą jaźń", 0)) )
		printf( "Błąd podczas tworzenia MBSTR %X\n", retval );
	// if( ms_string_init_wcs(&str3, L"Źdźbło łączy trawy", 0) )
	if( ms_string_init_wcs(&str3, L"Poczekaj tu!", 0) )
		printf( "Błąd podczas tworzenia WSTR\n" );

	// _setmode( _fileno(stdout), 0x00020000 );

	printf( "%s\n", str2.CData );

	if( (retval = ms_string_insert_cs(&str2, 7, "- porzeczka - ", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	printf( "%S\n", str3.WData );

	if( (retval = ms_string_insert_cs(&str3, 9, "no zaraz ", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	printf( "%S\n", str3.WData );

	if( (retval = ms_string_insert_cs(&str3, str3.Length, " Jak cudne manowce.", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	printf( "%S\n", str3.WData );

	printf( "%s\n", str2.CData );

	if( (retval = ms_string_insert_mbs(&str2, 7, "- przełęcz ", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	if( (retval = ms_string_insert_cs(&str2, str2.MBInfo->Length, " STOP COMP.", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	printf( "%s\n", str2.CData );

	if( (retval = ms_string_insert_wcs(&str2, 7, L"Łącki", 0)) )
		printf( "Błąd podczas wstawiania CSTR\n" );

	printf( "%s\n", str2.CData );

	for( retval = 0; retval < str2.MBInfo->Length; ++retval )
	{
		MS_MBINFO info = ms_array_get( str2.MBInfo, MS_MBINFO, retval );
		memcpy( chr, &str2.CData[info.Offset], info.Bytes );
		chr[info.Bytes] = '\0';
		printf( "Size: %d > Char: %s > Shift: 0x%02lX\n", info.Bytes, chr, info.Offset );
	}

	ms_string_free( &str1 );
	ms_string_free( &str2 );
	ms_string_free( &str3 );

	return 0;
}
