
Tablica dynamiczna elementów tego samego rozmiaru
=========================================================

Tablica dynamiczna składająca się z elementów o tym samym typie.
Teoretycznie tablica powinna posiadać wszystkie elementy tego samego typu, jednak jedyne ograniczenie jakie
stawia, to to, aby wszystkie elementy znajdujące się w tablicy były tego samego rozmiaru.
Możliwe jest utworzenie pochodnych tabeli tylko dla konkretnego typu poprzez makra.
Poniższe funkcje w niektórych przypadkach podczas błędu zapisują do zmiennej ``errno`` jego kod.
Wartość tej zmiennej nie jest nadpisywana gdy błąd występuje bezpośrednio w bibliotekach języka *C*.

Struktury i stałe
---------------------------------------------------------

.. c:type:: MS_ARRAY

    Struktura tablicy dynamicznej do której odwołują się wszystkie funkcje w module.
    Każde wywołanie funkcji tworzącej tablicę musi być zakończone wywołaniem funkcji zwalniającej
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
    Factor                2.f
    Destroy               automatycznie
    Items                 wskaźnik
    Increator             :c:member:`MSS_ARRAYFUNCTIONS.IncMultiply`
    ===================== ==========================================

    .. c:member:: size_t Capacity

        Pojemność tablicy.
        Po przekroczeniu lub wyrównaniu ilości elementów z tą wartością, podczas dodawania elementu,
        następuje przydzielenie większej ilości miejsca w tablicy dla nowych elementów, o wartości obliczanej
        zgodnie z ustawioną funkcją *inkreatora*.
        Wartość ta nie powinna być zmieniana samodzielnie, gdyż może to prowadzić do błędów naruszenia ochrony pamięci
        podczas dodawania elementów oraz w czasie kopiowania tablicy.

        .. highlight:: c

        Przykład automatycznej zmiany pojemności::

            int list[] = { 0, 1, 2 };
            MS_ARRAY array = ms_array_mreturn( sizeof(int), 2 );

            array.Factor = 2.f;
            array.Increator = MSC_ArrayFunctions.IncAdd;

            ms_array_mpush( &array, &list[0] );
            ms_array_mpush( &array, &list[1] );
            printf( "L:%d -> C:%d\n", array.Length, array.Capacity );
            ms_array_mpush( &array, &list[2] );
            printf( "L:%d -> C:%d\n", array.Length, array.Capacity );

            ms_array_free( &array );

    .. c:member:: size_t ItemSize

        Rozmiar pojedynczego elementu.
        Dodawanie elementu przez wskaźnik opiera się na założeniu że każdy element musi mieć taki sam rozmiar.
        Generalnie tablica taka jest tablicą zawierającą elementy tego samego typu.
        Wartość ta nie powinna być zmieniana samodzielnie, gdyż może to prowadzić do błędów naruszenia ochrony pamięci
        nie tylko podczas dodawania czy usuwania poszczególnych elementów, ale również podczas kopiowania tablicy.

    .. c:member:: size_t Length

        Ilość elementów w tablicy.
        Wartość ta reprezentuje aktualną długość tablicy i jest porównywana z pojemnością podczas dodawania elementów.
        Jej zmniejszenie spowoduje obcięcie ilości elementów od końca, zwiększenie zaś zebranie śmieci.
        Ustawienie długości tablicy poza pojemność może prowadzić do błędów naruszenia ochrony pamięci.
        Technika samodzielnej zmiany długości może być przydatna podczas przydzielenia pamięci na elementy przechowywane
        bezpośrednio w tablicy.

        .. highlight:: c

        Przykład samodzielnej zmiany długości::

            struct SAMPLE {
                int x, y, z;
            };

            struct SAMPLE *ptr;
            size_t iter;
            MS_ARRAY array = ms_array_mreturn( sizeof(struct SAMPLE), 4 );

            array.Length = 4;
            ptr = (struct SAMPLE*)array.Items;

            for( iter = 0; iter < array.Length; ++iter )
                ptr[iter].x = iter,
                ptr[iter].y = iter + 1,
                ptr[iter].z = iter + 2;
            
            for( iter = 0; iter < array.Length; ++iter )
                printf( "x:%d -> y:%d -> z:%d\n", ptr[iter].x, ptr[iter].y, ptr[iter].z );

            ms_array_free( &array );

    .. c:member:: float Factor

        Mnożnik kontrolujący powiększenie pojemności tablicy.
        Zastosowanie mnożnika jak i również przyjmowane przez niego wartości, są uzależnione od aktualnie
        ustawionej funkcji zwiększającej pojemność tablicy, do której wartość ta jest zawsze przekazywana.
        Ustawienie złej wartości może spowodować złe obliczenia nowej pojemności.
        Zastosowania mnożnika można znaleźć w opisach pól struktury :c:type:`MSS_ARRAYFUNCTIONS`.

        .. highlight:: c

        Przykład używania mnożnika::

            int list[] = { 0, 1, 2, 4 };
            MS_ARRAY array = ms_array_mreturn( sizeof(int), 1 );

            /* funkcja potęgowa */
            array.Factor = 1.1f;
            array.Increator = MSC_ArrayFunctions.IncPower;

            ms_array_mpush( &array, &list[0] );
            /* tutaj zadziała, 1^1.1 to dalej 1
               L == P, więc do pojemności dodawana jest wartość 1 */
            ms_array_mpush( &array, &list[1] );
            printf( "L:%d -> C:%d\n", array.Length, array.Capacity );

            array.Factor = 5.8f;
            /* tutaj zadziała, 2^5.8 to ~55.72, zaokrąglanie w dół, 55 */
            ms_array_mpush( &array, &list[2] );
            printf( "L:%d -> C:%d\n", array.Length, array.Capacity );

            ms_array_free( &array );

    .. c:member:: bool Destroy

        Informacja o tym, czy wszystko ma zostać zniszczone, wykorzystywana tylko i wyłącznie w funkcji
        :c:func:`ms_array_free` odpowiedzialnej za zwalnianie przydzielonych zasobów przeznaczonych na tablicę.
        Wartość ustawiana jest na ``TRUE`` tylko i wyłącznie w funkcjach zwracających wskaźnik do nowej tablicy.
        O ile wszystko to dzieje się automatycznie, o tyle samodzielna zmiana tej wartości może prowadzić
        zarówno do ochrony naruszenia jak i wycieków pamięci.

    .. c:member:: void** Items

        Elementy tablicy.
        W zależności od implementacji pochodnej struktury tablicy i wartościami przechowywanymi w niej, przed
        pobraniem elementu należy rzutować go do odpowiedniego typu.
        Pojedynczy element można szybko pobrać za pomocą jednego z wbudowanych makr, :c:macro:`ms_array_mget`
        lub :c:macro:`ms_array_mgetl`.
        W przypadku większej liczby elementów lub nawet całego zbioru, warto przypisać wszystkie do osobnej
        zmiennej, iterując po niej w pętli.

        .. highlight:: c

        Dwie techniki prostego pobierania zmiennych z tablicy::

            int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            int *elem;
            size_t iter;
            MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );

            /* dodaj wartości */
            ms_array_tpush( &array, list, 10 );

            /* pierwszy sposób */
            elem = (int*)array.Items;
            for( iter = 0; iter < array.Length; ++iter )
                printf( "%d ", elem[iter] );

            fputs( "\n", stdout );

            /* drugi sposób - spowoduje zmianę pierwszego elementu tablicy */
            for( iter = 0; iter < array.Length; ++iter )
                *elem = ms_array_mgetl( array, int, iter ),
                printf( "%d ", *elem );

    .. c:member:: size_t Increator(size_t capacity, float factor)

        Funkcja zwiększająca pojemność tablicy, używana podczas przydzielania pamięci dla jej nowych elementów.
        Różne funkcje dają różne wyniki. Dzięki temu polu można przypisać własną funkcję obliczającą nową ilość
        potrzebnej pamięci. Zwracaną wartość można modyfikować, zmieniając zawartość pola :c:member:`MS_ARRAY.Factor`.
        Wszystkie wskaźniki wbudowanych funkcji inkreatora znajdują się w stałej :c:data:`MSC_ArrayFunctions`,
        rozpoczynają się od przedrostka *Inc* oraz zawsze zaokrąglają wynik w dół.

        Zestawienie wbudowanych funkcji inkreatora i ich wyniki przy takich samych parametrach:

        ===================== =========== =========== ============
        Funkcja inkreatora    Pojemność   Mnożnik     Wynik
        ===================== =========== =========== ============
        IncAdd                3           4.5         7
        IncMultiply           3           4.5         13
        IncPower              3           4.5         140
        ===================== =========== =========== ============

        :param size_t capacity: Aktualna pojemność tablicy.
        :param float factor: Liczba traktowana jako mnożnik pojemności.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia.
        :rtype: size_t

