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
 . Documentation file for "Dynamic Array" module...
 ..

.. highlight:: c

Tablica dynamiczna elementów tego samego rozmiaru
=========================================================

Tablica dynamiczna składająca się z elementów o tym samym typie.
W odróżnieniu od standardowej tablicy języka *C*, ta potrafi automatycznie zwiększać swój rozmiar po przekroczeniu
ilości miejsca, które zostało dla niej przydzielone.
Przydzielanie większej ilości miejsca może odbywać się zarówno ręcznie jak i automatycznie, zaś mniejszej tylko ręcznie.
Automatyczne przydzielanie wywoływane jest przy dodawaniu elementów do tablicy, gdzie sprawdzany jest warunek,
czy tablica będzie w stanie przechować podane elementy lub element.

Teoretycznie tablica powinna składać się tylko z elementów tego samego typu, jednak podstawowa konstrukcja nie
przeszkadza przechowywać innych typów niż zadeklarowany.
Jedynym ograniczeniem jest rozmiar pojedynczego elementu, który zawsze musi być taki sam.
W niektórych przypadkach funkcje po wystąpieniu błędu oprócz zwracania kodu błędu, zapisują go do zmiennej ``errno``.
Zmienna ta nie jest modyfikowana, gdy błąd pochodzi ze standardowej biblioteki języka *C*.

Przykład kodu zawierającego operacje na tablicy dynamicznej::

    #define DESTRUCTARRAY(a,b,c,ercode,txt) \
        return \
            printf( txt ", code %d.\n", ercode ), \
            ms_array_free( a ), ms_array_free( b ), ms_array_free( c ), \
            ercode

    float points[] = {
        0.5f,   0.8f,   0.9f, 0.11f,  0.56f,
        0.178f, 0.245f, 0.1f, 0.875f, 0.404f
    };
    int    ercode;
    size_t iter;

    MS_ARRAY *array = ms_array_alloc( sizeof(float), 10 ),
              copy1,
              copy2 = ms_array_return_local( sizeof(float), 10 );

    if( !array )
        DESTRUCTARRAY( array, NULL, &copy2, MSEC_MEMORY_ALLOCATION, "Array create failed" );

    /* dodaj wszystkie wartości z tablicy */
    if( (ercode = ms_array_push_values(array, points, 10)) )
        DESTRUCTARRAY( array, &copy1, &copy2, ercode, "Push operation failed" );

    /* kopiuj całą tablicę do nie inicjalizowanej jeszcze zmiennej */
    if( (ercode = ms_array_copy(&copy1, array)) )
        DESTRUCTARRAY( array, &copy1, &copy2, ercode, "Array copy failed" );

    /* dodaj do tablicy 5 elementów na sam koniec licząc od indeksu 3 */
    if( (ercode = ms_array_join_slice(&copy1, array, 3, 5)) )
        DESTRUCTARRAY( array, &copy1, &copy2, ercode, "Array join failed" );
    printf( "Items in copy1: %zu\n", copy1.Length );

    /* usuń elementy na ineksach od 3 do 12 */
    ms_array_remove_range( &copy1, 3, 9 );
    printf( "Items in copy1: %zu\n", copy1.Length );

    /* pozostaw w tablicy elementy na indeksach od 2 do 6 */
    ms_array_slice( array, 2, 4 );
    printf( "Items in array: %zu\n", array->Length );

    /* zlep 3 z dwóch pozostałych */
    if( (ercode = ms_array_join(&copy2, array)) )
        DESTRUCTARRAY( array, &copy1, &copy2, ercode, "Array join failed" );
    if( (ercode = ms_array_join(&copy2, &copy1)) )
        DESTRUCTARRAY( array, &copy1, &copy2, ercode, "Array join failed" );
    printf( "Items in copy2: %zu\n", copy2.Length );

    /* wypisz elementy */
    for( iter = 0; iter < copy2.Length; ++iter )
        printf( "Index: %zu has Value: %f\n", iter, ms_array_getl(copy2, float, iter) );

    ms_array_free( array );
    ms_array_free( &copy1 );
    ms_array_free( &copy2 );

Wyjście:

.. sourcecode:: none

    Items in copy1: 15
    Items in copy1: 6
    Items in array: 4
    Items in copy2: 10
    Index: 0 has Value: 0.900000
    Index: 1 has Value: 0.110000
    Index: 2 has Value: 0.560000
    Index: 3 has Value: 0.178000
    Index: 4 has Value: 0.500000
    Index: 5 has Value: 0.800000
    Index: 6 has Value: 0.900000
    Index: 7 has Value: 0.178000
    Index: 8 has Value: 0.245000
    Index: 9 has Value: 0.100000

Moduł umożliwia tworzenie dynamicznych tablic dedykowanych poprzez zdefiniowane makra, które obejmują tylko określony typ.
Makra tworzą kopię struktury wraz z funkcjami, opartymi o funkcje bazowe.
Pozostałe funkcje nie są powielane, gdyż ich zastosowanie jest uniwersalne.
Dzięki temu tworzenie takich tablic jest o wiele prostsze niż tworzenie je własnoręcznie.
Wywołanie makra wymaga podania przyrostków dla struktury i funkcji, dzięki czemu możliwe jest utworzenie unikalnych
funkcji rozpoznawalnych przez język C.

Przykład użycia makra::

    MST_ARRAY_HEADER( float, FLOAT, _float );
    MST_ARRAY_BODY( float, FLOAT, _float );
    ...
    #define DESTRUCTARRAY(a,ercode,txt) \
        return \
            printf( txt ", code %d.\n", ercode ), \
            ms_array_free( &a ), \
            ercode

    MS_ARRAYFLOAT afloat = ms_array_return_float( 3 );
    int           ercode;
    size_t        iter;

    /* dodaj elementy do tablicy */
    if( (ercode = ms_array_push_float(&afloat, 3.1f)) )
        DESTRUCTARRAY( afloat, ercode, "Array push failed" );
    if( (ms_array_insert_float(&afloat, 0, 1.65f)) )
        DESTRUCTARRAY( afloat, ercode, "Array insert failed" );
    if( (ms_array_insert_float( &afloat, 1, 2.11f )) )
        DESTRUCTARRAY( afloat, ercode, "Array insert failed" );

    /* wyświetl elementy */
    for( iter = 0; iter < afloat.Length; ++iter )
        printf( "Array Float => Value: %f\n", afloat.Items[iter] );

    ms_array_free( &afloat );

Wyjście:

.. sourcecode:: none

    Array Float => Value: 1.650000
    Array Float => Value: 2.110000
    Array Float => Value: 3.100000

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     STRUKTURY I STAŁE
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Struktury, stałe i szablony
---------------------------------------------------------

