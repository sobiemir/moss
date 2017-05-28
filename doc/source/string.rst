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
 .  Documentation file for String module.
 .
 .  This file is part of Moss Library.
 .  See LICENSE file for copyright information.
 ..

.. highlight:: c

Tekst uniwersalny
=========================================================

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     STRUKTURY I STAŁE
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Struktury
---------------------------------------------------------

.. c:type:: MS_STRING

    Struktura ciągu znaków, do której odwołują się wszystkie funkcje w module.
    Każde wywołanie funkcji tworzącej strukturę musi być wieńczone funkcją zwalniającą zasoby
    przydzielone dla obiektu, aby zapobiec wyciekom pamięci.
    Jest to spowodowane tym, że funkcje tworzące przydzielają pamięć dla unii oraz, w zależności od typu ciągu znaków,
    również dla pola zawierającego informacje o znakach w ciągu wielobajtowym.
    Szczegóły na temat struktury znajdują się w opisach poszczególnych pól.

    Wartości domyślne pól struktury ustawiane podczas tworzenia tablicy:

    ===================== ==========================================
    Pole                  Wartość domyślna
    ===================== ==========================================
    Data                  unia
    Length                obliczany
    Capacity              parametr lub obliczany
    Hash                  0
    Modifier              2.f
    Wide                  automatycznie
    Destroy               automatycznie
    Hashed                false
    MBInfo                automatycznie
    FuncIncrease          :c:member:`MSS_ARRAYFUNCTIONS.IncMultiply`
    ===================== ==========================================

    .. c:member:: union Data

        Unia dla trzech typów znaków, z czego jeden z nich jest typem automatycznym.
        Struktura dzięki temu polu jest uniwersalna, co oznacza, że może się w niej znajdować ciąg znaków
        typu ``char`` lub ``wchar_t``, ale nigdy oba na raz.
        Wszystkie elementy w unii zajmują tą samą pamięć, więc możliwy jest bezkonfliktowy dostęp tylko do jednego pola.

        .. c:member:: char* Char

            Ciąg znaków typu ``char``.
            Pole to jest wykorzystywane w przypadku, gdy ciąg znaków posiada znaki jednobajtowe lub wielobajtowe.
            Dzięki temu polu, do ciągu można dostać się bezpośrednio bez rzutowania danych na typ.
        
        .. c:member:: wchar_t* Wide

            Ciąg znaków typu ``wchar_t``.
            Pole to jest wykorzystywane w przypadku, gdy ciąg znaków posiada znaki o typie ``wchar_t``.
            Rozmiar znaków może być różny w zależności od systemu, więc mogą to być 2 lub 4 bajty.
            Dzięki temu polu, do ciągu można dostać się bezpośrednio bez rzutowania danych na typ.

        .. c:member:: tchar Auto

            Ciąg znaków o typie automatycznym.
            Generalnie pole to nie jest wykorzystywane przez bibliotekę.
            Może być wykorzystane przez użytkownika, używającego typu ``tchar``, który przyjmuje typ ``char``
            lub ``wchar_t`` w zależności od ustawień biblioteki.
            Dzięki temu polu, do ciągu można dostać się bezpośrednio bez rzutowania danych na typ.

    .. c:member:: size_t Length

        Ilość znaków w ciągu.
        Wartość ta reprezentuje aktualną długość tablicy i jest porównywana z pojemnością podczas dodawania znaków.
        Każdy ciąg znaków powinien kończyć się znakiem ``\0``, który reprezentuje wartość ``NULL``.
        Samodzielna zmiana tej wartości może wpływać na wyświetlanie i operacje na ciągu, dlatego nie należy jej
        modyfikować.

    .. c:member:: size_t Capacity

        Pojemność tablicy, zawierającej ciąg znaków.
        Wartość ta odpowiada za ilość znaków, które mogą się jeszcze zmieścić w ciągu bez ponownego przydzielania
        miejsca na dodawane znaki.
        Wartość ta jest porównywana z wartością pola :c:member:`MS_STRING.Length`.
        Gdy wartość jest za mała, uruchamiany zostaje podany *inkreator*, czyli funkcja zawierająca sposób obliczania
        nowej pamięci do alokacji.
        Wartość ta zarządzana jest wyłącznie przez bibliotekę i nie może być zmieniana samodzielnie.

    .. c:member:: size_t Hash

        Skrót ciągu znaków w postaci 32 lub 64 bitowej liczby naturalnej.
        Ilość bitów dla skrótu można zmienić w ustawieniach biblioteki.
        Skrót wykorzystywany jest przy porównywaniu dwóch struktur, zawierających tablice znaków.
        Najczęściej przedstawiany jest w postaci szesnastkowej.
        Skrót tworzony jest bezpośrednio ze znaków i powinien być zmieniany w trakcie ich modyfikacji.
        Choć skrót powinien być unikalny dla dwóch różnych ciągów znaków, to niestety możliwe są konflikty.
        Stosowanie odpowiedniej funkcji skrótu zmniejsza ryzyko ich wystąpienia, ale nie niweluje go w całości.
        Domyślną funkcją skrótu jest funkcja o nazwie :c:func:`ms_hash_32_sdbm`.
        Wartość tego pola zmieniana jest automatycznie.

    .. c:member:: float Modifier

        Modyfikator kontrolujący powiększenie pojemności tablicy zawierającej ciąg znaków.
        Zastosowanie modyfikatora, jak i również przyjmowane przez niego wartości, są uzależnione od aktualnie
        ustawionej funkcji zwiększającej pojemność tablicy, do której zmienna jest zawsze przekazywana.
        Ustawienie złej wartości może spowodować błędne obliczenia nowej pojemności w *inkreatorze*.

    .. c:member:: bool Wide

        Informacja o tym, czy ciąg posiada znaki o typie ``wchar_t``.
        Znaki o tym typie są znakami zapisanymi zawsze na tylu bajtach, ilu bajtowy jest typ ``wchar_t``.
        W przypadku znaków o typie ``char``, ilość bajtów składających się na znak zależy od użytego kodowania.
        Wartość tego pola ustawiana jest automatycznie i nie może być zmieniana samodzielnie.

    .. c:member:: bool Destroy

        Informacja o tym, czy zmienna przechowująca strukturę ciągu znaków ma zostać zniszczona.
        Pole to wykorzystywane jest **tylko i wyłącznie** w funkcji :c:func:`ms_string_free`, odpowiedzialnej za zwalnianie
        przydzielonych zasobów przeznaczonych na ciąg znaków.
        Wartość ustawiana jest na *TRUE* tylko w funkcjach, zwracających wskaźnik do struktury ciągu.
        Samodzielna zmiana tej wartości może prowadzić do wycieków lub naruszenia ochrony pamięci.

    .. c:member:: bool Hashed

        Informacja o tym, czy dla zawartości struktury utworzony został skrót.
        Do sprawdzenia czy skrót został utworzony, potrzeba osobnej zmiennej.
        Jest to konieczne, ponieważ wartość 0 również może być skrótem danych, zawartych w strukturze, więc każda
        wartość dla pola :c:member:`MS_STRING.Hash` jest poprawna.
        Wartość tego pola zmieniana jest automatycznie podczas jakiejkolwiek zmiany w strukturze.
        Można jednak samodzielnie ustawić wartość na *FALSE*, aby wymusić ponowne utworzenie skrótu dla struktury.

    .. c:member:: MS_ARRAY* MBInfo

        Tablica zawierająca szczegóły dotyczące każdego znaku w ciągu.
        Pole uzupełniane jest tylko w przypadku gdy ciąg posiada znaki wielobajtowe.
        W przeciwnym wypadku pole zawiera wartość ``NULL``.
        Tablica ta wykorzystywana jest podczas podstawowych operacji na znakach, gdzie pobierany jest lub potrzebny
        indeks znaku lub cały zakres znaków do modyfikacji.
        Ilość elementów w tablicy jest różna od ilości znaków w ciągu, obliczanych za pomocą funkcji :c:func:`strlen`.
        Ilość elementów odpowiada rzeczywistej ilości znaków, wyświetlanych na ekranie, dzięki czemu w prosty sposób
        można się dostać do indeksu modyfikowanego znaku i jego rozmiaru w bajtach.
        Zarówno indeks jak i rozmiar obliczane są funkcją :c:func:`mblen`.
        Aby funkcja ta działała poprawnie, należy ustawić poprawną lokalizację w programie, wykorzystując funkcję
        :c:func:`setlocale`.

    .. c:member:: size_t FuncIncrease(size_t capacity, float modifier)

        Funkcja zwana inaczej *inkreatorem*, zwiększająca pojemność tablicy, używana podczas przydzielania pamięci
        dla jej nowych elementów.
        Dzięki temu polu można przypisać własną funkcję obliczającą nową ilość pamięci.
        Wszystkie wskaźniki wbudowanych *inkreatorów* znajdują się w stałej :c:data:`MSC_ArrayFunctions`
        mając w nazwie przedrostek *Inc*. Funkcje te zawsze po obliczeniach zaokrąglają wynik w dół.
        Do wnętrza funkcji pod parametrem ``modifier`` przekazywana jest wartość pola :c:member:`MS_STRING.Modifier`.
        Zmienna ta może być zarówno dodatnia jak i ujemna, jednak wynik końcowy teoretycznie nie może
        być mniejszy niż wprowadzony do funkcji w zmiennej ``capacity``.
        W praktyce jednak, w przypadku gdy funkcja zwraca wynik o mniejszej wartości niż aktualna pojemność tablicy,
        nowa ustawiona zostaje jako ``capacity + 1``.

        Zestawienie wbudowanych funkcji inkreatora i ich wyniki przy takich samych parametrach:

        ===================== =========== =========== ============
        Funkcja inkreatora    Pojemność   Mnożnik     Wynik
        ===================== =========== =========== ============
        IncAdd                3           4.5         7
        IncMultiply           3           4.5         13
        IncPower              3           4.5         140
        ===================== =========== =========== ============

        .. rst-class:: parameters

        :param capacity: Aktualna pojemność ciągu znaków.
        :param modifier: Modyfikator pojemności.
        :return: Nową pojemność ciągu znaków, przekazywaną do przydzielenia pamięci.