.. c:type:: MSS_ARRAYFUNCTIONS

    Struktura zawiera pola posiadające wskaźniki do wbudowanych funkcji modułu.
    Aktualnie wszystkie funkcje wykorzystywane są tylko do powiększania pojemności tablicy.
    Zmienna ``factor`` w argumentach funkcji może być zarówno dodatnia jak i ujemna, jednak wynik
    końcowy teoretycznie nie może być mniejszy niż wprowadzony do funkcji w zmiennej ``capacity``.
    W praktyce jednak, w przypadku gdy wynik końcowy będzie mniejszy, nowa pojemność tablicy zostanie
    ustawiona jako ``capacity + 1``. Wszystkie funkcje wbudowane pozwalające zwiększyć pojemność
    tablicy, zaokrąglają zwracany wynik zawsze w dół.

    .. c:member:: size_t IncMultiply(size_t capacity, float factor)

        Modyfikuje wartość zmiennej ``capacity``, mnożąc ją ze zmienną ``factor``.

        :param size_t capacity: Aktualna pojemność tablicy.
        :param float factor: Dodatkowy współczynnik do obliczeń.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia pamięci.
        :rtype: size_t

    .. c:member:: size_t IncAdd(size_t capacity, float factor)

        Modyfikuje wartość zmiennej ``capacity``, dodając do niej wartość zmiennej ``factor``.

        :param size_t capacity: Aktualna pojemność tablicy.
        :param float factor: Liczba o którą pojemność zostanie powiększona.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia pamięci.
        :rtype: size_t

    .. c:member:: size_t IncPower(size_t c, float f)

        Modyfikuje wartość zmiennej ``capacity``, podnosząc ją do potęgi wartości zmiennej ``factor``.

        :param size_t capacity: Aktualna pojemność tablicy, traktowana jako podstawa potęgi.
        :param float factor: Liczba traktowana jako wykładnik potęgi.
        :return: Nową pojemność tablicy przekazywaną do przydzielenia pamięci.
        :rtype: size_t