.. c:type:: MS_ARRAY

    Struktura tablicy dynamicznej do której odwołują się wszystkie funkcje w module.
    Każde wywołanie funkcji tworzącej tablicę musi być zakończone wywołaniem funkcji, zwalniającej
    zasoby przydzielone dla obiektu, aby zapobiec wyciekom pamięci.
    Dzieje się tak głównie dlatego, że dla pola :c:member:`MS_ARRAY.Items`, które w przypadku błędu
    powinno mieć zawsze wartość *NULL*, przydzielana jest dynamicznie pamięć podczas tworzenia tablicy,
    lub zmiany jej pojemności w przypadku przekroczenia aktualnej.
    Szczegóły na temat struktury znajdują się w opisach poszczególnych pól.

    Wartości domyślne pól struktury ustawiane podczas tworzenia tablicy:

    ===================== ==========================================
    Pole                  Wartość domyślna
    ===================== ==========================================
    Capacity              parametr
    ItemSize              parametr
    Length                0
    Modifier              2.f
    Destroy               automatycznie
    Items                 wskaźnik
    FuncIncrease          :c:member:`MSS_ARRAYFUNCTIONS.IncMultiply`
    ===================== ==========================================

    .. c:member:: size_t Capacity

        Pojemność tablicy. Po przekroczeniu lub wyrównaniu ilości elementów z tą wartością, podczas dodawania elementu,
        następuje przydzielenie większej ilości miejsca w tablicy dla nowych elementów o wartości obliczanej
        zgodnie z ustawioną funkcją zwiększającą pojemność, nazywaną dalej *inkreatorem*.
        Wartość ta nie powinna być zmieniana samodzielnie, gdyż może to prowadzić do błędów naruszenia ochrony pamięci
        podczas dodawania elementów oraz w czasie kopiowania tablicy.

        Przykład automatycznej zmiany pojemności::

            int list[] = { 0, 1, 2 };
            MS_ARRAY array = ms_array_return_local( sizeof(int), 2 );

            array.Modifier     = 4.f;
            array.FuncIncrease = MSC_ArrayFunctions.IncAdd;

            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );
            ms_array_push_value( &array, &list[0] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );
            ms_array_push_value( &array, &list[1] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );

            /* w tym momencie zadziała inkreator */
            ms_array_push_value( &array, &list[2] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );

            ms_array_free( &array );

        Wyjście:

        .. sourcecode:: none

            Array => Length: 0 with Capacity: 2
            Array => Length: 1 with Capacity: 2
            Array => Length: 2 with Capacity: 2
            Array => Length: 3 with Capacity: 6

    .. c:member:: size_t ItemSize

        Rozmiar pojedynczego elementu.
        Dodawanie elementu przez wskaźnik opiera się na założeniu, że każdy element musi mieć taki sam rozmiar.
        W przypadku dynamicznych tablic dedykowanych, ograniczeniem jest ten sam typ.
        Wartość ta nie powinna być zmieniana samodzielnie, gdyż może to prowadzić do błędów związanych z naruszeniem ochrony pamięci,
        występujących nie tylko podczas dodawania czy usuwania poszczególnych elementów, ale również podczas kopiowania tablicy.

    .. c:member:: size_t Length

        Ilość elementów w tablicy.
        Wartość ta reprezentuje aktualną długość tablicy i jest porównywana z pojemnością podczas dodawania elementów.
        Jej zmniejszenie spowoduje obcięcie ilości elementów od końca, zwiększenie zaś zebranie śmieci.
        Ustawienie długości tablicy poza pojemność może prowadzić do błędów naruszenia ochrony pamięci.
        Technika samodzielnej zmiany długości może być przydatna podczas przydzielenia pamięci na elementy, przechowywane
        bezpośrednio w tablicy.

        Przykład samodzielnej zmiany długości::

            struct S_SAMPLE {
                int x, y, z;
            };

            struct S_SAMPLE *ptr;
            size_t iter;

            MS_ARRAY array = ms_array_return_local( sizeof *ptr, 4 );

            array.Length = 4;
            ptr = (struct S_SAMPLE*)array.Items;

            for( iter = 0; iter < array.Length; ++iter )
                ptr[iter].x = iter,
                ptr[iter].y = iter + 1,
                ptr[iter].z = iter + 2;

            for( iter = 0; iter < array.Length; ++iter )
                printf( "Index: %zu with Value: [x:%d, y:%d, z:%d]\n",
                    iter, ptr[iter].x, ptr[iter].y, ptr[iter].z );

            ms_array_free( &array );

        Wyjście:

        .. sourcecode:: none

            Index: 0 with Value: [x:0, y:1, z:2]
            Index: 1 with Value: [x:1, y:2, z:3]
            Index: 2 with Value: [x:2, y:3, z:4]
            Index: 3 with Value: [x:3, y:4, z:5]

    .. c:member:: float Modifier

        Modyfikator kontrolujący powiększenie pojemności tablicy.
        Zastosowanie modyfikatora, jak i również przyjmowane przez niego wartości, są uzależnione od aktualnie
        ustawionej funkcji zwiększającej pojemność tablicy, do której zmienna jest zawsze przekazywana.
        Ustawienie złej wartości może spowodować błędne obliczenia nowej pojemności w *inkreatorze*.
        Zastosowania mnożnika można znaleźć w opisach pól struktury :c:type:`MSS_ARRAYFUNCTIONS`.

        Przykład używania mnożnika::

            int list[] = { 0, 1, 2, 4 };
            MS_ARRAY array = ms_array_return_local( sizeof(int), 1 );

            /* funkcja potęgowa */
            array.Modifier     = 1.1f;
            array.FuncIncrease = MSC_ArrayFunctions.IncPower;

            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );
            ms_array_push_value( &array, &list[0] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );

            /* tutaj zadziała inkreator, 1^1.1 to dalej 1
               L == P, więc do pojemności dodawana jest wartość 1 */
            ms_array_push_value( &array, &list[1] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );

            array.Modifier = 5.8f;
            /* tutaj zadziała, 2^5.8 to ~55.72, zaokrąglanie w dół, 55 */
            ms_array_push_value( &array, &list[2] );
            printf( "Array => Length: %zu with Capacity: %zu\n", array.Length, array.Capacity );

            ms_array_free( &array );

        Wyjście:

        .. sourcecode:: none
            
            Array => Length: 0 with Capacity: 1
            Array => Length: 1 with Capacity: 1
            Array => Length: 2 with Capacity: 2
            Array => Length: 3 with Capacity: 55

    .. c:member:: bool Destroy

        Informacja o tym, czy zmienna przechowująca strukturę tablicy ma zostać zniszczona.
        Wykorzystywana **tylko i wyłącznie** w funkcji :c:func:`ms_array_free`, odpowiedzialnej za zwalnianie
        przydzielonych zasobów przeznaczonych na tablicę.
        Wartość ustawiana jest na ``TRUE`` tylko w funkcjach, zwracających wskaźnik do nowej tablicy.
        Samodzielna zmiana tej wartości w zależności od typu tablicy może prowadzić do wycieków lub
        naruszenia ochrony pamięci.

    .. c:member:: void** Items

        Elementy zapisane do tablicy.
        W zależności od implementacji struktury tablicy i przechowywanych w niej wartości, przed
        pobraniem elementu należy rzutować go do odpowiedniego typu.
        Pojedynczy element można szybko pobrać za pomocą jednego z wbudowanych makr, :c:macro:`ms_array_get`
        lub :c:macro:`ms_array_getl`.
        Pole to w implementacji standardowej jest typu ``void**``, jednak tablica dedykowana zmienia go w zależności
        od deklarowanego typu przechowywanego w tablicy.
        W przypadku pobierania większej liczby elementów lub nawet całego zbioru, warto przypisać zmienną do innej zmiennej
        uprzednio rzutując ją na inny typ.

        Dwa sposoby pobierania zmiennych z tablicy::

            int    list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            int   *elem;
            size_t iter;
    
            MS_ARRAY array = ms_array_return_local( sizeof(int), 10 );

            /* dodaj wartości */
            ms_array_push_values( &array, list, 10 );

            /* pierwszy sposób */
            elem = (int*)array.Items;
            fputs( "Cast style:\n\t", stdout );
            for( iter = 0; iter < array.Length; ++iter )
                printf( "%d ", elem[iter] );

            fputs( "\n", stdout );
            fputs( "Macro style:\n\t", stdout );

            /* drugi sposób */
            for( iter = 0; iter < array.Length; ++iter )
                printf( "%d ", ms_array_getl(array, int, iter) );

            fputs( "\n", stdout );
            ms_array_free( &array );

        Wyjście:

        .. sourcecode:: none

            Cast style:
                0 1 2 3 4 5 6 7 8 9 
            Macro style:
                0 1 2 3 4 5 6 7 8 9

    .. c:member:: size_t FuncIncrease(size_t capacity, float modifier)

        Funkcja zwana inaczej *inkreatorem*, zwiększająca pojemność tablicy, używana podczas przydzielania pamięci
        dla jej nowych elementów.
        Dzięki temu polu można przypisać własną funkcję obliczającą nową ilość pamięci.
        Wszystkie wskaźniki wbudowanych *inkreatorów* znajdują się w stałej :c:data:`MSC_ArrayFunctions`
        mając w nazwie przedrostek *Inc*. Funkcje te zawsze po obliczeniach zaokrąglają wynik w dół.
        Do wnętrza funkcji pod parametrem ``modifier`` przekazywana jest wartość pola :c:member:`MS_ARRAY.Modifier`.
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

        :param capacity: Aktualna pojemność tablicy.
        :param modifier: Modyfikator pojemności.
        :return: Nową pojemność tablicy, przekazywaną do przydzielenia pamięci.