.. c:type:: MS_MBINFO

    Struktura przechowująca informacje o pojedynczym znaku w wielobajtowym ciągu znaków.
    Ciągi wielobajtowe mają to do siebie, że mogą reprezentować pojedynczy znak na kilku bajtach.
    Zatem każdy znak w ciągu może posiadać rzeczywistą długość zależną od użytego kodowania.
    Poprzez długość rzeczywistą ma się rozumieć ilość znaków, wyświetlanych na ekranie.
    Struktura została utworzona, aby przyspieszyć niektóre operacje, wykonywane na wielobajtowych ciągach znaków.
    Używana w strukturze :c:type:`MS_STRING`.
    Aby informacje w strukturze były poprawne, należy wcześniej ustawić odpowiednią lokalizację, używając funkcji
    :c:func:`setlocale`.

    .. c:member:: size_t Offset

        Prawdziwy indeks, na którym leży znak w ciągu.
        Indeks ten jest sumą ilości bajtów wszystkich znaków, poprzedzających aktualny.
        Pole reprezentuje przesunięcie od którego wielobajtowy ma się rozpoczynać.

    .. c:member:: char Bytes

        Ilość bajtów przypadająca na sprawdzany znak.
        Znaki w wielobajtowych ciągach w zależności od kodowania mogą być różnej długości.
        Względem powyższego stwierdzenia, to samo kodowanie potrafi zakodować znak **n** na 1 bajcie,
        zaś znak **ń** już 2 bajtach.
        To pole, wraz z polem :c:member:`MS_MBINFO.Offset`, umożliwia obliczenie zakresu, który okupuje
        pojedynczy znak w ciągu.
        Wartość pola obliczana jest poprzez funkcję :c:func:`mblen`.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     TWORZENIE I INICJALIZACJA
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Tworzenie i inicjalizacja
---------------------------------------------------------