.. c:var:: MSS_ARRAYFUNCTIONS MSC_ArrayFunctions

    Stała przechowująca wskaźniki do wbudowanych funkcji modułu pozwalających na zmianę pojemności tablicy.
    Funkcję obliczającą nową pojemność tablicy można zmienić, przypisując do pola :c:member:`MS_ARRAY.Increator`
    jedną z funkcji podanych w strukturze rozpoczynających się od przedrostka *Inc*. Wszystkie funkcje opisane
    są w polach struktury :c:type:`MSS_ARRAYFUNCTIONS`.

Tworzenie i inicjalizacja
---------------------------------------------------------

.. c:function:: void* ms_array_alloc( size_t size, size_t capacity )

    Tworzy nową tablicę oraz rezerwuje początkowe miejsce na dane.
    W odróżnieniu od inicjalizacji, funkcja zwraca wskaźnik do utworzonej tablicy.
    W przypadku błędu podczas tworzenia, funkcja zwraca wartość *NULL*.
    Funkcja jako jedna z nielicznych ustawia wartość pola :c:member:`MS_ARRAY.Destroy` na wartość ``TRUE``.
    Przydzieloną pamięć na tablicę zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY *array = NULL;

        array = ms_array_alloc( sizeof(int), 100 );
        if( array == NULL ) 
            printf( "Wystąpił błąd podczas tworzenia tablicy: %d\n", errno );
        else
            printf( "Zarezerwowano pamięć na %d elementów.\n", array->Capacity );

        ms_array_free( array );
        array = NULL;

    :param size_t size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Wskaźnik do utworzonej tablicy lub *NULL* w przypadku błedu.
    :rtype:  void*

.. c:function:: int ms_array_init( void *aptr, size_t size, size_t capacity )

    Inicjalizuje istniejącą tablicę i rezerwuje początkowe miejsce na dane.
    Funkcja w przypadku wystąpienia błędu zwraca jego kod, w przeciwnym wypadku zwraca wartość `MSEC_OK`.
    Utworzoną w taki sposób tablicę zawsze należy zwalniać, co umożliwia funkcja :c:func:`ms_array_free`.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array;
        int ercode;

        if( (ercode = ms_array_init(&array, sizeof(int), 100)) )
            printf( "Wystąpił bład podczas tworzenia tablicy o kodzie: %d\n", errno );
        else
            printf( "Zarezerwowano pamięć na %d elementów.\n", array->Capacity );

        ms_array_free( &array );
        array = NULL;

    :param void* aptr: Wskaźnik do zadeklarowanej wcześniej tablicy.
    :param size_t size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Kod błędu lub wartość ``MSEC_OK``.
    :rtype: int

.. mreturn -> memory return
.. c:function:: MS_ARRAY ms_array_mreturn( size_t size, size_t capacity )

    Tworzy tablicę lokalną oraz rezerwuje początkowe miejsce na dane.
    Odmiana tej funkcji pozwala na ustawienie rozmiaru pojedynczego elementu.
    Zaraz po utworzeniu warto sprawdzić, czy pole :c:member:`MS_ARRAY.Items`, nie jest równe *NULL*.
    Utworzona w ten sposób tablica nadal wymaga zwolnienia pamięci poprzez wywołanie funkcji
    :c:func:`ms_array_free`.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_mreturn( sizeof(int), 100 );
        
        if( array.Items == NULL )
            printf( "Wystąpił błąd podczas tworzenia tablicy: %d\n", errno );
        else
            printf( "Zarezerwowano pamięć na %d elementów.\n", array->Capacity );

        ms_array_free( &array );

    :param size_t size: Rozmiar pojedynczego elementu przechowywanego w tablicy.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Utworzoną tablicę lokalną.
    :rtype: MS_ARRAY

Zmiana pojemności
---------------------------------------------------------