.. c:type:: MSS_ARRAYFUNCTIONS

    Struktura zawiera pola, posiadające wskaźniki do wbudowanych funkcji modułu.
    Z tej struktury korzysta zmienna :c:data:`MSC_ArrayFunctions`, dzięki której możliwy jest dostęp do funkcji
    wbudowanych z zewnątrz.
    Poniższe pola opisane są skrótowo z racji tego, iż schemat działania został opisany dokładnie w polu,
    do którego dana funkcja przynależy. Przykładem tego jest pole :c:member:`MS_ARRAY.FuncIncrease`.
    Aktualnie struktura przechiwuje tylko funkcji zwiększających pojemność tablicy.

    .. c:member:: size_t IncMultiply(size_t capacity, float modifier)

        Modyfikuje wartość zmiennej ``capacity``, mnożąc ją ze zmienną ``modifier``.

        .. rst-class:: parameters

        :param capacity: Aktualna pojemność tablicy.
        :param modifier: Mnożnik pojemności.
        :return: Nową pojemność tablicy, przekazywaną do przydzielenia pamięci.

    .. c:member:: size_t IncAdd(size_t capacity, float modifier)

        Modyfikuje wartość zmiennej ``capacity``, dodając do niej wartość zmiennej ``modifier``.

        .. rst-class:: parameters

        :param capacity: Aktualna pojemność tablicy.
        :param modifier: Składnik dodawania.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia pamięci.

    .. c:member:: size_t IncPower(size_t capacity, float modifier)

        Modyfikuje wartość zmiennej ``capacity``, podnosząc ją do potęgi o wartości ze zmiennej ``modifier``.

        .. rst-class:: parameters

        :param capacity: Aktualna pojemność tablicy, traktowana jako podstawa potęgi.
        :param modifier: Wykładnik potęgi.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia pamięci.

.. c:var:: MSS_ARRAYFUNCTIONS MSC_ArrayFunctions

    Stała przechowująca wskaźniki do wbudowanych funkcji modułu.
    Aktualnie przechowuje tylko funkcje pozwalające na zwiększanie elementów w tablicy.
    Funkcję obliczającą nową pojemność tablicy można zmienić, przypisując do pola :c:member:`MS_ARRAY.FuncIncrease`
    jedną z funkcji podanych w strukturze, rozpoczynających się od przedrostka *Inc*.
    Wszystkie funkcje których wskaźniki zawiera stała, opisane są w polach struktury :c:type:`MSS_ARRAYFUNCTIONS`.

    Przykład użycia stałej::

        int number = 6;
        MS_ARRAY array = ms_array_return_local( sizeof(int), 1 );

        ms_array_push_value( &array, &number );

        /* tutaj zwiększy wartość, 1 * 2 = 2 */
        array.FuncIncrease = MSC_ArrayFunctions.IncMultiply;
        ms_array_push_value( &array, &number );

        /* tutaj zwiększy wartość, 2 + 2 = 4 */
        array.FuncIncrease = MSC_ArrayFunctions.IncAdd;
        ms_array_push_value( &array, &number );

        /* tutaj zwiększy wartość, 4 ^ 2 = 16 */
        array.FuncIncrease = MSC_ArrayFunctions.IncPower;
        ms_array_push_value( &array, &number );
        ms_array_push_value( &array, &number );

        ms_array_free( &array );

.. c:macro:: void MST_ARRAY_HEADER( type type, literal spfix, literal fpfix )

    Makro tworzące nagłówki funkcji pochodnych opartych o funkcje bazowe.
    Dodatkowo tworzy nową strukturę dla tablicy dedykowanej dla konkretnego typu podanego w parametrze.
    Wszystkie funkcje operują na tworzonej strukturze i podanym typie, dzięki czemu elementy mogą być prosto wstawiane do tablicy.
    Jako że makro tworzy tylko nagłówki, aby korzystać z funkcji, których sygnatury zostały utworzone, należy wywołać dodatkowo
    makro :c:macro:`MST_ARRAY_BODY`, tworzące ciała funkcji.
    Generalnie makro to powinno się wstawiać w jednym z plików nagłówkowych projektu.

    .. rst-class:: parameters

    :param type: Typ w którym przechowywane będą elementy w tablicy.
    :param spfix: Przyrostek, który będzie zawarty w nazwie tablicy.
    :param fpfix: Przyrostek, który będzie zawarty w nazwie funkcji.

.. c:macro:: void MST_ARRAY_BODY( type type, literal spfix, literal fpfix )

    Makro tworzące ciała funkcji pochodnych dedykowanych dla konkretnego typu, opartych o funkcje bazowe.
    Wszystkie funkcje operują na strukturze tworzonej w makrze :c:macro:`MST_ARRAY_HEADER`, dlatego makro to powinno być
    wywołane wcześniej.
    Funkcje umożliwiają szybsze i bezpośrednie techniki operowania na konkretnych danych w tablicach.
    Makro to powinno być wywoływane w plikach źródłowych, jednak nic nie przeszkadza umieścić go w plikach nagłówkowych.

    .. rst-class:: parameters

    :param type: Typ w którym przechowywane będą elementy w tablicy.
    :param spfix: Przyrostek, który będzie zawarty w nazwie tablicy.
    :param fpfix: Przyrostek, który będzie zawarty w nazwie funkcji.

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     TWORZENIE I INICJALIZACJA
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Tworzenie i inicjalizacja
---------------------------------------------------------

