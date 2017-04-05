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

Moduł skupiający zaimplementowane funkcje skrótu w bibliotece, podzielone na rodziny.
Funkcje z konkretnej rodziny można wyłączyć, usuwając lub komentując odpowiednie makro w pliku konfiguracyjnym.
Przynależności makra do rodziny opisana została w szczegółach dotyczących danej funkcji.
Dodatkowo można wyłączyć lub włączyć funkcje dedykowane dla ciągu znaków *mbs* i *wcs*.
Wymienione funkcje dedykowane dla *mbs* i *wcs* to funkcje, gdzie nie podaje się długości przekazywanych danych,
gdyż długość wykrywana jest automatycznie podczas obliczania skrótu. Tablica elementów o typie ``wchar_t`` to specyficzny
ciąg znaków, którego skrót może różnić się w zależności od użytego kompilatora, systemu czy platformy.
Dla tego typu przewidziane zostały dwie wielkości w trakcie implementacji - 16 i 32 bity.

Choć obliczanie skrótu danych jest proste (wystarczy wywołać funkcję), to można natknąć się na interesujące przypadki,
na które należy uważać podczas pracy z funkcjami skrótu. Poniżej wypisane zostały przypadki godne uwagi::

    char     data[] = "HashCalculator";
    wchar_t  wdat[] = L"HashCalculator";

    uint32_t result1_32,
             result2_32;

    printf( "sizeof(char) >> %zu\n", sizeof(char) );

    /* sizeof(data) jest poprawne, gdyż oblicza ilość przydzielonej pamięci na tablicę */
    result1_32 = ms_hash_32_sdbm( data, sizeof(data) );
    result2_32 = ms_hash_mbs_32_sdbm( data );

    /* jednak po sprawdzeniu okaże się, że skrót się nie zgadza */
    if( result1_32 != result2_32 )
        printf( "DATA: Hashes are not equal: %08X != %08X >> sizeof(data)\n",
            result1_32, result2_32 );
    
    /* teraz będzie ok */
    result1_32 = ms_hash_32_sdbm( data, sizeof(data) - 1 );
    if( result1_32 == result2_32 )
        printf( "DATA: Hashes are     equal: %08X == %08X >> sizeof(data) - 1\n",
            result1_32, result2_32 );

    /* można również zrobić to samo funkcją strlen */
    result1_32 = ms_hash_32_sdbm( data, strlen(data) );
    if( result1_32 == result2_32 )
        printf( "DATA: Hashes are     equal: %08X == %08X >> strlen\n",
            result1_32, result2_32 );

    printf( "sizeof(wchar_t) >> %zu\n", sizeof(wchar_t) );

    /* tutaj próba sizeof na tablicy wdat */
    result1_32 = ms_hash_32_sdbm( wdat, sizeof(wdat) );
    result2_32 = ms_hash_wcs_32_sdbm( wdat );

    /* skróty nie będą takie same */
    if( result1_32 != result2_32 )
        printf( "WDAT: Hashes are not equal: %08X != %08X >> sizeof(wdat)\n",
            result1_32, result2_32 );

    /* teraz powinno być ok, jednak ich wartości mogą się różnić w zależności
       od rozmiaru typu wchar_t */
    result1_32 = ms_hash_32_sdbm( wdat, sizeof(wdat) - sizeof(wchar_t) );
    if( result1_32 == result2_32 )
        printf( "WDAT: Hashes are     equal: %08X == %08X >> sizeof(wdat) - sizeof(wchar_t)\n",
            result1_32, result2_32 );

    /* no i klapa, długość musi być podawana w bajtach! */
    result1_32 = ms_hash_32_sdbm( wdat, wcslen(wdat) );
    if( result1_32 != result2_32 )
        printf( "WDAT: Hashes are not equal: %08X != %08X >> wcslen\n",
            result1_32, result2_32 );

    /* tutaj prawidłowo, rozmiar podawany w bajtach */
    result1_32 = ms_hash_32_sdbm( wdat, wcslen(wdat) * sizeof(wchar_t) );
    if( result1_32 == result2_32 )
        printf( "WDAT: Hashes are     equal: %08X == %08X >> wcslen * sizeof(wchar_t)\n",
            result1_32, result2_32 );

Przykładowe wyjście:

.. sourcecode:: none

    sizeof(char) >> 1
    DATA: Hashes are not equal: BEB93E90 != EE3E1D70 >> sizeof(data)
    DATA: Hashes are     equal: EE3E1D70 == EE3E1D70 >> sizeof(data) - 1
    DATA: Hashes are     equal: EE3E1D70 == EE3E1D70 >> strlen
    sizeof(wchar_t) >> 4
    WDAT: Hashes are not equal: 047A83F2 != ACDDB5F2 >> sizeof(wdat)
    WDAT: Hashes are     equal: ACDDB5F2 == ACDDB5F2 >> sizeof(wdat) - sizeof(wchar_t)
    WDAT: Hashes are not equal: 5C414C7C != ACDDB5F2 >> wcslen
    WDAT: Hashes are     equal: ACDDB5F2 == ACDDB5F2 >> wcslen * sizeof(wchar_t)

