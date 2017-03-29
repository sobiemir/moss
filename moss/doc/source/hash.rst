.. 
 . Moss Library >> http://moss.aculo.pl
 .
 .    /'\_/`\                           
 .   /\      \    ___     ____    ____  
 .   \ \ \__\ \  / __`\  /',__\  /',__\ 
 .    \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 .     \ \_\\ \_\ \____/\/\____/\/\____/
 .      \/_/ \/_/\/___/  \/___/  \/___/ 
 .
 . Documentation file for "Hash" module...
 ..

.. highlight:: c

Funkcje skrótu
=========================================================

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     32 bitowe skróty
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


32 Bitowe
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_djb2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_djb2a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_sdbm( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_joaat( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_fnv1a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_murmur3( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_murmur2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_murmur( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_xxhash( const void* data, size_t length )

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     64 bitowe skróty
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


64 Bitowe
---------------------------------------------------------

.. c:function:: uint64_t ms_hash_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_murmur2( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_xxhash( const void* data, size_t length )