.. c:function:: int ms_array_realloc( void* aptr, size_t capacity )

    Zmniejsza lub zwiększa pojemność tablicy.
    Funkcja posiada dwa tryby działania - zwiększanie automatyczne lub zmiana pojemności do konkretnej wartości.
    Automatyczne zwiększanie można uzyskać poprzez podanie w miejsce zmiennej ``capacity`` wartości 0.
    W takim przypadku do obliczania nowej pojemności używany jest *inkreator* zdefiniowany w tablicy.
    Gdy tablica nie posiada przypisanego *inkreatora*, funkcja zwróci błąd o kodzie ``MSEC_INVALID_ARGUMENT``.
    Wyjście *inkreatora* jest sprawdzane i w przypadku gdy wartość jest mniejsza lub równa ilości elementów,
    do aktualnej pojemności dodawana jest wartość 1.
    Drugi tryb zaczyna działać gdy w miejsce zmiennej ``capacity`` podana zostanie liczba inna niż 0.
    W przypadku gdy liczba ta jest mniejsza niż wartość pola :c:member:`MS_ARRAY.Length`, zwracany jest błąd
    o kodzie ``MSEC_DATA_OVERFLOW``. Tak więc pojemność w tym trybie może być zwiększana lub zmniejszana,
    jedynym ograniczeniem jest to, że musi być większa lub równa ilości elementów w tablicy.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_mreturn( sizeof(int), 3 );
        int ercode;
        
        array.Increator = MSC_ArrayFunctions.IncPower;
        array.Factor = 2.57f;

        /* 3^2.5 = ~16.83 co daje po zaokrągleniu 16 */
        if( (ercode = ms_array_realloc(&array, 0)) )
            printf( "Wystąpił błąd podczas zmiany pojemności tablicy: %d\n", ercode );
        printf( "Nowa pojemność tablicy: %d\n", array.Capacity );

        /* zmiana pojemności tablicy do podanej wartości */
        if( (ercode = ms_array_realloc(&array, 30)) )
            printf( "Wystąpił bład podczas zmiany pojemności tablicy: %d\n", ercode );
        printf( "Nowa pojemność tablicy: %d\n", array.Capacity );

        /* spodziewany błąd, brak inkreatora */
        array.Increator = NULL;
        if( (ercode = ms_array_realloc(&array, 0)) == MSEC_INVALID_ARGUMENT )
            printf( "Tablica nie ma przypisanego inkreatora!\n" );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy przeznaczonej do zmiany pojemności.
    :param size_t capacity: Nowa pojemność tablicy lub 0 w przypadku przydziału automatycznego.
    :return: Kod błędu lub wartość ``MSEC_OK``.
    :rtype: int

.. c:function:: int ms_array_min_realloc( void* aptr, size_t min )

    Zwiększa pojemność tablicy do co najmniej podanej wartości w parametrze ``min``.
    Podanie wartości mniejszej niż aktualna pojemność nie kończy się błędem, ale również nie zmienia
    pojemności całej tablicy, ponieważ wartość minimalna została już osiągnięta.
    Funkcja wywołuje *inkreatora* dopóty, dopóki wartość przez niego zwracana nie będzie większa lub
    równa wartości pola :c:member:`MS_ARRAY.Capacity`. W przypadku gdy pole *inkreatora* będzie równe
    wartości *NULL*, jako nowa pojemność przyjęta zostanie wartość minimalna, podana w parametrze ``min``.
    Funkcja szczególnie przydatna w trakcie wstawiania tablic, gdzie za jednym razem przydzielana jest
    odpowiednia ilość pamięci, pozwalająca na dodanie wszystkich elementów.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_mreturn( sizeof(int), 2 );
        int ercode;

        array.Factor = 1.5f;
        array.Increator = MSC_ArrayFunctions.IncPower;

        /* 3^1.5 ~= 5, 5^1.5 ~= 11, 11^1.5 ~= 36 -> STOP
           wartość minimalna (20) została osiągnięta */
        if( (ercode = ms_array_min_realloc(&array, 20)) )
            printf( "Wystąpił bład podczas zmiany pojemności tablicy: %d\n", ercode );
        printf( "Nowa pojemność tablicy: %d\n", array.Capacity );

        // dokładne zwiększanie, powinna być osiągnięta tylko wartość minimalna
        array.Increator = NULL;
        if( (ercode = ms_array_min_realloc(&array, 256)) )
            printf( "Wystąpił bład podczas zmiany pojemności tablicy: %d\n", ercode );
        printf( "Nowa pojemność tablicy: %d\n", array.Capacity );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy przeznaczonej do zmiany pojemności.
    :param size_t min: Minimalna wartość do której zwiększona zostanie pojemność tablicy.
    :return: Kod błędu lub wartość ``MSEC_OK``.
    :rtype: int

Kopiowanie
---------------------------------------------------------