.. c:function:: void* ms_array_alloc( size_t size, size_t capacity )

    Tworzy nową tablicę oraz rezerwuje początkowe miejsce na dane.
    W odróżnieniu od inicjalizacji, funkcja zwraca wskaźnik do utworzonej tablicy.
    W przypadku błędu podczas tworzenia, zwrócona zostaje wartość *NULL*.
    Funkcja jako jedna z nielicznych ustawia pole :c:member:`MS_ARRAY.Destroy` na wartość *TRUE*.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY *array = ms_array_alloc( sizeof(int), 100 );

        if( !array )
            printf( "Error! Memory allocation failed! Code: %d.\n", errno );

        ms_array_free( array );

    .. rst-class:: parameters

    :param size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param capacity: Początkowa ilość rezerwowanego miejsca na elementy tablicy.
    :return: Wskaźnik na utworzoną tablicę lub wartość *NULL* w przypadku błędu.

.. c:function:: int ms_array_init( void* aptr, size_t size, size_t capacity )

    Inicjalizuje istniejącą tablicę i rezerwuje początkowe miejsce na dane.
    Wszystkie funkcje tworzące tablice odwołują się bezpośrednio do tej funkcji.
    W przypadku błędu zwracany jest jego kod, w przeciwnym razie wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY array;
        int ercode;

        if( (ercode = ms_array_init(&array, sizeof(int), 100)) )
            printf( "Error! Array creation failed! Code: %d.\n", ercode );

        ms_array_free( &array );

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param capacity: Początkowa ilość rezerwowanego miejsca na elementy tablicy.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: MS_ARRAY ms_array_return_local( size_t size, size_t capacity )

    Tworzy tablicę lokalną oraz rezerwuje początkowe miejsce na dane.
    Odmiana tej funkcji pozwala na ustawienie rozmiaru pojedynczego elementu.
    W przypadku błędu podczas tworzenia tablicy, pole :c:member:`MS_ARRAY.Items` jest równe *NULL*.
    Utworzona w ten sposób tablica nadal wymaga zwolnienia przydzielonych zasobów poprzez wywołanie funkcji
    :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_return_local( sizeof(int), 100 );
        
        if( array.Items == NULL )
            printf( "Error! Memory allocation failed! Code: %d.\n", errno );

        ms_array_free( &array );

    .. rst-class:: parameters

    :param size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param capacity: Początkowa ilość rezerwowanego miejsca na elementy tablicy.
    :return: Utworzoną tablicę lokalną.


.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     ZMIANA POJEMNOŚCI
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Zmiana pojemności
---------------------------------------------------------

