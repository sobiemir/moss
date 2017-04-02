.. 
 .  Moss Library >>> http://moss.aculo.pl
 .
 .     /'\_/`\                           
 .    /\      \    ___     ____    ____  
 .    \ \ \__\ \  / __`\  /',__\  /',__\ 
 .     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 .      \ \_\\ \_\ \____/\/\____/\/\____/
 .       \/_/ \/_/\/___/  \/___/  \/___/ 
 .
 .  Documentation file for "Hash" module.
 .  See LICENSE file for copyright information.
 ..

.. highlight:: c

Funkcje skrótu
=========================================================


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny DJB
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny DJB
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_djb2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_djb2a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_djb2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_djb2a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_djb2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_djb2a( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm SDBM
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm SDBM
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_sdbm( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_sdbm( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_sdbm( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm JOAAT
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm JOAAT
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_joaat( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_joaat( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_joaat( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny FNV
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny FNV
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_fnv1a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_fnv1a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_mbs_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_wcs_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_mbs_64_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_wcs_64_fnv1a( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny Murmur
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny Murmur
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_murmur3( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_murmur3( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_murmur3( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_murmur2( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_32_murmur( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_murmur2( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm xxHash
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm xxHash
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_xxhash( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_xxhash( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_xxhash( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_xxhash( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_mbs_64_xxhash( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_wcs_64_xxhash( const void* data, size_t length )
