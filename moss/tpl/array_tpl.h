#ifndef __MSH_ARRAY_TEMPLATE__
#define __MSH_ARRAY_TEMPLATE__

#define __MST_ARRAY_COMPARE_UNSIGNED(_Type_) \
    _Type_ at = *(_Type_*)a, \
           bt = *(_Type_*)b; \
    return at < bt \
        ? -1 \
        : at == bt \
            ? 0 \
            : 1

#define __MST_ARRAY_COMPARE_SIGNED(_Type_) \
    return *(_Type_*)a - *(_Type_*)b


#define __MST_ARRAY_TSEARCH_LOOP(_Type_) \
    { \
        _Type_ *ptr  = tptr, \
                tmp  = *(_Type_*)item, \
               *lptr = ptr + (items - 1); \
        while( ptr != lptr && *lptr != itmp ) \
            ptr++; \
        tptr \
    }

#define __MST_ARRAY_TREVERSE_FORLOOP(_Type_) \
    _Type_ tmp, *ptr1 = (_Type_*)tptr, *ptr2 = (_Type_*)tptr + (items - 1); \
    for( iter = 0; iter < half; ++iter, ptr1++, ptr2-- ) \
        tmp = *ptr1, *ptr1 = *ptr2, *ptr2 = tmp;

#define __MST_ARRAY_TSEARCH_FORLOOP(_Type_) \
    _Type_ *ptr = tptr, itmp = *(_Type_*)item, *lptr = ptr + (items - 1); \
    while( ptr != lptr && *ptr != itmp ) ptr++; \
    tptr = ptr != lptr || *ptr == itmp ? ptr : NULL;

#define __MST_ARRAY_TRSEARCH_FORLOOP(_Type_) \
    _Type_ *ptr = tptr, itmp = *(_Type_*)item, *lptr = ptr + (items - 1); \
    while( ptr != lptr && *lptr != itmp ) ptr--; \
    tptr = ptr != lptr || *lptr == itmp ? lptr : NULL;

#define __MST_ARRAY_DESTROY(_Type_, _SName_, _FName_) \
void ms_##_FName_##_destroy( MS_##_SName_ *array ) { \
    if( !array ) return; \
    _msf_array_clean( array ); \
    if( array->Destroy ) { free( array ); array = NULL; } \
}

#define __MST_ARRAY_INIT(_Type_, _SName_, _FName_) \
int ms_##_FName_##_init( MS_##_SName_ *array, size_t capacity, bool zerofill ) { \
    assert( array ); \
    if( array->Items ) _msf_array_clean( array ); \
    array->Capacity = capacity; array->ItemSize = sizeof *array->Items; array->Size = 0; \
    array->Factor = 1.f; array->Destroy = false; array->ZeroFill = zerofill; \
    array->Items = zerofill ? calloc( array->Capacity, array->ItemSize ) : malloc( array->Capacity * array->ItemSize ); \
    if( !array->Items ) { ms_##_FName_##_destroy( array ); SETERRNOANDRETURN( MSEC_MEMORY_ALLOCATION ); } \
    return MSEC_OK; \
}

#endif