Aby pobrać wielkość tablicy, w przeciwieństwie do wskaźników, można na niej użyć operatora ``sizeof``.
Zaleta ta ma jeden haczyk, gdyż każdy ciąg znaków winien być zakończony znakiem zera, które wlicza się w długość tablicy.
Dlatego od wartości zwracanej przez operator ``sizeof`` należy odjąć ostatnie zero, aby poprawnie obliczyć
skrót dla ciągu znaków. Jak wcześniej zostało wspomniane, nie należy przyjmować, że skrót dla ciągu z elementami
o typie ``wchar_t`` będzie zawsze taki sam, gdyż jest on uzależniony od rozmiaru tego typu.
Również ze względu na rozmiar danych, skróty dla takich samych *mbs* i *wcs* będą różne.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny DJB
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytmy z rodziny DJB
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_djb2( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Algorytm w tej wersji używa dodawania podczas operacji na bajcie pobieranym ze źródła.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_DJB`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_32_djb2a( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2a*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Algorytm w tej wersji używa alternatywy wykluczającej *(XOR)* podczas operacji na bajcie pobieranym ze źródła.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_DJB`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których tworzony ma być.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_djb2( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_djb2`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_djb2a( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_djb2a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_djb2( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_djb2`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_djb2a( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *DJB2a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_djb2a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_DJB` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm SDBM
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm SDBM
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_sdbm( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu wykorzystywanego w bazie danych *SDBM*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_SDBM`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_sdbm( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu wykorzystywanego w bazie danych *SDBM*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_sdbm`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_SDBM` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_sdbm( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu wykorzystywanego w bazie danych *SDBM*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_sdbm`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_SDBM` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytm JOAAT
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Algorytm JOAAT
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_joaat( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *JOAAT*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_JOAAT`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_joaat( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *JOAAT*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_joaat`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_JOAAT` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_joaat( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *JOAAT*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_joaat`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_JOAAT` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     Algorytmy z rodziny FNV
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Algorytmy z rodziny FNV
---------------------------------------------------------

.. c:function:: uint32_t ms_hash_32_fnv1( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_FNV`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_32_fnv1a( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_FNV`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_64_fnv1( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie jest zoptymalizowany pod kątem działania na procesorach 32 bitowych.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_FNV`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_64_fnv1a( const void* data, size_t length )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Funkcja jest standardową implementacją algorytmu, gdzie do wyliczenia skrótu potrzebny jest rozmiar danych.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie jest zoptymalizowany pod kątem działania na procesorach 32 bitowych.
    Dostępność tej funkcji jest ściśle uzależniona od makra :c:macro:`MSD_HASH_FNV`, ustawianego w pliku konfiguracyjnym
    lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :param length: Długość danych wyrażona w bajtach.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_fnv1( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_fnv1`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_mbs_32_fnv1a( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_fnv1a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_mbs_64_fnv1( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_fnv1`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie został zoptymalizowany pod kątem procesorów 32 bitowych.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_mbs_64_fnv1a( const char* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_fnv1a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisaną na 8 bitach.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie został zoptymalizowany pod kątem procesorów 32 bitowych.
    Dostępność funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_MBS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_fnv1( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_fnv1`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint32_t ms_hash_wcs_32_fnv1a( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_32_fnv1a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 32 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_wcs_64_fnv1( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_fnv1`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie został zoptymalizowany pod kątem procesorów 32 bitowych.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.

.. c:function:: uint64_t ms_hash_wcs_64_fnv1a( const wchar_t* data )

    Oblicza skrót danych podanych w parametrze, na podstawie algorytmu *FNV-1a*.
    Wartość zwracanego skrótu jest taka sama jak w przypadku funkcji :c:func:`ms_hash_64_fnv1a`.
    Funkcja do obliczeń nie potrzebuje rozmiaru przekazywanych danych, gdyż zakłada, że ich koniec oznaczany jest
    przez wartość równą zero, zapisywaną na 16 lub 32 bitach, w zależności od rozmiaru typu ``wchar_t``.
    Skrót danych zwracany jest w postaci 64 bitowej liczby, wyświetlanej standardowo w postaci szesnastkowej.
    Użyty algorytm nie został zoptymalizowany pod kątem procesorów 32 bitowych.
    Dostępność tej funkcji jest uzależniona od makr :c:macro:`MSD_HASH_FNV` oraz :c:macro:`MSD_HASH_WCS_FUNCTIONS`,
    ustawianych w pliku konfiguracyjnym lub bezpośrednio w kompilatorze.

    .. rst-class:: parameters

    :param data: Dane na podstawie których ma być tworzony skrót.
    :return: Obliczony przez algorytm skrót danych.


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

.. c:function:: uint32_t ms_hash_32_murmur1( const void* data, size_t length )

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

.. container:: gitlinks

    * `Pokaż nagłówek modułu <https://raw.githubusercontent.com/sobiemir/moss/master/moss/inc/hash.h>`_
    * `Pokaż źródło rodziny DJB <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/djb.c>`_
    * `Pokaż źródło rodziny SDBM <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/sdbm.c>`_
    * `Pokaż źródło rodziny JOAAT <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/joaat.c>`_
    * `Pokaż źródło rodziny FNV <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/fnv.c>`_
    * `Pokaż źródło rodziny Murmur <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/murmur.c>`_
    * `Pokaż źródło rodziny xxHash <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/hash/xxhash.c>`_