.. c:function:: MS_STRING* ms_string_alloc_cs( const char* cstr, size_t capacity )

    Tworzy nowy ciąg jednobajtowych znaków o typie ``char`` oraz rezerwuje początkowe miejsce na dane.
    W odróżnieniu od inicjalizacji, funkcja zwraca wskaźnik do utworzonego ciągu.
    W przypadku błędu podczas tworzenia, zwrócona zostaje wartość *NULL*.
    Funkcja jako jedna z nielicznych ustawia pole :c:member:`MS_STRING.Destroy` na wartość *TRUE*.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.

    .. rst-class:: parameters

    :param cstr: Początkowy ciąg jednobajtowych znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Wskaźnik na utworzony ciąg lub wartość *NULL* w przypadku błędu.

.. c:function:: MS_STRING* ms_string_alloc_mbs( const char* mbstr, size_t capacity )

    Tworzy nowy ciąg znaków wielobajtowych o typie ``char`` oraz rezerwuje początkowe miejsce na dane.
    W odróżnieniu od inicjalizacji, funkcja zwraca wskaźnik do utworzonego ciągu.
    W przypadku błędu podczas tworzenia, zwrócona zostaje wartość *NULL*.
    Funkcja dodatkowo tworzy tablicę informacyjną dla znaków w ciągu dla pola :c:member:`MS_STRING.MBInfo`.
    Znajdują się w niej informacje o położeniu i rozmiarze każdego znaku w ciągu.
    Funkcja jako jedna z nielicznych ustawia pole :c:member:`MS_STRING.Destroy` na wartość *TRUE*.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.
    Wartość ``capacity`` dla tej funkcji nie oznacza tak jak w przypadku pozostałych funkcji tworzących
    ilość znaków, lecz ilość bajtów, które może zajmować ciąg znaków.
    Jest to spowodowane tym, że znaki tworzące mogą być zarówno 1 jak i 3 bajowe.

    .. rst-class:: parameters

    :param mbstr: Początkowy ciąg wielobajtowych znaków.
    :param capacity: Początkowa ilość bajtów rezerwowanego miejsca na znaki.
    :return: Wskaźnik na utworzony ciąg lub wartość *NULL* w przypadku błędu.

.. c:function:: MS_STRING* ms_string_alloc_wcs( const wchar_t* wcstr, size_t capacity )

    Tworzy nowy ciąg znaków o typie ``wchar_t`` oraz rezerwuje początkowe miejsce na dane.
    W odróżnieniu od inicjalizacji, funkcja ta zwraca wskaźnik do utworzonego ciągu.
    W przypadku błędu podczas tworzenia, zwracana jest wartość *NULL*.
    Funkcja jako jedna z nielicznych ustawia pole :c:member:`MS_STRING.Destroy` na wartość *TRUE*.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`wcslen`.

    .. rst-class:: parameters

    :param wcstr: Początkowy ciąg znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Wskaźnik na utworzony ciąg lub wartość *NULL* w przypadku błędu.