.. c:function:: int ms_array_realloc( void* aptr, size_t capacity )

    Zmniejsza lub zwiększa pojemność tablicy.
    Funkcja posiada dwa wbudowane tryby zwiększania pojemności - automatyczny i ręczny.
    Tryb automatyczny można uruchomić, podając pod zmienną ``capacity`` wartość 0.
    Obliczaniem pojemności w takim wypadku zajmuje się funkcja *inkreatora* podpięta pod przekazaną tablicę do pola
    :c:member:`MS_ARRAY.FuncIncrease`.
    Gdy *inkreator* nie jest podpięty, zwracany jest błąd, gdyż funkcja nie wie ile ma przydzielić pamięci.
    W przypadku zwrócenia przez *inkreator* pojemności mniejszej niż aktualna, wartość ta jest odrzucana i jako
    nowa przyjmowana jest suma ``capacity + 1``.
    Tryb ręczny uruchamiany jest w przypadku wpisania wartości innej niż 0 w parametrze ``capacity``.
    W tym trybie pojemność może być zarówno zwiększana jak i zmniejszana.
    Próba przydziału pojemności mniejszej niż ilość elementów zapisanych w tablicy skutkuje błędem i natychmiastowym
    zakończeniem działania funkcji.

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_return_local( sizeof(int), 3 );
        int      ercode;
        
        array.FuncIncrease = MSC_ArrayFunctions.IncPower;
        array.Modifier     = 2.57f;

        printf( "Capacity: %zu\n", array.Capacity );

        /* 3^2.5 = ~16.83 co daje po zaokrągleniu 16 */
        if( (ercode = ms_array_realloc(&array, 0)) )
            printf( "Error in ms_array_realloc, code: %d\n", ercode );
        printf( "Capacity: %zu\n", array.Capacity );

        /* zmiana pojemności tablicy do podanej wartości */
        if( (ercode = ms_array_realloc(&array, 30)) )
            printf( "Error in ms_array_realloc, code: %d\n", ercode );
        printf( "Capacity: %zu <- Exact\n", array.Capacity );

        /* spodziewany błąd, brak inkreatora */
        array.FuncIncrease = NULL;
        if( (ercode = ms_array_realloc(&array, 0)) == MSEC_INVALID_VALUE )
            printf( "Error! MSEC_INVALID_VALUE, FuncIncrease is missing!\n" );
        printf( "Capacity: %zu\n", array.Capacity );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Capacity: 3
        Capacity: 16 <- IncPower
        Capacity: 30 <- Exact
        Error! MSEC_INVALID_VALUE, FuncIncrease is missing!
        Capacity: 30

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param capacity: Nowa pojemność tablicy lub 0 w przypadku automatu.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
            * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_VALUE`
            * :c:member:`MSE_ERROR_CODES.MSEC_DATA_OVERFLOW`

.. c:function:: int ms_array_realloc_min( void* aptr, size_t min )

    Zwiększa pojemność tablicy do najbliższej wartości następującej po wartości podanej w parametrze ``min``.
    Podanie wartości mniejszej niż aktualna pojemność nie kończy się błędem, ale również nie zmienia
    pojemności całej tablicy, ponieważ minimum zostało już osiągnięte.
    Funkcja wywołuje *inkreatora* dopóty, dopóki wartość przez niego zwracana nie będzie większa lub
    równa wartości zmiennej ``min``, przekazanej w parametrze.
    W przypadku gdy pole :c:member:`MS_ARRAY.FuncIncrease` będzie równe wartości *NULL*, jako nowa pojemność
    przyjęta zostanie wartość minimalna.
    Funkcja przydaje się szczególnie w trakcie wstawiania tablic, gdzie za jednym razem przydzielana jest
    odpowiednia ilość pamięci, która pozwoli zmieścić wszystkie elementy w tablicy dynamicznej i ewentualnie
    pozostawić miejsce na nowe.

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_return_local( sizeof(int), 2 );
        int      ercode;

        array.FuncIncrease = MSC_ArrayFunctions.IncPower;
        array.Modifier     = 1.5f;

        printf( "Capacity: %zu\n", array.Capacity );

        /* 3^1.5 ~= 5, 5^1.5 ~= 11, 11^1.5 ~= 36 -> STOP
           wartość minimalna (20) została osiągnięta */
        if( (ercode = ms_array_realloc_min(&array, 20)) )
            printf( "Error! Failed to allocate new memory! Code: %d\n", ercode );
        printf( "Capacity: %zu <- IncPower\n", array.Capacity );

        // dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna
        array.FuncIncrease = NULL;
        if( (ercode = ms_array_realloc_min(&array, 256)) )
            printf( "Error! Failed to allocate new memory! Code: %d\n", ercode );
        printf( "Capacity: %zu <- Exact\n", array.Capacity );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Capacity: 2
        Capacity: 36 <- IncPower
        Capacity: 256 <- Exact

    .. rst-class:: parameters

    :param aptr: Wskaźnik tablicy przeznaczonej do zmiany pojemności.
    :param min: Minimalna wartość do której zwiększona zostanie pojemność tablicy.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     KOPIOWANIE
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================


Kopiowanie
---------------------------------------------------------

.. c:function:: int ms_array_copy( void* adst, const void* asrc )

    Kopiuje tablicę z parametru ``asrc`` do parametru ``adst``.
    Tablica do której dane będą kopiowane musi istnieć, ale nie może być wcześniej zainicjalizowana.
    W przeciwnym przypadku wszystkie dane zostaną nadpisane, co może skończyć się wyciekiem pamięci.
    Utworzoną wcześniej tablicę można wyczyścić, wykorzystując funkcję :c:func:`ms_array_clean`.
    Czyszczenie nie jest uruchamiane automatycznie, gdyż tablica niezainicjalizowana posiada w strukturze
    śmieci, co może prowadzić do błędu podczas zwalniania zasobów, a mianowicie do naruszenia ochrony pamięci.
    Zerowanie danych również nie pomoże w każdym przypadku, gdyż wartość *NULL* nie zawsze składa się ze
    wszystkich bitów równych 0.
    Kopiowane są tylko zapisane dane, tak więc w przypadku wskaźników, kopiowane są tylko wskaźniki.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_return_local( sizeof(int), 20 ),
                 array2 = ms_array_return_local( sizeof(int), 10 ),
                 array3;
        int      ercode;

        if( (ercode = ms_array_copy(&array3, &array2)) )
            printf( "Error! Array copy failed! Code: %d.\n", ercode );

        /* wyczyść tablicę i kopiuj do niej inne dane */
        ms_array_clean( &array2 );
        if( (ercode = ms_array_copy(&array2, &array1)) )
            printf( "Error! Array copy failed! Code: %d.\n", ercode );

        ms_array_free( &array1 );
        ms_array_free( &array2 );
        ms_array_free( &array3 );

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę, do której dane będą kopiowane.
    :param asrc: Wskaźnik na kopiowaną tablicę.
    :return: Kod błedu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:function:: void* ms_array_copy_alloc( const void* aptr )

    Tworzy tablicę i kopiuje do niej dane z tablicy podanej w parametrze.
    Dane kopiowane są w takim formacie w jakim zostały wstawione, tak więc gdy tablica posiada
    wskaźniki do danych, kopiowane są tylko wskaźniki, co prowadzi do tego, że dwie tablice będą miały dostęp
    do tych samych danych.
    Funkcja jako jedna z nielicznych ustawia wartość pola :c:member:`MS_ARRAY.Destroy` na wartość ``TRUE``.
    Przydzielone przez funkcję zasoby zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_return_local( sizeof(int), 20 ),
                *array2;

        if( !(array2 = ms_array_copy_alloc(&array1)) )
            printf( "Error! Array copy failed! Code: %d.\n", errno );

        ms_array_free( &array1 );
        ms_array_free( array2 );

    .. rst-class:: parameters

    :param aptr: Wskaźnik na kopiowaną tablicę.
    :return: Wskaźnik na utworzoną kopię tablicy lub wartość *NULL*.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     DODAWANIE ELEMENTÓW
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Dodawanie elementów
---------------------------------------------------------

.. c:function:: int ms_array_insert_value( void* aptr, size_t index, const void* item )

    Dodaje do tablicy element we wskazane miejsce, przekazany przez wskaźnik.
    Element kopiowany jest do tablicy z zachowaniem rozmiaru zapisanego w polu :c:member:`MS_ARRAY.ItemSize`.
    Przekazanie elementu o innym typie lub rozmiarze niż zadeklarowany, może prowadzić do naruszenia ochrony pamięci.
    Wstawianie elementu w inne miejsce niż na koniec tablicy powoduje przesunięcie wszystkich wartości
    znajdujących się za wartością ``index`` o jedno miejsce w prawo.
    Funkcja sprawdza czy element przekazany do funkcji, zmieści się w tablicy.
    Gdy warunek ten nie zostanie spełniony, wywoływana jest odpowiednia funkcja, zwiększająca pojemność tablicy.

    Przykład dodawania elementów do tablicy::

        int      list[] = { 820, 140, 566, 120 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 4 );
        int     *elems,
                 ercode = 0;

        ercode |= ms_array_insert_value( &array, 0, &list[0] );
        ercode |= ms_array_insert_value( &array, 0, &list[1] );
        ercode |= ms_array_insert_value( &array, 1, &list[2] );
        ercode |= ms_array_insert_value( &array, 1, &list[3] );

        if( ercode )
            printf( "Error! One of the insert function call failed!\n" );
        
        elems = (int*)array.Items;
        printf( "Array => Index: 0 with Value: %d\n", elems[0] );
        printf( "Array => Index: 1 with Value: %d\n", elems[1] );
        printf( "Array => Index: 2 with Value: %d\n", elems[2] );
        printf( "Array => Index: 3 with Value: %d\n", elems[3] );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none
        
        Array => Index: 0 with Value: 140
        Array => Index: 0 with Value: 120
        Array => Index: 0 with Value: 566
        Array => Index: 0 with Value: 820

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param size_t index: Indeks kopiowania elementu.
    :param item: Wskaźnik na element do wstawienia.
    :return: Kod błędu lub :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.OUT_OF_RANGE`

