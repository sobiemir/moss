#include "../inc/configuration.h"

const bool msc_float_in_range =
    sizeof(uint8_t)  == sizeof(mst_voidptr_t) || sizeof(uint16_t) == sizeof(mst_voidptr_t) ||
    sizeof(uint32_t) == sizeof(mst_voidptr_t) || sizeof(uint64_t) == sizeof(mst_voidptr_t);
const bool msc_double_in_range =
    sizeof(uint8_t)  == sizeof(mst_voidptr_t) || sizeof(uint16_t) == sizeof(mst_voidptr_t) ||
    sizeof(uint32_t) == sizeof(mst_voidptr_t) || sizeof(uint64_t) == sizeof(mst_voidptr_t) ||
    sizeof(float)    == sizeof(mst_voidptr_t);
const bool msc_long_double_in_range =
    sizeof(uint8_t)  == sizeof(mst_voidptr_t) || sizeof(uint16_t) == sizeof(mst_voidptr_t) ||
    sizeof(uint32_t) == sizeof(mst_voidptr_t) || sizeof(uint64_t) == sizeof(mst_voidptr_t) ||
    sizeof(float)    == sizeof(mst_voidptr_t) || sizeof(double)   == sizeof(mst_voidptr_t);
const bool msc_voidptr_in_range = 
    sizeof(uint8_t)  == sizeof(mst_voidptr_t) || sizeof(uint16_t) == sizeof(mst_voidptr_t) ||
    sizeof(uint32_t) == sizeof(mst_voidptr_t) || sizeof(uint64_t) == sizeof(mst_voidptr_t) ||
    sizeof(float)    == sizeof(mst_voidptr_t) || sizeof(double)   == sizeof(mst_voidptr_t) ||
    sizeof(mst_longdouble_t) == sizeof(mst_voidptr_t);

#ifdef MSD_DEBUG

void ms_debug_display_types( void )
{
    printf( "char: %zi\n", sizeof(char) );
    printf( "short: %zi\n", sizeof(short) );
    printf( "int: %zi\n", sizeof(int) );
    printf( "long: %zi\n", sizeof(long) );
    printf( "long long: %zi\n", sizeof(mst_longlong_t) );
    printf( "float: %zi\n", sizeof(float) );
    printf( "double: %zi\n", sizeof(double) );
    printf( "long double: %zi\n", sizeof(mst_longdouble_t) );
    printf( "void*: %zi\n", sizeof(mst_voidptr_t) );
    printf( "size_t: %zi\n", sizeof(size_t) );
    printf( "bool: %zi\n", sizeof(bool) );
}

#endif