.. c:function:: int ms_array_copy( void* asrc, void* adst )

    Kopiuje całą tablicę do drugiej tablicy.
    Druga tablica musi istnieć i nie może być zaincjalizowana.
    W przypadku gdy do parametru ``adst`` podany zostanie wskaźnik zawierający już istniejącą tablicę,
    wszystkie dane zostaną nadpisane, co może się wiązać z wyciekiem pamięci.
    Dlatego przed przekazaniem istniejącej i utworzonej lub zainicjalizowanej wcześniej tablicy do parametru
    ``adst``, należy ją wyczyścić używając funkcji :c:func:`ms_array_clean`.
    Funkcja nie może sama czyścić tablicy, gdyż podanie zadeklarowanej tablicy może skończyć się naruszeniem
    ochrony pamięci w przypadku gdy jej zawartość nie zostanie wyzerowana.
    Należy również brać pod uwagę to, iż samo zerowanie nie urchoni całkowicie przed błędem naruszenia
    ochrony pamięci, gdyż w niektórych przypadkach wartość *NULL* nie zawsze posiada wszystkie bity równe 0.
    W przypadku gdy elementy są wskaźnikami do struktur lub innych danych, dane te nie są kopiowane.
    Pamięć po tablicy do której dane będą kopiowane należy zwolnić funkcją :c:func:`ms_array_free`.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_mreturn( sizeof(int), 20 );
        MS_ARRAY array2 = ms_array_mreturn( sizeof(int), 10 );
        MS_ARRAY array3;
        int ercode;

        if( (ercode = ms_array_copy(&array2, &array3)) )
            printf( "Błąd kopiowania tablicy o kodzie: %d\n", ercode );
        ms_array_clean( &array2 );
        if( (ercode = ms_array_copy(&array1, &array2) )
            printf( "Błąd kopiowania tablicy o kodzie: %d\n", ercode );

        ms_array_free( &array1 );
        ms_array_free( &array2 );
        ms_array_free( &array3 );

    :param void* asrc: Kopiowana tablica.
    :param void* adst: Tablica do której dane będą kopiowane.
    :return: Kod błedu lub wartość ``MSEC_OK``.
    :rtype: int

.. c:function:: void* ms_array_copy_alloc( void* aptr )

    Tworzy nową tablicę i kopiuje do niej dane z tablicy podanej w parametrze.
    W przypadku gdy elementy są wskaźnikami do struktur lub innych danych, dane te nie są kopiowane.
    Funkcja jako jedna z nielicznych ustawia wartość pola :c:member:`MS_ARRAY.Destroy` na wartość ``TRUE``.
    Utworzoną kopię po zakończeniu operacji z nią związanych należy zwolnić funkcją :c:func:`ms_array_free`.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_mreturn( sizeof(int), 20 );
        MS_ARRAY *array2;

        if( !(array2 = ms_array_copy_alloc(array1)) )
            printf( "Błąd kopiowania tablicy o kodzie: %d\n", errno );

        ms_array_free( &array1 );
        ms_array_free( array2 );

    :param void* aptr: Kopiowana tablica.
    :return: Utworzoną kopię tablicy podanej w parametrze lub *NULL*.
    :rtype: void*

Dodawanie elementów
---------------------------------------------------------

.. mpush -> memory push
.. c:function:: int ms_array_mpush( void* aptr, void* item )

    Dodaje element przekazany przez wskaźnik na sam koniec tablicy.
    Element jest kopiowany wprost do tablicy z zachowaniem rozmiaru z pola :c:member:`MS_ARRAY.ItemSize`.
    Z tego powodu element przekazany przez referencję lub element na który wskazuje zmienna, musi mieć taki
    sam typ lub taki sam rozmiar jak rozmiar zadeklarowany podczas tworzenia tablicy.
    W przeciwnym wypadku skopiowana zostanie tylko część lub nadmiar danych, co może powodować naruszenie
    ochrony pamięci.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 3 );
        int *elems;

        ms_array_mpush( &array, &list[0] );
        ms_array_mpush( &array, &list[1] );
        ms_array_mpush( &array, &list[2] );
        
        elems = (int*)array.Items;

        printf( "1:%d => 2:%d => 3:%d\n", elems[0], elems[1], elems[2] );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy do której ma zostać dodany element.
    :param void* item: Element do dodania na koniec tablicy.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. minsert -> memory insert
.. c:function:: int ms_array_minsert( void* aptr, size_t index, void* item )

    Dodaje element przekazany przez wskaźnik do tablicy we wskazane miejsce.
    Zasada działania funkcji jest taka sama jak w przypadku funkcji :c:func:`ms_array_push` z tą różnicą,
    że element wstawiany jest miejsce wskazane przez parametr a nie zawsze na końcu.
    Kopiowanie elementu w miejsce inne niż sam koniec tablicy wiąże się z przeniesieniem wszystkich danych
    występujących powyżej indeksu do którego skopiowany zostanie element.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 3 );
        int *elems;

        ms_array_minsert( &array, 0, &list[0] );
        ms_array_minsert( &array, 0, &list[1] );
        ms_array_minsert( &array, 1, &list[2] );
        
        elems = (int*)array.Items;

        printf( "1:%d => 2:%d => 3:%d\n", elems[0], elems[1], elems[2] );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy do której ma zostać dodany element.
    :param size_t index: Indeks do którego zostanie skopiowany element.
    :param void* item: Element do dodania na koniec tablicy.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. apushr -> array push range
.. c:function:: int ms_array_apush_slice( void* aptrt, void* aptrf, size_t offset, size_t count )

.. ainsertr -> array insert range
.. c:function:: int ms_array_ainsert_slice( void* aptrt, size_t index, void* aptrf, size_t offset, size_t count )

