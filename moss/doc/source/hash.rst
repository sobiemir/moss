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

To jest różne od tego (tyczy się wszystkich funkcji)::

    char data[] = "ASDFGhijkl";
    
    uint32_t result1 = ms_hash_32_djb2( data, sizeof(data) );
    uint32_t result2 = ms_hash_32_djb2( data, strlen(data) );
    uint32_t resutl3 = ms_hash_32_djb2( data, sizeof(data) - 1 );

    // result1 jest różny od result2, gdyż sizeof(data) w przypadku tabeli liczy z zerem, które kończy
    // ciąg znaków, strlen go nie wlicza...
    if( result1 != result2 )
        printf( "Poprawnie.\n" );
    if( result2 == result3 )
        printf( "Poprawnie.\n" );

Skrót w przypadku wchar_t uzależniony od rozmiaru typu. Na Windows jest 2 bajty, na Linux 4.
Nie należy przyrównywać stałego haszu dla funkcji obliczających go z typu wchar_t, chyba że mamy pewność że
system na którym będzie program pracował będzie miał taki sam rozmiar typu wchar_t.

Każdy algorytm z rodziny można wyłączyć komentując odpowiednie makro dostępne w konfiguracji.
Makra te można również ustawić bezpośrednio z kompilatora.
To samo tyczy się wersji dla ciągu znaków o typie char i wchar_t.

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny DJB
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny DJB
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_djb2( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Algorytm w tej wersji używa dodawania podczas operacji na bajcie pobieranym ze źródła.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wypisywanej standardowo w postaci szesnastkowej.
    Dostępność funkcji jest uzależniona od makra :c:macro:`MSD_HASH_DJB`, ustawianego w pliku konfiguracyjnym lub
    bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_32_djb2a( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2a.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Algorytm w tej wersji używa alternatywy wykluczającej *(XOR)* podczas operacji na bajcie pobieranym ze źródła.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wypisywanej standardowo w postaci szesnastkowej.
    Dostępność funkcji jest uzależniona od makra :c:macro:`MSD_HASH_DJB`, ustawianego w pliku konfiguracyjnym lub
    bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_djb2( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2.
    Wartość skrótu powinna być taka sama jak wartość zwracana przez funkcję :c:func:`ms_hash_32_djb2`.
    Do obliczeń funkcja nie potrzebuje długości przekazywanych danych, gdyż zakłada, że ciąg danych zakończony
    jest zerem zapisanym na 8 bitach.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_djb2a( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2.
    Wartość skrótu powinna być taka sama jak wartość zwracana przez funkcję :c:func:`ms_hash_32_djb2a`.
    Do obliczeń funkcja nie potrzebuje długości przekazywanych danych, gdyż zakłada, że ciąg danych zakończony
    jest zerem zapisanym na 8 bitach.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_djb2( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2.
    Wartość skrótu powinna być taka sama jak wartość zwracana przez funkcję :c:func:`ms_hash_32_djb2`.
    Do obliczeń funkcja nie potrzebuje długości przekazywanych danych, gdyż zakłada, że ciąg danych zakończony
    jest zerem zapisanym na tylu bitach, ilu bitowy jest typ ``wchar_t``.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_djb2a( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu DJB2.
    Wartość skrótu powinna być taka sama jak wartość zwracana przez funkcję :c:func:`ms_hash_32_djb2a`.
    Do obliczeń funkcja nie potrzebuje długości przekazywanych danych, gdyż zakłada, że ciąg danych zakończony
    jest zerem zapisanym na tylu bitach, ilu bitowy jest typ ``wchar_t``.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :return: Obliczony przez algorytm skrót danych.


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

.. c:function:: uint32_t ms_hash_32_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_64_fnv1a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_mbs_32_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_mbs_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_mbs_64_fnv1a( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_fnv1( const void* data, size_t length )

.. c:function:: uint32_t ms_hash_wcs_32_fnv1a( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_wcs_64_fnv1( const void* data, size_t length )

.. c:function:: uint64_t ms_hash_wcs_64_fnv1a( const void* data, size_t length )


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny Murmur
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny Murmur
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_murmur3( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash3*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_MURMUR`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_32_murmur2( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash2*.
    Z powodu budowy algorytmu, nie jest możliwe utworzenie rozwiązania dedykowanego dla danych, zakończonych
    wartością równą zero (funkcje z dodatkiem *mbs* lub *wcs*).
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_MURMUR`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_32_murmur( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash1*.
    Z powodu budowy algorytmu, nie jest możliwe utworzenie rozwiązania dedykowanego dla danych, zakończonych
    wartością równą zero (funkcje z dodatkiem *mbs* lub *wcs*).
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_MURMUR`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_64_murmur2( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash2*.
    Z powodu budowy algorytmu, nie jest możliwe utworzenie rozwiązania dedykowanego dla danych, zakończonych
    wartością równą zero (funkcje z dodatkiem *mbs* lub *wcs*).
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie jest zoptymalizowany pod kątem działania na procesorach 32 bitowych.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_MURMUR`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_murmur3( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash3*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_murmur3`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_MURMUR` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_murmur3( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *MurmurHash3*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_murmur3`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_MURMUR` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm xxHash
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm xxHash
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_xxhash( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Algorytm ten udostępniony został na licencji `BSD 2-Clause License <http://www.opensource.org/licenses/bsd-license.php>`_.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest zależna od makra :c:macro:`MSD_HASH_XXHASH`, ustawianego w pliku konfiguracyjnym lub
    bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_64_xxhash( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Algorytm ten udostępniony został na licencji `BSD 2-Clause License <http://www.opensource.org/licenses/bsd-license.php>`_.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie jest zoptymalizowany pod kątem procesorów 32 bitowych.
    Dostępność tej funkcji jest zależna od makra :c:macro:`MSD_HASH_XXHASH`, ustawianego w pliku konfiguracyjnym lub
    bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_xxhash( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_xxhash`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_XXHASH` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_mbs_64_xxhash( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_xxhash`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_XXHASH` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_xxhash( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_xxhash`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_XXHASH` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_wcs_64_xxhash( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *xxHash*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_xxhash`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_XXHASH` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.