.. c:function:: int ms_string_init_cs( MS_STRING* str, const char* cstr, size_t capacity )

    Inicjalizuje istniejącą strukturę ciągu jednobajtowych znaków i rezerwuje początkowe miejsce na dane.
    Wszystkie funkcje tworzące ciąg jednobajtowych znaków odwołują się bezpośrednio do tej funkcji.
    W przypadku błędu zwracany jest jego kod, w przeciwnym razie wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.

    .. rst-class:: parameters

    :param str: Struktura ciągu znaków do uzupełnienia.
    :param cstr: Początkowy ciąg jednobajtowych znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: int ms_string_init_mbs( MS_STRING* str, const char* mbstr, size_t capacity )

    Inicjalizuje istniejącą strukturę ciągu znaków wielobajtowych i rezerwuje początkowe miejsce na dane.
    Wszystkie funkcje tworzące ciąg wielobajtowych znaków odwołują się bezpośrednio do tej funkcji.
    Funkcja dodatkowo tworzy tablicę informacyjną dla znaków w ciągu dla pola :c:member:`MS_STRING.MBInfo`.
    Znajdują się w niej informacje o położeniu i rozmiarze każdego znaku w ciągu.
    W przypadku błędu zwracany jest jego kod, w przeciwnym razie wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.
    Wartość ``capacity`` dla tej funkcji nie oznacza tak jak w przypadku pozostałych funkcji tworzących
    ilość znaków, lecz ilość bajtów, które może zajmować ciąg znaków.
    Jest to spowodowane tym, że znaki tworzące mogą być zarówno 1 jak i 3 bajowe.

    .. rst-class:: parameters

    :param str: Struktura ciągu znaków do uzupełnienia.
    :param mbstr: Początkowy ciąg wielobajtowych znaków.
    :param capacity: Początkowa ilość bajtów rezerwowanego miejsca na znaki.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: int ms_string_init_wcs( MS_STRING* str, const wchar_t* wcstr, size_t capacity )

    Inicjalizuje istniejącą strukturę ciągu znaków o typie ``wchar_t`` i rezerwuje początkowe miejsce na dane.
    Wszystkie funkcje tworzące ciąg znaków o typ typie odwołują się bezpośrednio do tej funkcji.
    W przypadku błędu zwracany jest jego kod, w przeciwnym razie wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_string_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`wcslen`.

    .. rst-class:: parameters

    :param str: Struktura ciągu znaków do uzupełnienia.
    :param wcstr: Początkowy ciąg znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: MS_STRING ms_string_return_cs( const char *cstr, size_t capacity )

    Tworzy nowy ciąg jednobajtowych znaków o typie ``char`` oraz rezerwuje początkowe miejsce na dane.
    Odmiana tej funkcji pozwala na ustawienie rozmiaru pojedynczego elementu.
    W przypadku błędu podczas tworzenia, każde pole unii :c:member:`MS_STRING.Data` jest równe *NULL*.
    Utworzony w ten sposób ciąg znaków nadal wymaga zwolnienia przydzielonych zasobów poprzez wywołanie funkcji
    :c:func:`ms_array_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.

    .. rst-class:: parameters

    :param cstr: Początkowy ciąg jednobajtowych znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Utworzony lokalny ciąg znaków.

.. c:function:: MS_STRING ms_string_return_mbs( const char *mbstr, size_t capacity )

    Tworzy nowy ciąg znaków wielobajtowych o typie ``char`` oraz rezerwuje początkowe miejsce na dane.
    Odmiana tej funkcji pozwala na ustawienie rozmiaru pojedynczego elementu.
    W przypadku błędu podczas tworzenia, każde pole unii :c:member:`MS_STRING.Data` jest równe *NULL*.
    Funkcja dodatkowo tworzy tablicę informacyjną dla znaków w ciągu dla pola :c:member:`MS_STRING.MBInfo`.
    Znajdują się w niej informacje o położeniu i rozmiarze każdego znaku w ciągu.
    Utworzony w ten sposób ciąg znaków nadal wymaga zwolnienia przydzielonych zasobów poprzez wywołanie funkcji
    :c:func:`ms_array_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`strlen`.
    Wartość ``capacity`` dla tej funkcji nie oznacza tak jak w przypadku pozostałych funkcji tworzących
    ilość znaków, lecz ilość bajtów, które może zajmować ciąg znaków.
    Jest to spowodowane tym, że znaki tworzące mogą być zarówno 1 jak i 3 bajowe.

    .. rst-class:: parameters

    :param mbstr: Początkowy ciąg wielobajtowych znaków.
    :param capacity: Początkowa ilość bajtów rezerwowanego miejsca na znaki.
    :return: Utworzony lokalny ciąg znaków.

.. c:function:: MS_STRING ms_string_return_wcs( const wchar_t *wcstr, size_t capacity )

    Tworzy nowy ciąg znaków o typie ``wchar_t`` oraz rezerwuje początkowe miejsce na dane.
    Odmiana tej funkcji pozwala na ustawienie rozmiaru pojedynczego elementu.
    W przypadku błędu podczas tworzenia, każde pole unii :c:member:`MS_STRING.Data` jest równe *NULL*.
    Utworzony w ten sposób ciąg znaków nadal wymaga zwolnienia przydzielonych zasobów poprzez wywołanie funkcji
    :c:func:`ms_array_free`.
    Podanie wartości 0 lub mniejszej niż długość podanego ciągu znaków w miejsce parametru ``capacity``
    powoduje zmianę tej wartości przez funkcję na długość ciągu, obliczonego funkcją :c:func:`wcslen`.

    .. rst-class:: parameters

    :param wcstr: Początkowy ciąg znaków.
    :param capacity: Początkowa ilość rezerwowanego miejsca na znaki.
    :return: Utworzony lokalny ciąg znaków.

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     ZMIANA POJEMNOŚCI
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Zmiana pojemności
---------------------------------------------------------