.. tpush -> table push
.. c:function:: int ms_array_tpush( void* aptr, void* tab, size_t size )

    Dodaje elementy przekazane w parametrze na sam koniec tablicy.
    Elementy muszą być zapisane w tablicy statycznej - standardowej tablicy języka *C*.
    Kopiowanie elementów działa na takiej samej zasadzie jak w przypadku funkcji :c:func:`ms_array_mpush`
    z tą różnicą że zamiast jednego elementu, na sam koniec tablicy dodawane jest ich kilka.
    Przekazanie rozmiaru przewyższającego ilość elementów w tablicy może spowodować naruszenie ochrony
    pamięci. Przekazanie mniejszego rozmiaru może w prosty sposób ograniczyć ilość dodawanych elementów.
    Dzięki temu można tworzyć zakres wraz z modyfikacją pierwszego kopiowanego elementu tablicy.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        int *elems, ercode;
        size_t iter;

        if( (ercode = ms_array_tpush(&array, list, 10)) )
            printf( "Wystąpił błąd podczas dodawania elementów: %d\n", ercode );
        /* dodaje elementy od indeksu 3 do (3 + 5 = 8) */
        if( (ercode = ms_array_tpush(&array, &list[3], 5)) )
            printf( "Wystąpił błąd podczas dodawania elementów: %d\n", ercode );
        
        elems = (int*)array.Items;
        for( iter = 0; iter < array->Length; ++iter )
            printf( "> %d\n", elems[iter] );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy do której ma zostać dodany element.
    :param void* tab: Elementy przechowywane w standardowej tablicy do dodania.
    :param size_t size: Ilość elementów do dodania.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. tinsert -> table insert
.. c:function:: int ms_array_tinsert( void* aptr, size_t index, void* tab, size_t size )

    Dodaje elementy przekazane w parametrze w wybrane miejsce.
    Kopiowanie elementów działa na takiej samej zasadzie jak w przypadku funkcji :c:func:`ms_array_tpush`
    z tą różnicą że elementy wstawiane są kolejno we wskazane miejsce, a nie na końcu.
    Kopiowanie elementów w miejsce inne niż sam koniec tablicy wiąże się z przeniesieniem wszystkich danych
    występujących powyżej indeksu do którego kopiowane będą elementy.
    Wszystkie kopiowane elementy będą okupowały indeksy od ``index`` do ``index + size``.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        int *elems, ercode;
        size_t iter;

        if( (ercode = ms_array_tinsert(&array, 0, list, 10)) )
            printf( "Wystąpił błąd podczas dodawania elementów: %d\n", ercode );
        /* dodaje elementy od indeksu 3 do (3 + 5 = 8) w środek tablicy */
        if( (ercode = ms_array_tinsert(&array, 5, &list[3], 5)) )
            printf( "Wystąpił błąd podczas dodawania elementów: %d\n", ercode );

        elems = (int*)array.Items;
        for( iter = 0; iter < array->Length; ++iter )
            printf( "> %d\n", elems[iter] );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik tablicy do której ma zostać dodany element.
    :param size_t index: Indeks początkowy od którego wstawiane bedą elementy z tablicy.
    :param void* tab: Elementy przechowywane w standardowej tablicy do dodania.
    :param size_t size: Ilość elementów do dodania.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

Usuwanie elementów
---------------------------------------------------------

.. mpop -> memory pop
.. c:function:: void* ms_array_mpop( void* aptr )

    Usuwa ostatni element z tablicy i zwraca wskaźnik do jego kopii.
    Przed usunięciem tworzona jest kopia elementu do zwrócenia.
    Zasoby po zwróconym w ten sposób elemencie należy później zwolnić funkcją :c:func:`free`,
    aby zapobiec wyciekom pamięci.
    W przypadku błędu podczas ściągania elementu funkcja zwraca wartość *NULL*.

    .. highlight:: c

    Przykład ściągania ostatniego elementu z tablicy::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        int *elem;

        ms_array_tpush( &array, list, 10 );
        while( (elem = ms_array_mpop(&array)) )
            printf( "> %d\n", *elem ),
            free( elem );
        printf( "L:%d\n", array.Length );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik do tablicy z której element ma zostać usunięty.
    :return: Wskaźnik do kopii usuwanego elementu.
    :rtype: void*

.. nrpop -> no return pop
.. c:function:: void ms_array_nrpop( void* aptr )

    Usuwa ostatni element z tablicy.
    Funkcja nie zwraca elementu w odróżnieniu od funkcji :c:func:`ms_array_mpop`.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );

        ms_array_tpush( &array, list, 10 );
        while( array.Length )
            ms_array_nrpop( &array );
        printf( "L:%d\n", array.Length );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik do tablicy z której element ma zostać usunięty.
    :rtype: void

.. c:function:: void ms_array_remove( void* aptr, size_t index )

    Usuwa element o podanym indeksie z tablicy.
    Funkcja przesuwa wszystkie elementy znajdujące się za indeksem zaraz po usunięciu elementu
    w przypadku gdy indeks będzie mniejszy niż wartość :c:member:`MS_ARRAY.Length` pomniejszona o jeden.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        size_t iter;
        int *elems;

        ms_array_tpush( &array, list, 10 );
        ms_array_remove( &array, 2 ); /* usuwa 2 */
        ms_array_remove( &array, 4 ); /* usuwa 5 */
        ms_array_remove( &array, 6 ); /* usuwa 8 */

        elems = (int*)array.Items;
        for( iter = 0; iter < array.Length; ++iter )
            printf( "> %d\n", elems[iter] );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik do tablicy z której element ma zostać usunięty.
    :param size_t index: Indeks elementu do usunięcia.
    :rtype: void

Czyszczenie danych
---------------------------------------------------------

