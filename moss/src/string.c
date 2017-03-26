#include "../inc/string.h"

// ========================================================================================================================================

size_t ms_string_hash_mbs( const char *mbs, size_t length )
{
	// __MST_STRING_HASH_FNV1A_64B( mbs )
	// __MST_STRING_HASH_FNV1A_32B( mbs )
	// __MST_STRING_HASH_FNV1_64B( mbs )
	// __MST_STRING_HASH_FNV1_32B( mbs )
	__MST_STRING_HASH_JENKINS_ONE_AT_A_TIME_32B( mbs );
	// __MST_STRING_HASH_DJB2_K33_32B( mbs );
	// __MST_STRING_HASH_SDBM_32B( mbs );
}

// ========================================================================================================================================

size_t ms_string_hash_wcs( const wchar_t *wcs, size_t length )
{
	// __MST_STRING_HASH_FNV1A_64B( wcs )
	// __MST_STRING_HASH_FNV1A_32B( wcs )
	// __MST_STRING_HASH_FNV1_64B( wcs )
	// __MST_STRING_HASH_FNV1_32B( wcs )
	__MST_STRING_HASH_JENKINS_ONE_AT_A_TIME_32B( wcs );
	// __MST_STRING_HASH_DJB2_K33_32B( wcs );
	// __MST_STRING_HASH_SDBM_32B( wcs );
}