.. c:function:: int ms_array_insert_values( void *adst, size_t index, const void *tsrc, size_t count )

    Dodaje do tablicy elementy do wybranego miejsca, przekazane w parametrze.
    Kopiowanie elementów działa w taki sam sposób jak w przypadku funkcji :c:func:`ms_array_insert_value`
    z tą różnicą, że od podanego indeksu wstawianych jest kilka elementów a nie jeden.
    Wstawianie elementów w inne miejsce niż na koniec tablicy wiąże się z przeniesieniem wszystkich danych,
    których pozycje przewyższają wartość zmiennej ``index``.
    Kopiowane elementy w tym przypadku muszą być przekazane w postaci standardowej tablicy języka *C*.
    Przekazanie rozmiaru tablicy w wartości ``size`` przekraczającej rozmiar rzeczywisty może spowodować
    naruszenie ochrony pamięci.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 8 );
        int     *elems,
                 ercode;
        size_t   iter;

        /* dodaj najpierw od elementy od 0-3, potem od 4-7 po 2 elemencie. */
        if( (ercode = ms_array_insert_values(&array, 0, list, 4)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );
        if( (ercode = ms_array_insert_values(&array, 2, &list[4], 4)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );
        
        /* wypisz wszystkie wartości */
        elems = (int*)array.Items;
        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 0
        Array => Index: 1 with Value: 1
        Array => Index: 2 with Value: 4
        Array => Index: 3 with Value: 5
        Array => Index: 4 with Value: 6
        Array => Index: 5 with Value: 7
        Array => Index: 6 with Value: 2
        Array => Index: 7 with Value: 3

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę.
    :param index: Indeks od którego elementy mają być kopiowane.
    :param tsrc: Wskaźnik na pierwszy element z tablicy standardowej do wstawienia.
    :param size: Ilość elementów do dodania.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:function:: int ms_array_join_slice( void* adst, const void* asrc, size_t offset, size_t count )

    Dodaje do tablicy elementy z podanego zakresu, kopiując je z innej tablicy.
    Kontrola zakresu uniemożliwia podanie indeksu w zmiennej ``offset`` oraz rozmiaru zakresu przewyższającego
    rzeczywistą ilość zapisanych elementów w tablicy.
    Funkcja działa w podobny sposób jak funkcja :c:func:`ms_array_insert_values` z dwoma różnicami.
    Pierwszą jest to, że elementy kopiowane muszą być umieszczone w tablicy dynamicznej, drugą zaś, że
    nie można podać indeksu od którego wartości będą wstawiane.
    Podczas łączenia tablic elementy wstawiane są zawsze na samym końcu tablicy przekazanej w zmiennej ``adst``.
    W przypadku podania wartości 0 do zmiennej ``count``, ilość kopiowanych elementów jest obliczana
    automatycznie i przyjmuje wartość równą ilości pozostałych elementów do końca tablicy, licząc od
    wartości zmiennej ``offset``.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        MS_ARRAY array1  = ms_array_return_local( sizeof(int), 8 ),
                 array2;
        int     *elems,
                 ercode;
        size_t   iter;

        ms_array_init( &array2, sizeof(int), 8 );

        if( (ercode = ms_array_insert_values(&array1, 0, list, 8)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        if( (ercode = ms_array_join_slice(&array2, &array1, 2, 4)) )
            printf( "Error! Function failed with code: %d!\n", ercode );

        /* dodaj elementy od indeksu 5 do końca */
        if( (ercode = ms_array_join_slice(&array2, &array1, 5, 0)) )
            printf( "Error! Function failed with code: %d!\n", ercode );
        
        /* wypisz wszystkie wartości */
        elems = (int*)array2.Items;
        for( iter = 0; iter < array2.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array1 );
        ms_array_free( &array2 );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 2
        Array => Index: 1 with Value: 3
        Array => Index: 2 with Value: 4
        Array => Index: 3 with Value: 5
        Array => Index: 4 with Value: 5
        Array => Index: 5 with Value: 6
        Array => Index: 6 with Value: 7

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę do której elementy będą wstawiane.
    :param asrc: Wskaźnik na tablicę z której elementy będą kopiowane.
    :param offset: Indeks od którego wartości mają być kopiowane.
    :param count: Ilość elementów w zakresie lub 0.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:function:: int ms_array_join_slice_inverse( void* adst, const void* asrc, size_t offset, size_t count )

    Dodaje do tablicy elementy z innej tablicy, pomijając wartości znajdujące się w podanym zakresie.
    Jak sama nazwa wskazuje, funkcja ta jest inwersją funkcji :c:func:`ms_array_join_slice`, co oznacza,
    że dodaje elementy, które nie obejmuje podany zakres.
    Wszystkie kopiowane elementy umieszczane są na samym końcu tablicy.
    W przypadku podania wartości 0 do zmiennej ``count``, ilość kopiowanych elementów jest obliczana
    automatycznie i przyjmuje wartość równą ilości pozostałych elementów do końca tablicy, licząc od
    wartości zmiennej ``offset``.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        MS_ARRAY array1  = ms_array_return_local( sizeof(int), 8 ),
                 array2;
        int     *elems,
                 ercode;
        size_t   iter;

        ms_array_init( &array2, sizeof(int), 8 );

        if( (ercode = ms_array_insert_values(&array1, 0, list, 8)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        if( (ercode = ms_array_join_slice_inverse(&array2, &array1, 2, 4)) )
            printf( "Error! Function failed with code: %d!\n", ercode );

        /* dodaj elementy od indeksów 0 do 2 */
        if( (ercode = ms_array_join_slice_inverse(&array2, &array1, 3, 0)) )
            printf( "Error! Function failed with code: %d!\n", ercode );
        
        /* wypisz wszystkie wartości */
        elems = (int*)array2.Items;
        for( iter = 0; iter < array2.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array1 );
        ms_array_free( &array2 );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 0
        Array => Index: 1 with Value: 1
        Array => Index: 2 with Value: 6
        Array => Index: 3 with Value: 7
        Array => Index: 4 with Value: 0
        Array => Index: 5 with Value: 1
        Array => Index: 6 with Value: 2

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę do której elementy będą wstawiane.
    :param asrc: Wskaźnik na tablicę z której elementy będą kopiowane.
    :param offset: Indeks od którego wartości nie będą kopiowane.
    :param count: Ilość elementów w zakresie lub 0.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     USUWANIE ELEMENTÓW
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Usuwanie elementów
---------------------------------------------------------

.. c:function:: int ms_array_slice( void* aptr, size_t offset, size_t count )

    Pozostawia w tablicy elementy z podanego zakresu.
    Funkcja działa w taki sam sposób jak funkcja :c:func:`ms_array_join_slice` z tą różnicą, że operacje
    wykonywane są bezpośrednio na przekazanej tablicy.
    Wszystkie elementy znajdujące się poza podanym zakresem są usuwane.
    Podanie wartości zmiennej ``offset`` innej niż 0, powoduje przesunięcie wszystkich elementów znajdujących
    się w podanym zakresie i wyrównanie ich do indeksu zerowego.
    Wartość 0 w zmiennej ``count`` traktowana jest jako ilość elementów pozostałych do końca tablicy, licząc
    od wartości ``offset``.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 8 );
        int     *elems,
                 ercode;
        size_t   iter;

        if( (ercode = ms_array_insert_values(&array, 0, list, 8)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        /* pozostaw indeksy od 2 do końca */
        if( (ercode = ms_array_slice(&array, 2, 0)) )
            printf( "Error! Failed to slice array! Code: %d.\n", ercode );

        elems = (int*)array.Items;
        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        printf( "-------------------- <= Second Loop\n" );

        /* pozostaw indeksy od 2 do 4 */
        if( (ercode = ms_array_slice(&array, 2, 3)) )
            printf( "Error! Failed to slice array! Code: %d.\n", ercode );

        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 2
        Array => Index: 1 with Value: 3
        Array => Index: 2 with Value: 4
        Array => Index: 3 with Value: 5
        Array => Index: 4 with Value: 6
        Array => Index: 5 with Value: 7
        -------------------- <= Second Loop
        Array => Index: 0 with Value: 4
        Array => Index: 1 with Value: 5
        Array => Index: 2 with Value: 6

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param offset: Indeks od którego wartości nie będą usuwane.
    :param count: Ilość elementów w zakresie lub 0.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:function:: void* ms_array_remove_range( void* aptr, size_t offset, size_t count )

    Usuwa z tablicy elementy znajdujące się w podanym zakresie.
    Funkcja działa w taki sam sposób jak funkcja :c:func:`ms_array_join_slice_inverse` z tą różnicą, że operacje
    wykonywane są bezpośrednio na przekazanej tablicy.
    Jest inwersją funkcji :c:func:`ms_array_slice`, choć nazwa na to nie wskazuje.
    Podanie wartości zmiennej ``count`` innej niż 0, powoduje przesunięcie wszystkich elementów znajdujących się
    poza indeksem o wartości ``offset + count`` i wyrównanie ich do indeksu zerowego.
    Wartość 0 w zmiennej ``count`` traktowana jest jako ilość elementów pozostałych do końca tablicy, licząc
    od wartości ``offset``.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 8 );
        int     *elems,
                 ercode;
        size_t   iter;

        if( (ercode = ms_array_insert_values(&array, 0, list, 8)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        /* usuń indeksy od 5 do końca */
        if( (ercode = ms_array_remove_range(&array, 5, 0)) )
            printf( "Error! Failed to remove elements from array! Code: %d.\n", ercode );

        elems = (int*)array.Items;
        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        printf( "-------------------- <= Second Loop\n" );

        /* usuń indeksy od 2 do 3 */
        if( (ercode = ms_array_remove_range(&array, 2, 2)) )
            printf( "Error! Failed to remove elements from array! Code: %d.\n", ercode );

        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 0
        Array => Index: 1 with Value: 1
        Array => Index: 2 with Value: 2
        Array => Index: 3 with Value: 3
        Array => Index: 4 with Value: 4
        -------------------- <= Second Loop
        Array => Index: 0 with Value: 0
        Array => Index: 1 with Value: 1
        Array => Index: 2 with Value: 4

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param offset: Indeks od którego wartości będą usuwane.
    :param count: Ilość elementów do usunięcia z tablicy lub 0.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:function:: int ms_array_remove( void* aptr, size_t index )

    Usuwa element o podanym indeksie z tablicy.
    W przypadku gdy indeks jest mniejszy niż wartość :c:member:`MS_ARRAY.Length` pomniejszona o jeden,
    wszystkie elementy znajdujące się poza indeksem są przesuwane.
    Próba usunięcia elementu który nie istnieje, kończy się zwróceniem przez funkcję błędu.

    Przykład użycia funkcji::

        int      list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 10 );
        size_t   iter;
        int     *elems;
        int      ercode;

        if( (ercode = ms_array_insert_values(&array, 0, list, 10)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        ercode = 0;
        ercode |= ms_array_remove( &array, 2 ); /* usuwa 2 */
        ercode |= ms_array_remove( &array, 4 ); /* usuwa 5 */
        ercode |= ms_array_remove( &array, 6 ); /* usuwa 8 */

        if( ercode )
            printf( "Error! One of element remove function failed!\n" );

        elems = (int*)array.Items;
        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, elems[iter] );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 0
        Array => Index: 1 with Value: 1
        Array => Index: 2 with Value: 3
        Array => Index: 3 with Value: 4
        Array => Index: 4 with Value: 6
        Array => Index: 5 with Value: 7
        Array => Index: 6 with Value: 9

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param index: Indeks elementu do usunięcia.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     CZYSZCZENIE DANYCH
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Czyszczenie danych
---------------------------------------------------------

.. c:function:: void ms_array_clear( void* aptr )

    Czyści tablicę usuwając jej wszystkie elementy.
    Funkcja nie zwalnia pamięci po elementach i nie zmniejsza jej.

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_return_local( sizeof(int), 5 );

        printf( "Elements: %zu >> Capacity: %zu\n", array.Length, array.Capacity );
        ms_array_insert_values( &array, 0, list, 10 );
        printf( "Elements: %zu >> Capacity: %zu\n", array.Length, array.Capacity );
        ms_array_clear( &array );
        printf( "Elements: %zu >> Capacity: %zu\n", array.Length, array.Capacity );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Elements: 0 >> Capacity: 5
        Elements: 10 >> Capacity: 10
        Elements: 0 >> Capacity: 10

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.

.. c:function:: void ms_array_clean( void* aptr )

    Czyści tablicę zwalniając zasoby przydzielone dla elementów.
    Po takiej operacji tablica wymaga ponownej inicjalizacji.
    W przypaku gdy struktura tablicy jest zmienną lokalną, wywołanie tej funkcji zwalnia z użycia
    funkcji :c:func:`ms_array_free`.

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_return_local( sizeof(int), 20 ),
                 array2 = ms_array_return_local( sizeof(int), 10 ),
                *array3 = ms_array_alloc( sizeof(int), 30 );

        ms_array_clean( &array2 );
        ms_array_copy( &array2, &array1 );

        ms_array_clean( &array1 );
        ms_array_clean( array3 );

        /* w tym monecie nie trzeba usuwać zasobów po array1, gdyż jest zmienną lokalną
           lecz wywołanie tej funkcji nie zaszkodzi i nie spowoduje błędu */
        ms_array_free( &array1 );
    
        /* lecz zmienną array3 należy zwolnić */
        ms_array_free( array3 );

        ms_array_free( &array2 );

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.

.. c:function:: void ms_array_free( void* aptr )

    Zwalnia zasoby przydzielone zarówno do tablicy jak i jej elementów.
    Każdy blok kodu składający się z utworzenia tablicy, należy zakończyć tą funkcją, aby zapobiec wyciekom pamięci.
    Jedyny przypadek w którym funkcji nie trzeba wywoływać został opisany i pokazany w przykładzie użycia funkcji
    :c:member:`ms_array_clean`.

    Przykład zwalniania zasobów::

        MS_ARRAY array = ms_array_return_local( sizeof(int), 10 );
        ms_array_free( &array );

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     MAKRA
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Makra
---------------------------------------------------------

.. c:macro:: type ms_array_get( void* array, type type, size_t index )

    Pobiera element z tablicy o podanym typie i indeksie.
    Makro rzutuje elementy tablicy do typu podanego w parametrze i zwraca wartość znajdującą
    się w podanym indeksie. Zalecane jest rzutowanie bezpośrednie do zmiennej w przypadku
    pobierania większej ilości elementów.
    Makro działa na zmiennej zawierającej wskaźnik do tablicy.

    Przykład użycia makra::

        int       list[] = { 630, 342, 534, 678, 944 };
        MS_ARRAY *array  = ms_array_alloc( sizeof(int), 5 );
        int       ercode;
        size_t    iter;

        if( (ercode = ms_array_insert_values(array, 0, list, 5)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        for( iter = 0; iter < array->Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, ms_array_get(array, int, iter) );

        ms_array_free( array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 630
        Array => Index: 0 with Value: 342
        Array => Index: 0 with Value: 534
        Array => Index: 0 with Value: 678
        Array => Index: 0 with Value: 944

    .. rst-class:: parameters

    :param array: Wskaźnik na tablicę.
    :param type: Typ pobieranego elementu.
    :param index: Indeks elementu do pobrania.
    :return: Element pobrany z tablicy o typie podanym w parametrze.

.. c:macro:: type ms_array_getl( local array, type type, size_t index )

    Pobiera element z tablicy o podanym typie i indeksie.
    Makro rzutuje elementy tablicy do typu podanego w parametrze i zwraca wartość znajdującą
    się w podanym indeksie. Zalecane jest rzutowanie bezpośrednie do zmiennej w przypadku
    pobierania większej ilości elementów.
    Makro działa na zmiennej lokalnej tablicy.

    Przykład użycia makra::

        int      list[] = { 630, 342, 534, 678, 944 };
        MS_ARRAY array  = ms_array_return_local( sizeof(int), 5 );
        int      ercode;
        size_t   iter;

        if( (ercode = ms_array_insert_values(&array, 0, list, 5)) )
            printf( "Error! Failed to insert elements to array! Code: %d.\n", ercode );

        for( iter = 0; iter < array.Length; ++iter )
            printf( "Array => Index: %zu with Value: %d\n", iter, ms_array_getl(array, int, iter) );

        ms_array_free( &array );

    Wyjście:

    .. sourcecode:: none

        Array => Index: 0 with Value: 630
        Array => Index: 0 with Value: 342
        Array => Index: 0 with Value: 534
        Array => Index: 0 with Value: 678
        Array => Index: 0 with Value: 944

    .. rst-class:: parameters

    :param array: Tablica z której element ma być pobrany.
    :param type: Typ pobieranego elementu.
    :param index: Indeks elementu do pobrania.
    :return: Element pobrany z tablicy o typie podanym w parametrze.

.. c:macro:: int ms_array_slice_inverse( void* aptr, size_t offset, size_t count )

    Usuwa z tablicy elementy znajdujące się w podanym zakresie.
    Makro jest aliasem do funkcji o nazwie :c:func:`ms_array_remove_range`, tam też znajduje się
    dokładny opis działania funkcji.

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param offset: Indeks od którego wartości będą usuwane.
    :param count: Ilość elementów do usunięcia z tablicy lub 0.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:macro:: int ms_array_join( void* adst, const void* asrc )

    Dodaje do tablicy elementy z innej tablicy z podanego zakresu.
    Makro jest aliasem do funkcji o nazwie :c:func:`ms_array_join_slice`, tam też znajduje się
    dokładny opis działania funkcji.
    Makro uzupełnia dwa ostatnie parametry wartością 0, co powoduje kopiowanie całej tablicy.

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę do której elementy będą wstawiane.
    :param asrc: Wskaźnik na tablicę z której elementy będą kopiowane.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:macro:: int ms_array_push_values( void* adst, const void* tsrc, size_t count )

    Dodaje do tablicy elementy, przekazane w parametrze, na sam koniec tablicy dynamicznej.
    Makro jest aliasem do funkcji o nazwie :c:func:`ms_array_insert_values`, tam też znajduje się
    dokładny opis działania funkcji.
    Makro uzupełnia indeks o rozmiar tablicy, co powoduje wstawianie elementów na sam koniec.

    .. rst-class:: parameters

    :param adst: Wskaźnik na tablicę.
    :param tsrc: Wskaźnik na pierwszy element z tablicy standardowej do wstawienia.
    :param size: Ilość elementów do dodania.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:macro:: int ms_array_push_value( void* aptr, const void* item )

    Dodaje do tablicy element, przekazany przez wskaźnik, na sam koniec tablicy dynamicznej.
    Makro jest aliasem do funkcji o nazwie :c:func:`ms_array_insert_value`, tam też znajduje się dokładny
    opis działania funkcji.
    Makro uzupełnia indeks o rozmiar tablicy, co powoduje wstawianie elementu na sam koniec.

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :param item: Wskaźnik na element do wstawienia.
    :return: Kod błędu lub :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`

.. c:macro:: int ms_array_remove_last( void* aptr )

    Usuwa ostatni element z tablicy.
    Makro jest aliasem do funkcji o nazwie :c:func:`ms_array_remove`, tam też znajduje się dokładny
    opis działania funkcji.
    Makro uzupełnia indeks o rozmiar tablicy pomniejszony o jeden, co powoduje usunięcie ostatniego elementu.

    .. rst-class:: parameters

    :param aptr: Wskaźnik na tablicę.
    :return: Kod błędu lub wartość :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy: * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. ===================================================================================================================
.. ---------------------------------------------------------------------------------------------------------------
..     BAZA FUNKCJI POCHODNYCH
.. ---------------------------------------------------------------------------------------------------------------
.. ===================================================================================================================

Baza funkcji pochodnych
---------------------------------------------------------

.. c:function:: MS_ARRAY ms_array_return( size_t capacity )

    Zwraca tablicę lokalna bez podawania rozmiaru elementu.
    Funkcja jest podstawą do tworzenia funkcji pochodnych operujących na tablicy, działających na różnych typach.
    Zasada działania jest taka sama jak w przypadku funkcji :c:func:`ms_array_return_local`.

    .. rst-class:: parameters

    :param capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Utworzoną tablicę lokalną.

.. c:function:: MS_ARRAY ms_array_copy_return( const MS_ARRAY* array )

    Kopiuje podaną w parametrze tablicę, tworząc jej lokalny odpowiednik.
    Funkcja jest podstawą do tworzenia funkcji pochodnych operujących na tablicy, działających na różnych typach.
    Zasada działania jest podobna do funkcji :c:func:`ms_array_copy_alloc` z tą różnicą, iż nie przydziela
    miejsca na samą strukturę tablicy.

    .. rst-class:: parameters

    :param capacity: Wskaźnik na tablicę do skopiowania.
    :return: Utworzoną lokalną kopię tablicy.

.. c:function:: int ms_array_push( MS_ARRAY* array, void* item )

    Dodaje element do tablicy na sam koniec.
    Funkcja jest podstawą do tworzenia funkcji pochodnych operujących na tablicy, działających na różnych typach.
    Zasada działania podobna do makra :c:func:`ms_array_push_value` z tą różnicą, iż nie należy podawać
    wskaźnika do elementu, który ma zostać umieszczony w tablicy.

    .. rst-class:: parameters

    :param capacity: Tablica do której ma zostać dodany element.
    :param item: Element do wstawienia.
    :return: Kod błędu lub :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`

.. c:function:: int ms_array_insert( MS_ARRAY* array, size_t index, void* item )

    Doaje element do tablicy w wybrane miejsce.
    Funkcja jest podstawą do tworzenia funkcji pochodnych operujących na tablicy, działających na różnych typach.
    Zasada działania jest podobna do funkcji :c:func:`ms_array_insert_value` z tą różnicą, iż nie należy podawać
    wskaźnika do elementu, który ma zostać umieszczony w tablicy.

    .. rst-class:: parameters

    :param capacity: Wskaźnik do tablicy.
    :param index: Indeks w tablicy do którego wstawiony ma być element.
    :param item: Element do wstawienia.
    :return: Kod błędu lub :c:member:`MSE_ERROR_CODES.MSEC_OK`.
    :Błędy:
        * :c:member:`MSE_ERROR_CODES.MSEC_MEMORY_ALLOCATION`
        * :c:member:`MSE_ERROR_CODES.MSEC_OUT_OF_RANGE`
        * :c:member:`MSE_ERROR_CODES.MSEC_INVALID_ARGUMENT`