.. c:function:: void ms_array_clear( void* aptr )

    Czyści tablicę usuwając jej wszystkie elementy.
    Funkcja nie zwalnia pamięci po elementach i nie zmniejsza jej.

    .. highlight:: c

    Przykład użycia funkcji::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );

        ms_array_tpush( &array, list, 10 );
        printf( "Ilość elementów: %d, pojemność: %d\n", array.Length, array.Capacity );
        ms_array_clear( &array );
        printf( "Ilość elementów: %d, pojemność: %d\n", array.Length, array.Capacity );

        ms_array_free( &array );

    :param void* aptr: Wskaźnik do tablicy przeznaczonej do wyczyszczenia.
    :rtype: void

.. c:function:: void ms_array_clean( void* aptr )

    Czyści tablicę zwalniając zasoby przydzielone dla elementów.
    Po takiej operacji tablica wymaga ponownej inicjalizacji.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array1 = ms_array_mreturn( sizeof(int), 20 );
        MS_ARRAY array2 = ms_array_mreturn( sizeof(int), 10 );

        ms_array_clean( &array2 );
        ms_array_copy( &array1, &array2 );

        ms_array_free( &array1 );
        ms_array_free( &array2 );

    :param void* aptr: Wskaźnik do tablicy przeznaczonej do wyczyszczenia.
    :rtype: void

.. c:function:: void ms_array_free( void* aptr )

    Zwalnia zasoby przydzielone zarówno do tablicy jak i jej elementów.

    .. highlight:: c

    Przykład użycia funkcji::

        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        ms_array_free( &array );

    :param void* aptr: Wskaźnik do tablicy przeznaczonej do usunięcia.
    :rtype: void

Makra
---------------------------------------------------------

.. c:macro:: type ms_array_mget(void* array, type type, size_t index)

    Pobiera element z tablicy o podanym typie i indeksie.
    Makro rzutuje elementy tablicy do typu podanego w parametrze i zwraca wartość znajdującą
    się na podanym indeksie. Zalecane rzutowanie bezpośrednie do zmiennej w przypadku
    pobierania większej ilości elementów.
    Makro działa na zmiennej zawierającej wskaźnik do tablicy.

    .. highlight:: c

    Przykład użycia makra::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY *array = ms_array_alloc( sizeof(int), 10 );
        int elem;

        ms_array_tpush( array, list, 10 );

        for( iter = 0; iter < array.Length; ++iter )
            elem = ms_array_mget( array, int, iter ),
            printf( "%d ", *elem );

        ms_array_free( array );

    :param void* array: Wskaźnik do tablicy z której dane będą pobierane.
    :param type type: Typ pobieranego elementu.
    :param size_t index: Indeks elementu do pobrania.
    :return: Element pobrany z tablicy o typie podanym w parametrze.
    :rtype: type