.. c:function:: int ms_string_realloc( MS_STRING* str, size_t capacity )

    Zmniejsza lub zwiększa pojemność ciągu znaków.

    Funkcja posiada dwa wbudowane tryby zwiększania pojemności - automatyczny i ręczny.
    Tryb automatyczny można uruchomić, podając pod zmienną ``capacity`` wartość 0.
    Obliczaniem pojemności w takim wypadku zajmuje się funkcja *inkreatora* podpięta pod przekazaną tablicę do pola
    :c:member:`MS_STRING.FuncIncrease`.
    Gdy *inkreator* nie jest podpięty, zwracany jest błąd, gdyż funkcja nie wie ile ma przydzielić pamięci.
    W przypadku zwrócenia przez *inkreator* pojemności mniejszej niż aktualna, wartość ta jest odrzucana i jako
    nowa przyjmowana jest suma ``capacity + 1``.
    Tryb ręczny uruchamiany jest w przypadku wpisania wartości innej niż 0 w parametrze ``capacity``.
    W tym trybie pojemność może być zarówno zwiększana jak i zmniejszana.
    Próba przydziału pojemności mniejszej niż ilość znaków zapisanych w ciągu skutkuje błędem i natychmiastowym
    zakończeniem działania funkcji.
    W przypadku ciągu znaków wielobajtowych o typie ``char``, parametr ``capacity`` oznacza ilość bajtów a nie ilość
    znaków, jak to ma miejsce w przypadku innych ciągów.

    .. rst-class:: parameters

    :param str: Wskaźnik na strukturę ciągu.
    :param capacity: Nowa pojemność ciągu lub 0 w przypadku automatu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
            * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_VALUE`
            * :c:member:`MSE_ERROR_CODES.MSEC_DATA_OVERFLOW`

.. c:function:: int ms_string_realloc_min( MS_STRING* str, size_t min )

    Zwiększa pojemność ciągu do co najmniej podanej wartości.

    Zwiększa pojemność ciągu znaków do najbliższej wartości następującej po wartości podanej w parametrze ``min``.
    Podanie wartości mniejszej niż aktualna pojemność nie kończy się błędem, ale również nie zmienia
    pojemności całego ciągu, ponieważ minimum zostało już osiągnięte.
    Funkcja wywołuje *inkreatora* dopóty, dopóki wartość przez niego zwracana nie będzie większa lub
    równa wartości zmiennej ``min``, przekazanej w parametrze.
    W przypadku gdy pole :c:member:`MS_STRING.FuncIncrease` będzie równe wartości *NULL*, jako nowa pojemność
    przyjęta zostanie wartość minimalna.
    Funkcja przydaje się szczególnie w trakcie wstawiania wielu znaków, gdzie za jednym razem przydzielana jest
    odpowiednia ilość pamięci, która pozwoli zmieścić wszystkie znaki w ciągu i w miarę możliwości pozostawić miejsce
    na nowe.

    .. rst-class:: parameters

    :param str: Wskaźnik na strukturę ciągu.
    :param min: Minimalna wartość do której zwiększona zostanie pojemność ciągu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     DODAWANIE ELEMENTÓW
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Dodawanie elementów
---------------------------------------------------------

.. c:function:: int ms_string_insert_cs( MS_STRING* str, size_t index, const char *cstr, size_t count )

    Wstawia tekst o znakach jednobajtowych do podanego ciągu w wybrane miejsce.

    Funkcja pozwala na wstawienie podanego tekstu do przekazanego w parametrze ciągu znaków.
    Gdy sposób zapisu dwóch tekstów się różni, wstawiany tekst konwertowany jest automatycznie do sposobu zapisu
    tekstu uniwersalnego.
    Pojemność ciągu zwiększana jest automatycznie w przypadku gdy brakuje miejsca dla wstawianych danych.
    Podanie indeksu różnego od długości ciągu powoduje przesunięcie elementów następujących po indeksie dokładnie
    o ilość znaków podawanych w parametrze ``count`` lub o całą długość wstawianego tekstu.

    Można ograniczyć ilość wstawianych znaków, podając ją w parametrze ``count``.
    Gdy do parametru przekazana zostanie wartość 0, funkcja wstawia do ciągu cały podany tekst.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param index: Indeks od którego dane będą wstawiane.
    :param cstr: Standardowy teskt o znakach jednobajtowych.
    :param count: Ilość znaków do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
            * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: int ms_string_insert_mbs( MS_STRING* str, size_t index, const char *mbstr, size_t bytes )

    Wstawia tekst o znakach wielobajtowych do podanego ciągu w wybrane miejsce.

    Funkcja pozwala na wstawienie podanego tekstu do przekazanego w parametrze ciągu znaków.
    Gdy sposób zapisu dwóch tekstów się różni, wstawiany tekst konwertowany jest automatycznie do sposobu zapisu
    tekstu uniwersalnego.
    Pojemność ciągu zwiększana jest automatycznie w przypadku gdy brakuje miejsca dla wstawianych danych.
    Podanie indeksu różnego od długości ciągu powoduje przesunięcie elementów następujących po indeksie dokładnie
    o ilość bajtów podawanych w parametrze ``bytes`` lub o cały rozmiar wstawianego tekstu.

    Można ograniczyć ilość wstawianych bajtów, podając ją w parametrze ``bytes``.
    Gdy do parametru przekazana zostanie wartość 0, funkcja wstawia do ciągu cały podany tekst.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param index: Indeks od którego dane będą wstawiane.
    :param mbstr: Standardowy teskt o znakach wielobajtowych.
    :param bytes: Ilość bajtów do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
            * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: int ms_string_insert_wcs( MS_STRING* str, size_t index, const wchar_t *wcstr, size_t count )

    Wstawia tekst o znakach unicode do podanego ciągu w wybrane miejsce.

    Funkcja pozwala na wstawienie podanego tekstu do przekazanego w parametrze ciągu znaków.
    Gdy sposób zapisu dwóch tekstów się różni, wstawiany tekst konwertowany jest automatycznie do sposobu zapisu
    tekstu uniwersalnego.
    Pojemność ciągu zwiększana jest automatycznie w przypadku gdy brakuje miejsca dla wstawianych danych.
    Podanie indeksu różnego od długości ciągu powoduje przesunięcie elementów następujących po indeksie dokładnie
    o ilość znaków podawanych w parametrze ``count`` lub o całą długość wstawianego tekstu.

    Można ograniczyć ilość wstawianych znaków, podając ją w parametrze ``count``.
    Gdy do parametru przekazana zostanie wartość 0, funkcja wstawia do ciągu cały podany tekst.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param index: Indeks od którego dane będą wstawiane.
    :param wcstr: Standardowy teskt o znakach unicode.
    :param count: Ilość znaków do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
            * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     CZYSZCZENIE DANYCH
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Czyszczenie danych
---------------------------------------------------------

.. c:function:: void ms_string_clear( MS_STRING* str )

    Czyści ciąg usuwając wszystkie znaki.

    Funkcja nie zwalnia pamięci po elementach i nie zmniejsza aktualnej pojemności tablicy dla ciągu.
    Dzięki temu wprowadzanie kolejnych znaków nie uruchamia ponownie funkcji zwiększającej pamięć dla zmiennej.
    Pojemność ciągu znaków zawsze można zmniejszyć samodzielnie, wywołując funkcję :c:func:`ms_string_realloc`.

    Przykład użycia funkcji::

        MS_STRING str = ms_string_return_cs( "Local string", 0 );

        printf( "Chars: %" PFSIZET " >> Capacity: %" PFSIZET "\n",
            str.Length, str.Capacity );
        ms_string_clear( &str );
        printf( "Chars: %" PFSIZET " >> Capacity: %" PFSIZET "\n",
            str.Length, str.Capacity );

        ms_string_free( &str );

    .. rst-class:: output
    .. code-block:: none

        Chars: 12 >> Capacity: 13
        Chars: 0 >> Capacity: 13

    .. rst-class:: parameters

    :param aptr: Wskaźnik na ciąg znaków.

.. c:function:: void ms_string_free( MS_STRING* str )

    Zwalnia pamięć przydzieloną przez funkcję tworzącą ciąg znaków.

    Każdy blok kodu w którym wystąpiło wywołanie konstruktora ciągu znaków, niezależnie od tego czy struktura
    jest lokalna czy nie, powinien uruchamiać funkcję czyszczącą pamięć przydzieloną przez konstruktor.
    Pominięcie tego kroku w danym bloku lub w całym programie przed jego zakończeniem spowoduje wyciek pamięci.
    Informacje dotyczące wykrywania wycieków pamięci w programie znajdują się w rozdziale zatytułowanym
    ":ref:`MemoryLeaks`".

    Przykład użycia funkcji::

        MS_STRING  str1 = ms_string_return_wcs( L"Żółta gęś", 0 );
        MS_STRING *str2 = ms_string_alloc_mbs( "Zielona paproć", 0 );

        ms_string_free( &str1 );
        ms_string_free( str2 );

        str2 = NULL;

    .. rst-class:: parameters

    :param aptr: Wskaźnik na ciąg znaków.

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     OBLICZENIA I INFORMACJE
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Obliczenia i informacje
---------------------------------------------------------

.. c:function:: size_t ms_string_length_mbs( const char* mbstr, size_t bytes )

    Oblicza ilość znaków w ciągu wielobajtowym.

    Ciąg wielobajtowy typu ``char`` może posiadać znaki o różnych rozmiarach, dlatego funkcja :c:func:`strlen`
    nie jest wystarczająca do sprawdzenia ich ilości.
    Do sprawdzenia ilości znaków wewnątrz funkcji używana jest funkcja :c:func:`mblen`, wywoływana dla każdego
    znaku w ciągu.

    Ilość sprawdzanych znaków w ciągu można ograniczyć, podając rozmiar do drugiego parametru.
    W przypadku podania błędnego rozmiaru, funkcja zwróci wartość 0.
    Podanie wartości 0 w ostatnim parametrze jest równoznaczne z podaniem długości obliczonej funkcją :c:func:`strlen`.
    Gdy ciąg znaków zakończony jest znakiem ``\0`` oraz w parametrze ``bytes`` podana będzie wartość większa
    niż ilość bajtów ciągu, obliczenia zostaną przerwane zaraz po napotkaniu znaku ``\0``.

    Przykład użycia funkcji::

        int retval;
        MS_STRING str;

        size_t len1, len2, len3, len4,
               byt1, byt2, byt3, byt4;

        /* locale must be set */
        setlocale( LC_ALL, "pl_PL.utf8" );

        if( (retval = ms_string_init_mbs(&str, "Zażółć gęślą jaźń", 0)) )
            return retval;

        len1 = ms_string_length_mbs( str.Data.Char, 0 );
        len2 = ms_string_length_mbs( "Żółte źdźbła trawy na koszyku wiklinowym.", 0 );
        len3 = ms_string_length_mbs( "Jámbor célú, öv ügyű ex-qwan ki dó-s főz.", 0 );

        /* limit - only 26 bytes */
        len4 = ms_string_length_mbs( "Jámbor célú, öv ügyű ex-qwan ki dó-s főz.", 26 );

        byt1 = strlen( str.Data.Char );
        byt2 = strlen( "Żółte źdźbła trawy na koszyku wiklinowym." );
        byt3 = strlen( "Jámbor célú, öv ügyű ex-qwan ki dó-s főz." );
        byt4 = strlen( "Jámbor célú, öv ügyű" );

        printf( "POL/1: %" PFSIZET " chars and %" PFSIZET " bytes.\n", len1, byt1 );
        printf( "POL/2: %" PFSIZET " chars and %" PFSIZET " bytes.\n", len2, byt2 );
        printf( "HUN/3: %" PFSIZET " chars and %" PFSIZET " bytes.\n", len3, byt3 );
        printf( "HUN/4: %" PFSIZET " chars and %" PFSIZET " bytes.\n", len4, byt4 );

        ms_string_free( &str );

    .. rst-class:: output
    .. sourcecode:: none

        POL/1: 17 chars and 26 bytes.
        POL/2: 41 chars and 47 bytes.
        HUN/3: 41 chars and 49 bytes.
        HUN/4: 20 chars and 26 bytes.

    .. rst-class:: parameters

    :param mbstr: Wielobajtowy ciąg znaków.
    :param bytes: Ilość bajtów do sprawdzenia.
    :return: Ilość znaków w ciągu.

.. c:function:: size_t ms_string_length_wcstombs( const wchar_t* wstr, size_t count )

    Oblicza ilość bajtów zajmowanych przez ciąg po konwersji na *mbs*.

    Ciąg wielobajtowy typu ``char`` po konwersji może zajmować więcej niż sam ciąg złożony ze znaków ``wchar_t``.
    Dlatego funkcja :c:func:`wcslen` nie jest wystarczająca przy przewidywaniu rozmiaru ciągu po konwersji.
    Podczas obliczania długości ciągu, funkcja konwertuje znaki używając funkcji :c:func:`wctomb`, która zwraca
    rozmiar konwertowanego znaków w bajtach.

    Długość ciągu można ograniczyć poprzez podanie ilości konwertowanych znaków w drugim parametrze.
    Podanie wartości 0 jest równoznaczne z podaniem długości całego ciągu, obliczonej funkcją :c:func:`wcslen`.
    Gdy ciąg znaków zakończony jest znakiem ``\0`` oraz w parametrze ``count`` podana będzie wartość większa
    niż ilość znaków w ciągu, obliczenia zostaną przerwane zaraz po napotkaniu znaku ``\0``.

    Przykład użycia funkcji::

        int retval;
        MS_STRING str;

        size_t len1, len2, len3, len4,
               byt1, byt2, byt3, byt4;

        /* locale must be set */
        setlocale( LC_ALL, "pl_PL.utf8" );

        if( (retval = ms_string_init_wcs(&str, L"Zażółć gęślą jaźń", 0)) )
            return retval;

        len1 = ms_string_length_wcstombs( str.Data.Wide, 0 );
        len2 = ms_string_length_wcstombs( L"Żółte źdźbła trawy na koszyku wiklinowym.", 0 );
        len3 = ms_string_length_wcstombs( L"Эх, чужак, общий съём цен шляп.", 0 );

        /* limit - only 16 characters */
        len4 = ms_string_length_wcstombs( L"Эх, чужак, общий съём цен шляп.", 16 );

        byt1 = wcslen( str.Data.Wide );
        byt2 = wcslen( L"Żółte źdźbła trawy na koszyku wiklinowym." );
        byt3 = wcslen( L"Эх, чужак, общий съём цен шляп." );
        byt4 = wcslen( L"Эх, чужак, общий" );

        printf( "POL/1: %" PFSIZET " bytes and %" PFSIZET " chars.\n", len1, byt1 );
        printf( "POL/2: %" PFSIZET " bytes and %" PFSIZET " chars.\n", len2, byt2 );
        printf( "RUS/3: %" PFSIZET " bytes and %" PFSIZET " chars.\n", len3, byt3 );
        printf( "RUS/4: %" PFSIZET " bytes and %" PFSIZET " chars.\n", len4, byt4 );

        ms_string_free( &str );

    .. rst-class:: output
    .. sourcecode:: none

        POL/1: 26 bytes and 17 chars.
        POL/2: 47 bytes and 41 chars.
        RUS/3: 54 bytes and 31 chars.
        RUS/4: 28 bytes and 16 chars.

    .. rst-class:: parameters

    :param wstr: Ciąg znaków unicode.
    :param count: Ilość znaków do sprawdzenia.
    :return: Ilość bajtów zajmowanych przez ciąg po konwersji.

.. c:function:: int ms_string_info_mbs( const char* mbs, size_t bytes, MS_ARRAY* info )

    Pobiera szczegóły dotyczące znaków w ciągu.

    Funkcja sprawdza każdy znak w ciągu i zapisuje do tablicy informacje o jego położeniu i rozmiarze.
    Informacje te tworzone są automatycznie podczas tworzenia ciągu wielobajtowych znaków
    w polu o nazwie :c:member:`MS_STRING.MBInfo`.
    Tablica przekazywana do funkcji musi być wcześniej utworzona jednym z konstruktorów.
    Rozmiar tablicy jest jednocześnie ilością znaków w ciągu.
    Wartość ta jest więc równoznaczna z wartością zwracaną przez funkcję :c:func:`ms_string_length_mbs`.

    Istnieje możliwość ograniczenia ilości sprawdzanych znaków, poprzez podanie odpowiedniego rozmiaru
    przetwarzanego ciągu w polu ``bytes``.
    W przypadku podania wartości dla której ciąg jest błędny, tablica będzie uzupełniona do ostatniego
    poprawnego znaku występującego w ciągu ograniczonego podaną wartością.
    Gdy ciąg znaków zakończony jest znakiem ``\0`` oraz w parametrze ``bytes`` podana będzie wartość większa
    niż ilość bajtów ciągu, obliczenia zostaną przerwane zaraz po napotkaniu znaku ``\0``.

    Przykład użycia funkcji::

        int  retval;
        char chr[MB_LEN_MAX + 1];
        char str[] = "Эх, чужак.";

        MS_ARRAY  array;

        setlocale( LC_ALL, "pl_PL.utf8" );

        /* initialize array */
        array = ms_array_return_local( sizeof(MS_MBINFO), MSD_ARRAY_DEFAULT_SIZE );

        /* get info about string */
        if( (retval = ms_string_info_mbs(str, 0, &array)) )
            return retval;

        /* display info */
        for( retval = 0; retval < array.Length; ++retval )
        {
            MS_MBINFO info = ms_array_getl( array, MS_MBINFO, retval );

            /* prepare data to display */
            memcpy( chr, &str[info.Offset], info.Bytes );
            chr[info.Bytes] = '\0';

            printf( "Size: %d > Char: %s > Shift: 0x%02" PFSIZETUH "\n",
                info.Bytes, chr, info.Offset );
        }

        ms_array_free( &array );

    .. rst-class:: output
    .. sourcecode:: none

        Size: 2 > Char: Э > Shift: 0x00
        Size: 2 > Char: х > Shift: 0x02
        Size: 1 > Char: , > Shift: 0x04
        Size: 1 > Char:   > Shift: 0x05
        Size: 2 > Char: ч > Shift: 0x06
        Size: 2 > Char: у > Shift: 0x08
        Size: 2 > Char: ж > Shift: 0x0A
        Size: 2 > Char: а > Shift: 0x0C
        Size: 2 > Char: к > Shift: 0x0E
        Size: 1 > Char: . > Shift: 0x10

    .. rst-class:: parameters

    :param mbs: Ciąg znaków wielobajtowych.
    :param bytes: Ilość bajtów przetwarzanych w ciągu.
    :param info: Tablica wyjściowa, do której zapisywane będą informacje.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_VALUE`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: int ms_string_info_wcstombs( const wchar_t* wstr, size_t count, MS_ARRAY* info )

    Pobiera szczegóły dotyczące znaków w ciągu po konwersji na *mbs*.

    Funkcja kowertuje każdy znak i zapisuje jego położenie i rozmiar po konwersji bezpośrednio do tablicy.
    Tablica przekazywana do funkcji musi być wcześniej utworzona jednym z konstruktorów.
    Aby uzyskać ilość bajtów dla całego ciągu znaków należy do pozycji ostatniego elementu dodać jego rozmiar.
    Suma ta jest równoznaczna z wartością zwracaną przez funkcję :c:func:`ms_string_length_wcstombs`.

    Można ograniczyć ciąg znaków, podając ich ilość w parametrze ``count``.
    Wartość 0 traktowana jest jako ilość znaków w ciągu.
    Gdy ciąg znaków zakończony jest znakiem ``\0`` oraz w parametrze ``count`` podana będzie wartość większa
    niż ilość znaków w ciągu, obliczenia zostaną przerwane zaraz po napotkaniu znaku ``\0``.

    Przykład użycia funkcji::

        int     retval;
        char    chr[MB_LEN_MAX + 1];
        wchar_t str[] = L"Jámbor célú.";

        MS_ARRAY  array;

        setlocale( LC_ALL, "pl_PL.utf8" );

        /* initialize array */
        array = ms_array_return_local( sizeof(MS_MBINFO), MSD_ARRAY_DEFAULT_SIZE );

        /* get info about string */
        if( (retval = ms_string_info_wcstombs(str, 0, &array)) )
            return retval;

        /* display info */
        for( retval = 0; retval < array.Length; ++retval )
        {
            MS_MBINFO info = ms_array_getl( array, MS_MBINFO, retval );

            printf( "Size: %d > Char: %lc > Shift: 0x%02" PFSIZETUH "\n",
                info.Bytes, str[retval], info.Offset );
        }

        ms_array_free( &array );

    .. rst-class:: output
    .. sourcecode:: none

        Size: 1 > Char: J > Shift: 0x00
        Size: 2 > Char: á > Shift: 0x01
        Size: 1 > Char: m > Shift: 0x03
        Size: 1 > Char: b > Shift: 0x04
        Size: 1 > Char: o > Shift: 0x05
        Size: 1 > Char: r > Shift: 0x06
        Size: 1 > Char:   > Shift: 0x07
        Size: 1 > Char: c > Shift: 0x08
        Size: 2 > Char: é > Shift: 0x09
        Size: 1 > Char: l > Shift: 0x0B
        Size: 2 > Char: ú > Shift: 0x0C
        Size: 1 > Char: . > Shift: 0x0E

    .. rst-class:: parameters

    :param wstr: Ciąg znaków unicode.
    :param count: Ilość znaków przetwarzanych w ciągu.
    :param info: Tablica wyjściowa, do której zapisywane będą informacje.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_VALUE`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     MAKRA
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Makra
---------------------------------------------------------

.. c:macro:: int ms_string_push_cs( MS_STRING* str, const char *cstr, size_t count )

    Wstawia tekst o jednobajtowych znakach na sam koniec podanego ciągu.

    Makro to jest skrótem do funkcji :c:func:`ms_string_insert_cs`.
    Różnica w wywołaniu sprowadza się do braku parametru powiązanego z pozycją wstawianego tekstu.
    Parametr ten uzupełniany jest wartością pobieraną z pola :c:member:`MS_STRING.Length`.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param cstr: Standardowy teskt o jednobajtowych znakach.
    :param count: Ilość znaków do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:macro:: int ms_string_push_mbs( MS_STRING* str, const char *mbstr, size_t bytes )

    Wstawia tekst o wielobajtowych znakach na sam koniec podanego ciągu.

    Makro to jest skrótem do funkcji :c:func:`ms_string_insert_mbs`.
    Różnica w wywołaniu sprowadza się do braku parametru powiązanego z pozycją wstawianego tekstu.
    Parametr ten uzupełniany jest wartością pobieraną z pola :c:member:`MS_STRING.Length`.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param mbstr: Standardowy teskt o wielobajtowych znakach.
    :param bytes: Ilość bajtów do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:macro:: int ms_string_push_wcs( MS_STRING* str, const wchar_t *wcstr, size_t count )

    Wstawia tekst o znakach unikodu na sam koniec podanego ciągu.

    Makro to jest skrótem do funkcji :c:func:`ms_string_insert_wcs`.
    Różnica w wywołaniu sprowadza się do braku parametru powiązanego z pozycją wstawianego tekstu.
    Parametr ten uzupełniany jest wartością pobieraną z pola :c:member:`MS_STRING.Length`.

    .. rst-class:: parameters

    :param str: Wskaźnik na uniwersalną strukturę tekstu.
    :param wcstr: Standardowy teskt o znakach unicode.
    :param count: Ilość znaków do wstawienia z podanego tekstu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
            * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. container:: gitlinks

    * `Pokaż nagłówek modułu <https://raw.githubusercontent.com/sobiemir/moss/master/moss/inc/string.h>`_
    * `Pokaż źródło modułu <https://raw.githubusercontent.com/sobiemir/moss/master/moss/src/string.c>`_
