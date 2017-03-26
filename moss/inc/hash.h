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
 * Header file for "Hash" module...
 */

#ifndef __MSH_HASH__
#define __MSH_HASH__

#include "configuration.h"

uint32_t ms_hash_32_djb2( const void *data, size_t length );
uint32_t ms_hash_32_djb2a( const void *data, size_t length );
uint32_t ms_hash_32_sdbm( const void *data, size_t length );
uint32_t ms_hash_32_joaat( const void *data, size_t length );
uint32_t ms_hash_32_fnv1( const void *data, size_t length );
uint64_t ms_hash_64_fnv1( const void *data, size_t length );
uint32_t ms_hash_32_fnv1a( const void *data, size_t length );
uint64_t ms_hash_64_fnv1a( const void *data, size_t length );
uint32_t ms_hash_32_murmur3( const void *data, size_t length );

#endif
