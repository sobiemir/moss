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
 * Template file for "String" module...
 */

#ifndef __MSH_STRING_TEMPLATE__
#define __MSH_STRING_TEMPLATE__

/**
 * DJB2 K33
 * http://www.cse.yorku.ca/~oz/hash.html
 */
#define __MST_STRING_HASH_DJB2_K33_32B(_Var) \
	uint32_t hash = 5381; \
	for( ; length > 3; length -= 4 ) \
		hash = ((hash << 5) + hash) + *_Var++, \
		hash = ((hash << 5) + hash) + *_Var++, \
		hash = ((hash << 5) + hash) + *_Var++, \
		hash = ((hash << 5) + hash) + *_Var++; \
	switch( length ) \
	{ \
		case 3: hash = ((hash << 5) + hash) + *_Var++; \
		case 2: hash = ((hash << 5) + hash) + *_Var++; \
		case 1: hash = ((hash << 5) + hash) + *_Var++; \
		break; \
	} \
	return (size_t)hash

/**
 * SDBM
 * http://www.cse.yorku.ca/~oz/hash.html
 */
#define __MST_STRING_HASH_SDBM_32B(_Var) \
	uint32_t hash = 0; \
	for( ; length > 3; length -= 4 ) \
		hash = *_Var++ + (hash << 6) + (hash << 16) - hash, \
		hash = *_Var++ + (hash << 6) + (hash << 16) - hash, \
		hash = *_Var++ + (hash << 6) + (hash << 16) - hash, \
		hash = *_Var++ + (hash << 6) + (hash << 16) - hash; \
	switch( length ) \
	{ \
	case 3: hash = *_Var++ + (hash << 6) + (hash << 16) - hash; \
	case 2: hash = *_Var++ + (hash << 6) + (hash << 16) - hash; \
	case 1: hash = *_Var++ + (hash << 6) + (hash << 16) - hash; \
	break; \
	} \
	return (size_t)hash

/**
 * Jenkins One At Time
 * https://en.wikipedia.org/wiki/Jenkins_hash_function
 */
#define __MST_STRING_HASH_JENKINS_ONE_AT_A_TIME_32B(_Var) \
	uint32_t hash = 0; \
	for( ; length > 3; length -= 4 ) \
		hash += *_Var++, hash += hash << 10, hash ^= hash >> 6, \
		hash += *_Var++, hash += hash << 10, hash ^= hash >> 6, \
		hash += *_Var++, hash += hash << 10, hash ^= hash >> 6, \
		hash += *_Var++, hash += hash << 10, hash ^= hash >> 6; \
	switch( length ) \
	{ \
	case 3: hash += *_Var++, hash += hash << 10, hash ^= hash >> 6; \
	case 2: hash += *_Var++, hash += hash << 10, hash ^= hash >> 6; \
	case 1: hash += *_Var++, hash += hash << 10, hash ^= hash >> 6; \
	break; \
	} \
	hash += hash << 3; \
	hash ^= hash >> 11; \
	hash += hash << 15; \
    return (size_t)hash

/**
 * FNV-1 32BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
 */
#define __MST_STRING_HASH_FNV1_32B(_Var) \
	uint32_t hash = 2166136261u; \
	for( ; length > 3; length -= 4 ) \
		hash *= 16777619u, hash ^= (uint32_t)(*_Var++), \
		hash *= 16777619u, hash ^= (uint32_t)(*_Var++), \
		hash *= 16777619u, hash ^= (uint32_t)(*_Var++), \
		hash *= 16777619u, hash ^= (uint32_t)(*_Var++); \
	switch( length ) \
	{ \
	case 3: hash *= 16777619u, hash ^= (uint32_t)(*_Var++); \
	case 2: hash *= 16777619u, hash ^= (uint32_t)(*_Var++); \
	case 1: hash *= 16777619u, hash ^= (uint32_t)(*_Var++); \
	break; \
	} \
	return (size_t)hash

/**
 * FNV-1 64BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1
 */
#define __MST_STRING_HASH_FNV1_64B(_Var) \
	uint64_t hash = 14695981039346656037ull; \
	for( ; length > 3; length -= 4 ) \
		hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++), \
		hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++), \
		hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++), \
		hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++); \
	switch( length ) \
	{ \
	case 3: hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++); \
	case 2: hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++); \
	case 1: hash *= 1099511628211ull, hash ^= (uint64_t)(*_Var++); \
	break; \
	} \
	return (size_t)hash

/**
 * FNV-1A 32BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
 */
#define __MST_STRING_HASH_FNV1A_32B(_Var) \
	uint32_t hash = 2166136261u; \
	for( ; length > 3; length -= 4 ) \
		hash ^= (uint32_t)(*_Var++), hash *= 16777619u, \
		hash ^= (uint32_t)(*_Var++), hash *= 16777619u, \
		hash ^= (uint32_t)(*_Var++), hash *= 16777619u, \
		hash ^= (uint32_t)(*_Var++), hash *= 16777619u; \
	switch( length ) \
	{ \
	case 3: hash ^= (uint32_t)(*_Var++), hash *= 16777619u; \
	case 2: hash ^= (uint32_t)(*_Var++), hash *= 16777619u; \
	case 1: hash ^= (uint32_t)(*_Var++), hash *= 16777619u; \
	break; \
	} \
	return (size_t)hash

/**
 * FNV-1A 64BIT
 * http://isthe.com/chongo/tech/comp/fnv/#FNV-1a
 */
#define __MST_STRING_HASH_FNV1A_64B(_Var) \
	uint64_t hash = 14695981039346656037ull; \
	for( ; length > 3; length -= 4 ) \
		hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull, \
		hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull, \
		hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull, \
		hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull; \
	switch( length ) \
	{ \
	case 3: hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull; \
	case 2: hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull; \
	case 1: hash ^= (uint64_t)(*_Var++), hash *= 1099511628211ull; \
	break; \
	} \
	return (size_t)hash

#endif