.. c:macro:: type ms_array_mgetl(local array, type type, size_t index)

    Pobiera element z tablicy o podanym typie i indeksie.
    Makro rzutuje elementy tablicy do typu podanego w parametrze i zwraca wartość znajdującą
    się na podanym indeksie. Zalecane rzutowanie bezpośrednie do zmiennej w przypadku
    pobierania większej ilości elementów.
    Makro działa na zmiennej lokalnej tablicy.

    .. highlight:: c

    Przykład użycia makra::

        int list[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        MS_ARRAY array = ms_array_mreturn( sizeof(int), 10 );
        int elem;

        ms_array_tpush( &array, list, 10 );

        for( iter = 0; iter < array.Length; ++iter )
            elem = ms_array_mgetl( array, int, iter ),
            printf( "%d ", *elem );

        ms_array_free( &array );

    :param local array: Tablica z której dane będą pobierane.
    :param type type: Typ pobieranego elementu.
    :param size_t index: Indeks elementu do pobrania.
    :return: Element pobrany z tablicy o typie podanym w parametrze.
    :rtype: type

.. salloc -> sizeof alloc
.. c:macro:: void* ms_array_salloc(type type, size_t capacity)

    Makro służące do tworzenia tablicy, skracające wywołanie o operator ``sizeof``.
    Uruchamia funkcję :c:func:`ms_array_alloc`.

    .. highlight:: c

    Przykład użycia makra::

        MS_ARRAY *array = ms_array_salloc( int, 100 );
        ms_array_free( array );

    :param type type: Typ elementów przechowywanych w tablicy.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Wskaźnik do utworzonej tablicy lub *NULL* w przypadku błedu.
    :rtype: void*

.. aalloc -> auto alloc
.. c:macro:: void* ms_array_aalloc(void* array, size_t capacity)

    Makro słuzące do tworzenia tablicy, skracające wywołanie o podawanie typu elementów.
    Jako przechowywany typ przyjmowany jest aktualny typ pola :c:member:`MS_ARRAY.Items`.
    Uruchamia funkcję :c:func:`ms_array_alloc`.

    .. highlight:: c

    Przykład użycia makra::

        MS_ARRAY *array = ms_array_aalloc( array, 100 );
        ms_array_free( array );

    :param void* array: Wskaźnik do tablicy z której ma być wyciągnięty typ.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Wskaźnik do utworzonej tablicy lub *NULL* w przypadku błedu.
    :rtype: void*

.. sinit -> sizeof init
.. c:macro:: int ms_array_sinit(void* array, type type, size_t capacity)

    Makro służące do inicjalizacji tablicy, skracające wywołanie o operator ``sizeof``.
    Uruchamia funkcję :c:func:`ms_array_init`.

    .. highlight:: c

    Przykład użycia makra::

        MS_ARRAY array;
        if( ms_array_sinit(&array, int, 100) )
            printf( "Wystąpił bład podczas inicjalizacji tablicy.\n" );
        ms_array_free( &array );

    :param void* array: Wskaźnik tablicy do inicjalizacji.
    :param type type: Typ elementów przechowywanych w tablicy.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. ainit - auto init
.. c:macro:: int ms_array_ainit(void* array, size_t capacity)

    Makro słuzące do inicjalizacji tablicy, skracające wywołanie o podawanie typu elementów.
    Jako przechowywany typ przyjmowany jest aktualny typ pola :c:member:`MS_ARRAY.Items`.
    Uruchamia funkcję :c:func:`ms_array_init`.

    .. highlight:: c

    Przykład użycia makra::

        MS_ARRAY array;
        if( ms_array_ainit(&array, 100) )
            printf( "Wystąpił bład podczas inicjalizacji tablicy.\n" );
        ms_array_free( &array );

    :param void* array: Wskaźnik tablicy do inicjalizacji oraz z której będzie pobierany typ.
    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. c:macro:: int ms_array_apush( void* aptrt, void* aptrf )

.. c:macro:: int ms_array_ainsert( void* aptrt, size_t index, void* aptrf )

.. c:macro:: int ms_array_tpush_slice( void* aptrt, void* tab, size_t offset, size_t count )

.. c:macro:: int ms_array_tinsert_slice( void* aptrt, size_t index, void* tab, size_t offset, size_t count )

Baza funkcji pochodnych
---------------------------------------------------------

.. c:function:: MS_ARRAY ms_array_return( size_t capacity )

    Wbudowana odmiana funkcji zwracającej tablicę lokalną bez podawania rozmiaru elementu.
    Działa na takiej samej zasadzie jak funkcja :c:func:`ms_array_mreturn`.
    Funkcja jest podstawą do tworzenia funkcji pochodnych działających na innych typach.

    :param size_t capacity: Początkowa ilość zarezerwowanego miejsca na elementy tablicy.
    :return: Utworzoną tablicę lokalną.
    :rtype: MS_ARRAY

.. c:function:: MS_ARRAY ms_array_copy_return( MS_ARRAY* array )

    Wbudowana odmiana funkcji kopiującej podaną tablicę, tworząc jej lokalny odpowiednik.
    Działa na takiej samej zasadzie jak funkcja :c:func:`ms_array_copy_alloc` z tą różnicą,
    iż nie przydziela miejsca na samą strukturę tablicy.
    Funkcja jest podstawą do tworzenia funkcji pochodnych działających na innych typach.

    :param MS_ARRAY* capacity: Tablica przeznaczona do kopiowania.
    :return: Utworzoną lokalną kopię tablicy.
    :rtype: MS_ARRAY

.. c:function:: int ms_array_push( MS_ARRAY* array, void* item )

    Wbudowana odmiana funkcji dodającej element do tablicy na sam koniec.
    Działa na takiej samej zasadzie jak funkcja :c:func:`ms_array_mpush` z tą różnicą,
    iż nie trzeba podawać wskaźnika do elementu który zostanie umieszczony w tablicy.
    Funkcja jest podstawą do tworzenia funkcji pochodnych działających na innych typach.

    :param MS_ARRAY* capacity: Tablica do której ma zostać dodany element.
    :param void* item: Element do wstawienia.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. c:function:: int ms_array_insert( MS_ARRAY* array, size_t index, void* item )

    Wbudowana odmiana funkcji dodającej element w wybrane miejsce do tablicy.
    Działa na takiej samej zasadzie jak funkcja :c:func:`ms_array_minsert` z tą różnicą,
    iż nie trzeba podawać wskaźnika do elementu który zostanie umieszczony w tablicy.
    Funkcja jest podstawą do tworzenia funkcji pochodnych działających na innych typach.

    :param MS_ARRAY* capacity: Tablica do której ma zostać dodany element.
    :param size_t index: Indeks w tablicy do którego wstawiony ma być element.
    :param void* item: Element do wstawienia.
    :return: Kod błędu lub ``MSEC_OK``.
    :rtype: int

.. c:function:: void* ms_array_pop( MS_ARRAY* array )

    Wbudowana odmiana funkcji usuwającej i zwracającej ostatni element z tablicy.
    Działa na takiej samej zasadzie jak funkcja :c:func:`ms_array_mpop` z tą różnicą,
    iż nie można zwalniać pamięci po elemencie, gdyż jest on lokalną kopią usuwanego elementu.
    Funkcja jest podstawą do tworzenia funkcji pochodnych działających na innych typach.

    :param MS_ARRAY* capacity: Tablica z której ma zostać ściągnięty element.
    :return: Lokalna kopia ściągniętego elementu.
    :rtype: void*
