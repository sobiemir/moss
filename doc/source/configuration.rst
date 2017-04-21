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
 .  Documentation of Moss Library configuration file.
 .
 .  This file is part of Moss Library.
 .  See LICENSE file for copyright information.
 ..

.. highlight:: c

Ustawienia biblioteki i podstawowe typy
=========================================================

Tablica dynamiczna składająca się z elementów o tym samym typie.
Teoretycznie tablica powinna posiadać wszystkie elementy tego samego typu, jednak jedyne ograniczenie jakie
stawia, to to, aby wszystkie elementy znajdujące się w tablicy były tego samego rozmiaru.
Możliwe jest utworzenie pochodnych tabeli tylko dla konkretnego typu poprzez makra.
Poniższe funkcje w niektórych przypadkach podczas błędu zapisują do zmiennej ``errno`` jego kod.
Wartość tej zmiennej nie jest nadpisywana gdy błąd występuje bezpośrednio w bibliotekach języka *C*.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     STRUKTURY I STAŁE
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Enumeracje
---------------------------------------------------------

.. c:type:: enum MSE_ERROR_CODES

    .. c:member:: MSEC_OK

        0x0000. Kod oznacza że działanie funkcji przebiegło pomyślnie.

    .. c:member:: MSEC_MEMORY_ALLOCATION

    .. c:member:: MSEC_OUT_OF_RANGE

    .. c:member:: MSEC_INVALID_ARGUMENT

    .. c:member:: MSEC_DATA_OVERFLOW

    .. c:member:: MSEC_INVALID_VALUE
