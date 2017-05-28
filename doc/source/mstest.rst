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

Testy jednostkowe
=========================================================

Biblioteka Moss posiada w swoim zbiorze modułów plik, zawierający funkcje, ułatwiające tworzenie testów jednostkowych.
Testowanie kodu jest stosunkowo ważną rzeczą w trakcie tworzenia oprogramowania.
Jak to niektórzy mówią - lepiej zapobiegać niż leczyć.
Testy jednostkowe wydłużają czas tworzenia oprogramowania, jednak mogą zaoowocować w przyszłości, wykrywając błędy w zmianach
wprowadzonych w kodzie modułu, do którego test został napisany.
Niejednokrotnie zdarza się nawet że niektóre błędy wykrywane są już podczas jego tworzenia.

Moduł *MSTest* odpowiedzialny za tworzenie testów jednostkowych jest stosunkowo prosty w użytkowaniu.
Wiele przykładów jego użycia można znaleźć w folderze z testami jednostkowymi dla biblioteki.
Do rozpoczęcia, oczywiście będzie potrzebny kod, który ma być testowany.
Poniżej podany zostanie pełny przykład wykorzytania modułu do tworzenia testów jednostkowym.

Testowany plik::

    int f_add( int a, int b )
    {
        return a + b;
    }
    int f_sub( int a, int b )
    {
        return b - a;
    }
    int f_mul( int a, int b )
    {
        return a * b;
    }

Aby przetestować powyższy kod, należy najpierw utworzyć funkcje testujące dla niego.
Nie ma żadnych reguł dotyczących tego, jak dobry test ma wyglądać, jednak tworzenie testu dla każdej funkcji
jest dobrym rozwiązaniem.
Dobry test, to test, który próbuje wywołać funkcję na wszystkie możliwe sposoby.
Oczywiście nie zawsze jest to możliwe.

Przykład funkcji testujących::

    /* test dla dodawania */
    int f_test_add( MST_FUNCTION *info )
    {
        struct EXTRADATA *data = (struct EXTRADATA*)info->Data;
        mst_prepare( info );

        data->result = f_add( data->a, data->b );
        mst_assert_sint( data->result, ==, 5 );

        return MSET_OK;
    }
    /* test dla odejmowania */
    int f_test_sub( MST_FUNCTION *info )
    {
        struct EXTRADATA *data = (struct EXTRADATA*)info->Data;
        mst_prepare( info );

        data->result = f_sub( data->a, data->b );
        mst_assert_sint( data->result, ==, 1 );

        return MSET_OK;
    }
    /* test dla mnożenia */
    int f_test_mul( MST_FUNCTION *info )
    {
        struct EXTRADATA *data = (struct EXTRADATA*)info->Data;
        mst_prepare( info );

        data->result = f_mul( data->a, data->b );
        mst_assert_sint( data->result, ==, 6 );

        return MSET_OK;
    }

Utworzenie funkcji testujących napisany kod, jest już większą połową sukcesu.
Teraz wystarczy zainicjalizować struktury, odpowiedzialne za testy.
W tym momencie można wybrać jedno z dwóch rozwiązań - albo wywoływać każdy test pojedynczo,
albo wszystkie na raz po przypisaniu ich do struktury zestawu.
Poniżej zaprezentowana została druga opcja.
Warto się zastanowić, czy funkcje dostępne w strukturze zbioru, przekazywane do pól :c:member:`MST_SUITE.Setup`
oraz :c:member:`MST_SUITE.TearDown` nie ułatwią wykonywania testów i zarządzania pamięcią.

Pozostała część kodu::

    struct EXTRADATA {
        int a, b, result;
    };

    int f_test_add( MST_FUNCTION *info );
    int f_test_sub( MST_FUNCTION *info );
    int f_test_mul( MST_FUNCTION *info );

    /* funkcja przygotowująca dane */
    int setup_op_function( MST_SUITE *info )
    {
        struct EXTRADATA *data;

        if( info->Data )
            return MSEC_INVALID_ARGUMENT;

        info->Data = malloc( sizeof(struct EXTRADATA) );
        data = (struct EXTRADATA*)info->Data;

        data->result = 0;
        data->a      = 3;
        data->b      = 2;

        return MSEC_OK;
    }

    /* funkcja zwalniająca pamięć po danych */
    void teardown_op_function( MST_SUITE *info )
    {
        free( info->Data );
    }

    /* funkcja główna, uruchamiająca test */
    int main( int argc, char **argv )
    {
        MST_FUNCTION operation_funcs[] = {
            { MST_STRINGIFY(f_test_add), "Add two numbers", NULL },
            { MST_STRINGIFY(f_test_sub), "Substract two numbers", NULL },
            { MST_STRINGIFY(f_test_mul), "Multiply two numbers", NULL },
            { MST_LASTRECORD }
        };

        MST_SUITE operation_suite = {
            "Test add, sub and mul functions",
            FALSE,                    /* BreakOnError */
            setup_op_function,        /* Setup */
            teardown_op_function,     /* TearDown */
            NULL,
            operation_funcs
        };

        /* uruchom testy w zbiorze */
        return mst_run_suite( &operation_suite );
    }

Przykładowe wyjście:

.. sourcecode:: none

    ===============================================================================
    Test add, sub and mul functions
    --------------------------------------------------------------------- [001/003]
    [TEST] f_test_add
    [DESC] Add two numbers
    [STAT] SUCCESS! > Passed asserts: 1
    --------------------------------------------------------------------- [002/003]
    [TEST] f_test_sub
    [DESC] Substract two numbers
    [STAT] FAILED! > Passed asserts: 0
    ------
    # Error in test.c on line 90
    # data->result == 1
    # Where: L = -1 and R = 1
    --------------------------------------------------------------------- [003/003]
    [TEST] f_test_mul
    [DESC] Multiply two numbers
    [STAT] SUCCESS! > Passed asserts: 1
    ===============================================================================

Jak można zauważyć, powyższy kod wygenerował błąd w trakcie działania funkcji ``f_test_sub``.
Przyglądając się bliżej funkcji ``f_sub``, można zauważyć, że podczas odejmowania argumenty podane zostały na odwrót.
Testy umieszczone w zestawie nie zostały przerwane podczas wystąpienia błędu, ponieważ pole :c:member:`MST_SUITE.BreakOnError`
zostało ustawione na wartość ``FALSE``.
Testy nie są zależne od siebie, więc ustawianie tego pola na wartość ``TRUE`` jest niepotrzebne.
Oczywiście nie ulega wątpliwości to, że aby błąd został wykryty, test jednostkowy musi być poprawnie napisany.

Struktury
---------------------------------------------------------

.. c:type:: MST_FUNCTION

    Struktura odpowiedzialna za przechowywanie informacji o funkcji testującej daną część kodu.
    Większość pól struktury w głównej mierze wykorzystywana jest w funkcji :c:func:`mst_run_test`, gdzie używane są
    podczas wyświetlania w konsoli informacji o uruchamianym teście.
    Można je również wykorzystać bezpośrednio wewnątrz uruchomionego testu, gdzie cała struktura przekazywana
    jest do argumentu funkcji testującej.
    Pole :c:member:`MST_FUNCTION.PassedAsserts` zwiększane jest automatycznie za każdy razem, gdy wywoływane jest
    makro :c:macro:`mst_assert`.

    Strukturę można inicjalizować w następujący sposób::

        int additional_data = 5;

        MST_FUNCTION test = {
            module_part_test_func,             /* Function */
            "testing_function",                /* Name */
            "Some description of this test.",  /* Desc */
            &additional_data,                  /* Data */
            0                                  /* PassedAsserts */
        };

    Inicjalizację ostatniego pola można jednak pominąć, ponieważ ustawiane jest ono automatycznie podczas uruchamiania testu.
    W przypadku gdy struktura nie będzie zawierać żadnych danych, warto dopisać do niej wartość *NULL*, zamiast
    zostawiać inicjalizacje na pastwę losu kompilatora. Jest to ważna rada w przypadku gdy test uruchamia się z zestawu
    i to właśnie z niego argument ma być podesłany do funkcji.

    .. c:member:: char *Function(MST_FUNCTION *info)

        Funkcja testująca uruchamiana przez funkcję :c:func:`mst_run_test`.
        Powinna udowodnić poprawność testowanego kodu poprzez stosowanie makra :c:macro:`mst_assert`
        dla każdego testowanego wyrażenia.
        Dzięki temu błąd, który wystąpi podczas działania funkcji zostanie odpowiednio odnotowany w konsoli.
        Funkcja jako argument pobiera strukturę informacji o teście, dzięki której można pobrać zapisane w niej dane.
        Wystąpienie błędu w asercji podczas działania funkcji natychmiast przerywa jej działanie, zwracając treść błędu.
        Nazwa parametru funkcji nie może być zmieniana, zawsze musi to być ``info``.

        Przykład prostej funkcji testującej::

            char *test_function( MST_FUNCTION *info )
            {
                int ercode;
                mst_assert( info->Data != NULL );

                ercode = do_something_with_data( info->Data );
                mst_assert( !ercode );

                return MST_SUCCESS;
            }

        .. rst-class:: parameters

        :param info: Struktura zawierająca informacje o przetwarzanym teście.
        :return: Wartość *NULL* lub treść błędu który wystąpił w przetwarzanej asercji.

    .. c:member:: char *Name

        Nazwa testu używana przy wyświetlaniu informacji o uruchomionym teście w kosnoli.
        Nazwa reprezentuje test podczas wyświetlania w konsoli, dzięki czemu zamiast numerków oznaczających indeksy
        testów wyświetlane są przypisane do nich nazwy.
        To pole struktury jest wymagane i nie może być oznaczone wartością *NULL*.

    .. c:member:: char *Desc

        Opis testu używany przy wyświetlaniu informacji o uruchomionym teście w konsoli.
        Opis powinien być krótki i przejrzysty, dzięki czemu może pomóc w zrozumieniu na czym tak właściwie
        polega napisany test, bez zaglądania w kod i odczytywania komentarzy.
        Wartość ta nie jest wymagana i w przypadku braku opisu powinna być oznaczona wartością *NULL*, co
        spowoduje, że opis nie zostanie w ogóle wyświetlony w konsoli.

    .. c:member:: void *Data

        Dodatkowe dane przekazywane do funkcji testującej wraz ze strukturą testu.
        Zmienna nie jest wymagana i w przypadku braku danych powinna być oznaczona wartością *NULL*.
        Podczas wykonywania zestawu testów, wartość ta w przypadku ustawienia wartości *NULL* jest
        zastępowana wartością zmiennej globalnej dla całego zestawu, ustawioną w jego strukturze.

    .. c:member:: size_t PassedAsserts

        Ilość poprawnie wykonanych asercji w podpiętej do struktury funkcji testu.
        Zmienna resetowana jest do wartości 0 podczas startu testu i zwiększana automatycznie
        w trakcie wywoływania makra pozwalającego na sprawdzenie danego wyrażenia.
        Można dzięki temu wypisać ilość wszystkich asercji w funkcji lub tylko tych, które zostały
        wywołane do wystąpienia błędu.
        Wartość tej zmiennej nie powinna być zmieniana manualnie.

        Pole to można pominąć podczas inicjalizacji struktury::

            MST_FUNCTION test = { test_func, "func_name", "desc", NULL };

    .. c:member:: char *ErrorMessage

        Treść błędu generowanego podczas działania testu.
        W trakcie błędu, dla zmiennej przydzielana jest pamięć, potrzebna do przechowania treści wiadomości.
        Zmienna ta jest wypełniana automatycznie przez funkcje raportujące błędy, wywoływane przez asercje.
        Pamięć przydzielona dla tej zmiennej nie jest zwalniana automatycznie.

.. c:type:: MST_SUITE

    Struktura odpowiedzialna za przechowywanie informacji o zestawie, zawierającym funkcje testujące.
    W głównej mierze struktura wykorzystywana jest w funkcji :c:func:`mst_run_suite`.
    Jednym z ważniejszych pól jest pole :c:member:`MST_SUITE.BreakOnError`, gdzie wartość decyduje o tym,
    czy zbiór podczas działania zostanie przerwany po wykryciu błędu.
    Struktura jest przekazywana do funkcji wywoływanych przed rozpoczęciem i zaraz po zakończeniu
    wszystkich dostępnych w tablicy testów.
    Ostatni test w tablicy powinien zawierać wszystkie pola równe wartości *NULL* lub 0.
    Najlepszym sposobem jest inicjalizacja ostatniego rekordu makrem :c:macro:`MST_LASTRECORD`.

    Przykład inicjalizacji struktury::

        /* lista funkcji testujących */
        MST_FUNCTION suite_functions[] = {
            { MST_STRINGIFY(mst_test_01), "Desc_01", NULL },
            { MST_STRINGIFY(mst_test_02), "Desc_02", NULL },
            { MST_STRINGIFY(mst_test_02), "Desc_03", NULL },
            { MST_LASTRECORD }
        };

        int sample_data = 5;

        /* inicjalizacja struktury dla zestawu */
        MST_SUITE suite_tests = {
            "Suite description",      /* Desc */
            TRUE,                     /* BreakOnError */
            setup_test_function,      /* Setup */
            teardown_test_function,   /* TearDown */
            &sample_data,             /* Data */
            suite_functions           /* Tests */
        };

    Powyższy kod utworzy zestaw testów, przerywanych w przypadku wystąpienia błędu.
    Każda struktura testu otrzyma wartość ``sample_data`` w polu odpowiedzialnym za dane.
    Każdy test może być zależny od danych, zmodyfikowanych w poprzednim teście, dlatego
    struktura posiada pole :c:member:`MST_SUITE.BreakOnError` ustawione na wartość ``TRUE``.

    .. c:member:: char *Desc

        Opis zestawu wyświetlany przed uruchomieniem jakiegokolwiek testu.
        Pole to powinno opisywać w skrócie całość zestawu, choć może być jednocześnie jego nazwą.
        Opis wykorzystywany jest w funkcji :c:func:`mst_run_suite`.
        Pole jest wymagane, więc powinno zawierać przynajmniej informację o tym, do czego zestaw się odnosi.

    .. c:member:: bool *BreakOnError

        Informacja o tym, czy zestaw po wykryciu błędu w jednej z funkcji testujących ma przerwać działanie pozostałych.
        Pole to jest bardzo ważne w przypadku, gdy dane wychodzące z jednej funkcji testowej, modyfikowane są w niej
        i przekazywane do drugiej, która operuje na modyfikacjach i nie może bez nich wykonać się prawidłowo.
        Gdy pole zawiera wartość ``TRUE`` i w jednej z funkcji testujących wystąpi bład w asercji, cały zestaw uruchamianych
        testów natychmiast zostaje przerwany, zwracając błąd. W przeciwnym wypadku funkcja nadal zwróci błąd, ale
        dopiero po zakończeniu wykonywania wszystkich znajdujących się w zestawie testów.

    .. c:member:: int Setup(MST_SUITE *info)

        Funkcja wywoływana przed wykonaniem jakiekogolwiek testu.
        Może być potraktowana jako funkcja pozwalająca na przygotowanie danych do testowania.
        Jako argument przyjmuje strukturę z informacjami o zestawie w której znajdują się również przekazane
        dane globalne dla każdego testu.
        W przypadku gdy funkcja zwróci wartość inną niż 0, funkcje testowe nie są wywoływane.
        Pole nie jest wymagane i w przypadku braku funkcji należy podać wartość *NULL*.

        Przykład prostej funkcji::

            int setup_test( MST_SUITE *info )
            {
                if( info->Data )
                    return MSEC_INVALID_ARGUMENT;

                /* przygotuj tablicę do działania */
                info->Data = ms_array_alloc( sizeof(int), 100 );
                return info->Data
                    ? MSEC_OK
                    : MSEC_MEMORY_ALLOCATION;
            }

        .. rst-class:: parameters

        :param info: Wskaźnik na strukturę zawierającą informacje o uruchomionym zestawie.
        :return: Kod błędu który wystąpił podczas działania funkcji lub wartość 0.

    .. c:member:: void TearDown(MST_SUITE *info)

        Funkcja wywoływana po wykonaniu testów zawartych w zestawie.
        Wywołanie tej funkcji występuje nawet gdy w trakcie testów wyktyty zostanie błąd a struktura będzie
        miała ustawione pole :c:member:`MST_SUITE.BreakOnError` na wartość ``TRUE``.
        Główne zastosowanie tej funkcji to zwalnianie pamięci pozostałej po wykonanych testach.
        Pole nie jest wymagane i w przypadku braku funkcji należy podać wartość *NULL*.

        Przykład prostej funkcji::

            struct ARRAYSET {
                MS_ARRAY a1, a2, a3, a4;
            };

            void teardown_test( MST_SUITE *info )
            {
                struct ARRAYSET *aset = (struct ARRAYSET*)info->Data;

                if( !aset )
                    return;

                ms_array_free( &aset->a1 );
                ms_array_free( &aset->a2 );
                ms_array_free( &aset->a3 );
                ms_array_free( &aset->a4 );
            }

        .. rst-class:: parameters

        :param info: Wskaźnik na strukturę zawierającą informacje o uruchomionym zestawie.

    .. c:member:: void *Data

        Dodatkowe dane przekazywane do struktury testu.
        W przypadku gdy struktura testu zawiera swoje własne dane, pole to nie jest wykorzystywane.
        W przeciwnym wypadku wartość pola kopiowana jest do pola :c:member:`MST_FUNCTION.Data`, gdzie
        przekazywana jest wraz ze strukturą do funkcji testującej.
        Pole to nie jest wymagane i powinno być ustawiane na wartość *NULL* w przypadku gdy do funkcji nie mają
        być przekazywane żadne dane.

    .. c:member:: MST_FUNCTION *Tests

        Tablica zawierająca testy do wykonania podczas uruchomienia zestawu.
        Wszystkie testy znajdujące się na liście w tym polu, zczytywane są i wykonywane przez funkcję :c:func:`mst_run_suite`.
        Testy wykonywane są w takiej kolejności w jakiej zostały podane w tablicy, aż do napotkania ostatniego rekordu, który
        musi być zainicjalizowany wartościami *NULL* lub 0 dla każdego pola struktury :c:type:`MST_FUNCTION`.
        Do ułatwienia tego zadania stworzono makro o nazwie :c:macro:`MST_LASTRECORD`.

        Przykład tablicy z testami przekazywanymi do zestawu::

            MST_FUNCTION TestFunctions[] =
            {
                { MST_STRINGIFY(mst_test_01), "Desc_01", NULL },
                { MST_STRINGIFY(mst_test_02), "Desc_02", NULL },
                { MST_STRINGIFY(mst_test_03), "Desc_03", NULL },
                { MST_STRINGIFY(mst_test_04), "Desc_04", NULL },
                { MST_STRINGIFY(mst_test_05), "Desc_05", NULL },
                { MST_STRINGIFY(mst_test_06), "Desc_06", NULL },
                { MST_STRINGIFY(mst_test_07), "Desc_07", NULL },
                { MST_STRINGIFY(mst_test_08), "Desc_08", NULL },
                { MST_STRINGIFY(mst_test_09), "Desc_09", NULL },

                /* ostatni rekord można inicjalizować w ten sposób */
                { MST_LASTRECORD },

                /* lub w ten, choć ostatnie 0 jest zbędne */
                { NULL, NULL, NULL, NULL, 0 }
            };

Funkcje
---------------------------------------------------------

.. c:function:: int mst_run_suite(MST_SUITE *suite)

    Funkcja uruchamia zestaw testów jednostkowych, przypisanych do podanej struktury w postaci tablicy.
    Przed uruchomieniem wyświetlana jest informacja o aktualnie działającym zestawie i wywoływana jest funkcja
    :c:member:`MST_SUITE.Setup`, pozwalająca na przygotowanie danych do testów.
    Następnie wywoływane są po koleji wszystkie testy z tablicy przypisanej do pola :c:member:`MST_SUITE.Tests`.
    W przypadku gdy któryś z nich zwróci błąd a pole :c:member:`MST_SUITE.BreakOnError` będzie ustawione
    na wartość ``TRUE``, pętla wywołująca funkcje testowe zostanie przerwana.
    Po zakończeniu wszystkich dostępnych w tablicy testów lub w przypadku ich przerwania, wywoływana jest funkcja zapisana
    w polu :c:member:`MST_SUITE.TearDown`, pozwalająca na zwolnienie pamięci przydzielonej na dane testowe.
    Kod błędu zwracany przez funkcję nie jest ustalony z racji tego, iż jest on zależny w głównej mierze od funkcji
    :c:func:`mst_run_test`.

    Przykład użycia funkcji::

        ...
        MST_FUNCTION tests[] = {
            { test_func1, "func_name1", "desc1", NULL },
            { test_func2, "func_name2", "desc2", NULL },
            { test_func3, "func_name3", "desc3", NULL }
        };
        MST_SUITE suite = {
            "Suite description",
            TRUE,
            NULL, NULL, NULL,
            tests
        };
        ...
        if( mst_run_suite(&suite) )
            return -1;
        ...

    .. rst-class:: parameters

    :param func: Struktura zawierająca informacje o zestawie funkcji testujących.
    :return: Wartość różna od 0 w przypadku błędu lub 0.

.. c:function:: int mst_run_test(MST_FUNCTION *info, size_t current, size_t count)

    Funkcja uruchamia test jednostkowy przypisany do podanej struktury.
    Przed jego uruchomieniem wyświetla informacje o teście podane w strukturze.
    Funkcja wywoływana jest w głównej mierze wprost z funkcji :c:func:`mst_run_suite`, jednak może
    być wywoływana samodzielnie.
    Na uwagę zasługują ostatnie dwa parametry, reprezentujące numer aktualnego testu i ilość wszystkich testów.
    Liczby te wyświetlane są nad informacjami wypisywanymi ze struktury.
    Można je pominąć, wpisując w ich miejsce wartości 0, co spowoduje pominięcie ich w trakcie wypisywania
    informacji w konsoli.
    Funkcja w przypadku wystąpienia błędu zwróci wartość różną od 0, która będzie reprezentowała ilość
    znaków w zwróconej przez test wiadomości.

    Przykład użycia funkcji::

        ...
        MST_FUNCTION test1 = { test_func1, "func_name1", "desc1", NULL };
        MST_FUNCTION test2 = { test_func2, "func_name2", "desc2", NULL };
        ...
        if( mst_run_test(&test1, 0, 0) )
            return -1;
        if( mst_run_test(&test2, 3, 50) )
            return -1;
        ...

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param current: Aktualny numer testu lub 0.
    :param count: Ilość wszystkich testów lub 0.
    :return: Wartość 0 lub w przypadku błędu ilość wypisanych znaków.

.. c:function:: int mst_report_sint(MST_FUNCTION *info, const char *exp, const char *file, int line, llong a, llong b)

    Tworzenie raportu o błędzie, spowodowanym przez asercje sprawdzanego wyrażenia, składającego się z dwóch liczb całkowitych.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makro :c:macro:`mst_assert_sint`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :param a: Lewa strona wyrażenia, znajdująca się przed operatorem głównym.
    :param b: Prawa strona wyrażenia, znajdująca się po operatorze głównym.
    :return: Kod błędu lub wartość 0.

.. c:function:: int mst_report_uint(MST_FUNCTION *info, const char *exp, const char *file, int line, ullong a, ullong b)

    Tworzenie raportu o błędzie, spowodowanym przez asercje sprawdzanego wyrażenia, składającego się z dwóch liczb naturalnych.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makro :c:macro:`mst_assert_uint`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :param a: Lewa strona wyrażenia, znajdująca się przed operatorem głównym.
    :param b: Prawa strona wyrażenia, znajdująca się po operatorze głównym.
    :return: Kod błędu lub wartość 0.

.. c:function:: int mst_report_float(MST_FUNCTION *info, const char *exp, const char *file, int line, ldouble a, ldouble b)

    Tworzenie raportu o błędzie, spowodowanym przez asercje sprawdzanego wyrażenia, składającego się z dwóch liczb rzeczywistych
    reprezentowanych przez zapis zmiennoprzecinkowy.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makro :c:macro:`mst_assert_float`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.
    Należy pamiętać, że porównywanie liczb zmiennoprzecinkowych nie wygląda tak samo jak porównywanie liczb całkowitych.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :param a: Lewa strona wyrażenia, znajdująca się przed operatorem głównym.
    :param b: Prawa strona wyrażenia, znajdująca się po operatorze głównym.
    :return: Kod błędu lub wartość 0.

.. c:function:: int mst_report(MST_FUNCTION *info, const char *exp, const char *file, int line)

    Tworzenie raportu o błędzie, spowodowanym przez asercje na wyrażeniu, które posiada z prawej i lewej strony typ logiczny.
    Dlatego w raporcie wyświetlane jest tylko podane wyrażenie, bez informacji o wartościach stojących po prawej i lewej stronie.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makro :c:macro:`mst_assert`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :return: Kod błędu lub wartość 0.

.. c:function:: int mst_report_cs(MST_FUNCTION *info, const char *file, int line, const char *a, const char *b)

    Tworzenie raportu o błędzie, spowodowanym przez porównanie dwóch przekazanych ciągów znaków, zawierających znaki
    składające się z jednego lub wielu bajtów.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makra :c:macro:`mst_assert_cs` oraz :c:macro:`mst_asser_mbs`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :param a: Ciąg znaków znajdujący się po lewej stronie.
    :param b: Ciąg znaków znajdujący się po prawej stronie.
    :return: Kod błędu lub wartość 0.

.. c:function:: int mst_report_wcs(MST_FUNCTION *info, const char *file, int line, const wchar_t *a, const wchar_t *b)

    Tworzenie raportu o błędzie, spowodowanym przez porównanie dwóch przekazanych ciągów znaków, zawierających znaki
    o typie ``wchar_t`` o rozmiarze 2 lub 4 bajtów.
    Funkcja w głównej mierze wywoływana jest automatycznie przez makro :c:macro:`mst_assert_wcs`.
    Uruchomienie funkcji powoduje przydzielenie pamięci dla pola :c:member:`MST_FUNCTION.ErrorMessage` i zapisanie
    w nim treści błędu, tworzonego z przekazanych parametrów.
    Przydzieloną pamięć należy zwolnić samodzielnie po wykorzystaniu danych, zapisanych w zmiennej.

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.
    :param exp: Wyrażenie w którym wystąpił błąd.
    :param file: Plik w którym wystąpił błąd.
    :param line: Linia w której wystąpił błąd w pliku.
    :param a: Ciąg znaków znajdujący się po lewej stronie.
    :param b: Ciąg znaków znajdujący się po prawej stronie.
    :return: Kod błędu lub wartość 0.

Makra
---------------------------------------------------------

.. c:macro:: void mst_prepare(MST_FUNCTION *info)

    Przygotowuje strukturę danych w funkcji do użycia w makrach.
    Makro to należy wywołać w funkcji testującej bezpośrednio po deklaracjach wykorzystywanych zmiennych.
    Przy okazji sprawdza, czy struktura przekazana w parametrze jest poprawna.

    Przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            int x, y, z;

            /* makro wywoływane zaraz po deklaracjach */
            mst_prepare( info );

            for( x = 5; y = 1, z = 0; z < 10; ++z )
                mst_assert( x + y == x++ + ++y - 1 );

            return MSEC_OK;
        }

    .. rst-class:: parameters

    :param info: Struktura zawierająca informacje o funkcji testującej.

.. c:macro:: int mst_assert(expression exp)

    Makro sprawdzające wartość wyrażenia podanego w argumencie.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie podane w argumencie
    okaże się prawdziwe.
    Gdy wyrażenie okaże się fałszywe, makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Asercja ta działa dobrze w przypadku sprawdzania poprawności zmiennych o typie logicznym czy przyrównywania
    wartość do *NULL*.
    W pozostałych przypadkach informacje mogą się okazać zbyt mało szczegółowe.
    Makro wywołuje funkcje raportowania :c:func:`mst_report`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            bool istrue = TRUE;

            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert( istrue == TRUE );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert( istrue == FALSE );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0x2A
        Message:
        -----------------
        # Error in test.c on line 14
        # istrue == 0

    .. rst-class:: parameters

    :param exp: Wyrażenie do sprawdzenia.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_sint(expression left, operator compare, expression right)

    Makro sprawdzające wartość wyrażenia formułowanego z podanych argumentów.
    Wersja dla liczb całkowitych, pozwalająca na wyświetlenie wartości lewej i prawej strony wyrażenia.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie okaże się prawdziwe.
    W przeciwnym wypadku makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Makro wywołuje funkcje raportowania :c:func:`mst_report_sint`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert_sint( 2 + 2, ==, 4 );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert_sint( 2 + 2, ==, 5 );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0x42
        Message:
        -----------------
        # Error in test.c on line 11
        # 2 + 2 == 5
        # Where: L = 4 and R = 5

    .. rst-class:: parameters

    :param left: Lewa strona wyrażenia.
    :param compare: Operator, stojący pomiędzy prawą a lewą stroną.
    :param right: Prawa strona wyrażenia.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_uint(expression left, operator compare, expression right)

    Makro sprawdzające wartość wyrażenia formułowanego z podanych argumentów.
    Wersja dla liczb naturalnych, pozwalająca na wyświetlenie wartości lewej i prawej strony wyrażenia.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie okaże się prawdziwe.
    W przeciwnym wypadku makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Makro wywołuje funkcje raportowania :c:func:`mst_report_uint`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            ullong max = ULLONG_MAX;

            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert_uint( max - 3, ==, max - 3 );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert_uint( max - 3, ==, max - 4 );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0x70
        Message:
        -----------------
        # Error in test.c on line 14
        # max - 3 == max - 4
        # Where: L = 18446744073709551612 and R = 18446744073709551611

    .. rst-class:: parameters

    :param left: Lewa strona wyrażenia.
    :param compare: Operator, stojący pomiędzy prawą a lewą stroną.
    :param right: Prawa strona wyrażenia.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_float(expression left, operator compare, expression right)

    Makro sprawdzające wartość wyrażenia formułowanego z podanych argumentów.
    Wersja dla liczb zmiennoprzecinkowych, pozwalająca na wyświetlenie wartości lewej i prawej strony wyrażenia.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie okaże się prawdziwe.
    W przeciwnym wypadku makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Makro wywołuje funkcje raportowania :c:func:`mst_report_float`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            float fnum = 0.2f;

            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert_float( fnum, ==, 0.2f );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert_float( fnum - 0.1f, ==, 0.2f );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0x59
        Message:
        -----------------
        # Error in main.c on line 14
        # fnum - 0.1f == 0.2f
        # Where: L = 0.100000 and R = 0.200000

    .. rst-class:: parameters

    :param left: Lewa strona wyrażenia.
    :param compare: Operator, stojący pomiędzy prawą a lewą stroną.
    :param right: Prawa strona wyrażenia.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_cs(const char *left, const char *right)

    Makro pozwalające na porównanie dwóch ciągów znaków, zawierających znaki jednobajtowe.
    Ciągi porównywane są funkcją ``strcmp``.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie okaże się prawdziwe.
    W przeciwnym wypadku makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Makro wywołuje funkcje raportowania :c:func:`mst_report_cs`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            const char *str = "The quick brown fox jumps over the lazy dog.";
            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert_cs(
                str,
                "The quick brown fox jumps over the lazy dog."
            );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert_cs(
                str,
                "The quick brown fox jumps over the lazy cat."
            );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0xA7
        Message:
        -----------------
        # Error in main.c on line 19
        # Function strcmp( L, R ) failed... 
        # L = The quick brown fox jumps over the lazy dog.
        # R = The quick brown fox jumps over the lazy cat.

    .. rst-class:: parameters

    :param left: Ciąg znaków po lewej stronie.
    :param right: Ciąg znaków po prawej stronie.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_mbs(const char *left, const char *right)

    Makro pozwalające na porównanie dwóch ciągów znaków, zawierających znaki jedno lub kilku bajtowe.
    Na chwilę obecną makro to jest tylko aliasem makra o nazwie :c:macro:`mst_assert_cs`.
    Warto jednak używać go do sprawdzania ciągów wielobajtowych znaków.

    .. rst-class:: parameters

    :param left: Ciąg znaków po lewej stronie.
    :param right: Ciąg znaków po prawej stronie.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: int mst_assert_wcs(const wchar_t *left, const wchar_t *right)

    Makro pozwalające na porównanie dwóch ciągów znaków, zawierających znaki o typie ``wchar_t``.
    Ciągi porównywane są funkcją ``wcscmp``.
    Makro zwiększa wartość pola :c:member:`MST_FUNCTION.PassedAsserts` w przypadku gdy wyrażenie okaże się prawdziwe.
    W przeciwnym wypadku makro natychmiast zakończy funkcję w której zostało wywołane.
    Wywoływana funkcja raportowania powinna zwracać ilość znaków zapisanych w zmiennej :c:member:`MST_FUNCTION.ErrorMessage`,
    dlatego funkcja w której makro jest wywoływane, musi zwracać typ ``int``.
    Makro wywołuje funkcje raportowania :c:func:`mst_report_wcs`, automatycznie uzupełniając potrzebne argumenty.

    Pełny przykład użycia makra::

        int test_function( MST_FUNCTION *info )
        {
            const wchar_t *str =
                L"Чушь: гид вёз кэб цапф, юный жмот съел хрящ.";
            mst_prepare( info );

            /* ta asercja będzie w porządku */
            mst_assert_wcs(
                str,
                L"Чушь: гид вёз кэб цапф, юный жмот съел хрящ."
            );

            setlocale( LC_ALL, "" );

            /* ta już nie, funkcja powinna zwrócić błąd */
            mst_assert_wcs(
                str,
                L"Чушь: гид вёз кэб цапф, жмот съел хрящ."
            );

            return MSEC_OK;
        }

        int main( int argc, char **argv )
        {
            MST_FUNCTION tfunc = { MST_LASTRECORD };
            int ercode = test_function( &tfunc );

            printf( "Error code: 0x%X\nMessage:\n-----------------\n%s\n",
                ercode, tfunc.ErrorMessage );

            return 0;
        }

    Przykładowe wyjście:

    .. sourcecode:: none

        Error code: 0xE0
        Message:
        -----------------
        # Error in main.c on line 23
        # Function wcscmp( L, R ) failed... 
        # L = Чушь: гид вёз кэб цапф, юный жмот съел хрящ.
        # R = Чушь: гид вёз кэб цапф, жмот съел хрящ.

    .. rst-class:: parameters

    :param left: Ciąg znaków po lewej stronie.
    :param right: Ciąg znaków po prawej stronie.
    :return: Kod błędu zwracany przez funkcję raportu lub nic.

.. c:macro:: part MST_STRINGIFY(literal func)

    Makro wstawiające w miejsce wystąpienia wskazanie na funkcję oraz jej nazwę w postaci ciągu znaków.
    Makro jest częścią inicjalizacji struktury :c:type:`MST_FUNCTION` i tylko wtedy powinno być stosowane.
    Jego użycie upraszcza nieco kod w przypadku funkcji o długich nazwach.

    Przykład użycia makra::

        /* ta inicjalizacja */
        MST_FUNCTION test1 = { MST_STRINGIFY(some_test_function), "desc", NULL };

        /* jest równoznaczna z tą */
        MST_FUNCTION test2 = { some_test_function, "some_test_function", "desc", NULL };

    .. rst-class:: parameters

    :param func: Nazwa funkcji zamieniana na ciąg znaków.
    :return: Dwa pierwsze pola podczas inicjalizacji struktury.

.. c:macro:: MST_LASTRECORD

    Makro pozwalające na szybką inicjalizację ostatniego rekordu testu dla struktury zestawu.
    Wystarczy użyć tego makra, zamiast wpisywać dla każdego pola wartość *NULL*.
    Makro przeznaczone tylko do inicjalizacji struktury :c:type:`MST_FUNCTION` wewnątrz :c:type:`MST_SUITE`.

    Przykład użycia makra::

        /* ta inicjalizacja */
        MST_FUNCTION test1 = { MST_LASTRECORD };

        /* jest równoznaczna z tą */
        MST_FUNCTION test2 = { NULL, NULL, NULL, NULL, 0, NULL };

Pokrycie kodu
---------------------------------------------------------

Pokrycie kodu z punktu widzenia tworzenia testów jednostkowych jest jedną z ważniejszych kwestii, które należy poruszyć.
Tworzenie statystyk pokrycia kodu, pozwala na wykrycie miejsc, do których program nie zdołał dojść w trakcie działania.
Metoda ta jest więc stosowana do wykrywania zarówno starego kodu, który od dawna już za nic nie odpowiada, zajmując
niepotrzebnie miejsce w plikach, jak i również miejsc, do których napisany test jednostkowy nie zdołał dojść.
Dzięki temu można dopisać kod uwzlgędniający przypadek, który przechodzi przez daną część kodu testowanego.
Do tworzenia informacji o pokryciu kodu potrzebne jest specjalne narzędzie, przeznaczone do tego celu.
Różne systemy oferują różne narzędzia, które są uzależnione od użytego kompilatora.

.. _GCOV:

GCOV
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Systemy z rodziny *Unix* (którym jest de facto *Linux*) udostępniają narzędzie ``gcov``, działające wraz z kompilatorem ``gcc``.
Aby całość mogła współgrać ze sobą, program należy skompilować wraz z odpowiednimi parametrami, które umożliwią dokładne
wykrycie linii, które są aktywowane podczas działania programu.

Poniższy wycinek kodu przedstawia przykład kompilacje testu w ``gcc``:

.. sourcecode:: bash

    gcc "source.c" "test_source.c" -o "program.out"
        -g                      # włącza debugowanie
        -O0                     # wyłącza optymalizację kodu przez kompilator
        -Wall                   # włącza raportowanie wszystkich błędów
        -fprofile-arcs          # dodaje możliwość analizy kodu
        -ftest-coverage         # tworzy pliki potrzebne przez gcov

Jak można zauważyć, kompilator, otrzymując odpowiednie argumenty, pozwala przygotować kompilowany program do użycia ``gcov``.
Po poprawnej kompilacji dla każdego pliku źródłowego utworzone zostaną pliki o rozszerzeniu *.gcda*, które będą potrzebne
podczas uruchamiania narzędzia ``gcov``:

.. sourcecode:: bash

    ./program.out
    gcov "source.c" "test_source.c"

Przed uruchomieniem narzędzia ``gcov``, należy uruchomić skompilowany przed chwilą program.
Spowoduje to wygenerowanie danych, które ``gcov`` będzie w stanie przetworzyć i wygenerować odpowiedni raport.
Uruchomienie polecenia ``gcov`` wymaga podania plików dla których statystyki będą utworzone.
Po uruchomieniu narzędzia w konsoli wyświetlą się informacje o pokryciu kodu:

.. sourcecode:: none

    Checking code coverage...
    File 'source.c'
    Lines executed:95.14% of 247
    Creating 'array.c.gcov'

    File 'test_source.c'
    Lines executed:99.80% of 494
    Creating 'array_test.c.gcov'

Dane przedstawiają procent wykonania kodu w pliku.
Plik wygenerowany przez narzędzie o rozszerzeniu *.gcov*  zawiera informacje o przebiegu programu.
Precyzując, można tam znaleźć statystyki na temat tego, ile razy dana linia programu została wykonana, oraz które
z linii nie zostały wykonane w uruchomionej instancji aplikacji.
Przykład zawartości pliku prosto ze źródeł testowych modułu tablicy dynamicznej:

.. sourcecode:: none

        -:  312:    /* sprawdź czy nowy element się zmieści */
        9:  313:    if( array->Length + size > array->Capacity )
        -:  314:    {
        -:  315:        int ercode;
        3:  316:        if( (ercode = ms_array_realloc_min(array, array->Length + size)) )
    #####:  317:            return ercode;
        -:  318:    }
        6:  319:    else if( !size )
        1:  320:        SETERRNOANDRETURN( MSEC_INVALID_ARGUMENT );

Jak można zauważyć, linie które nie zostały wykonane, oznaczone są kratkami.
Linie zawierające znak minusa to linie nieistotne dla przebiegu programu.
Pozostałe zawierają liczbę, która przedstawia ile razy program przeszedł przed dane miejsce.

.. _VSPerf:

VSPerf
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Program *Microsoft Visual Studio* zawiera narzędzia do generowania statystyk pokrycia kodu.
Tutaj jest jednak haczyk, gdyż choć każda wersja potrafi je generować, to już nie każda potrafi je otworzyć.
Odczytanie wygenerowanych statystyk wiąże się z zainstalowaniem odpowiedniej wersji produktu, posiadającego w swym zbiorze
bibliotekę *Microsoft.VisualStudio.Coverage.Analysis*. Aktualnie, wersje zawierające ten moduł, oznaczone są nazwą
*Enterprise*. Oczywiście w przypadku systemu *Windows* zawsze można użyć metody, opisanej w rozdziale GCOV_
po wcześniejszym zainstalowaniu środowiska *MinGW* lub dużo prostrzej w użyciu metody z rozdziału OpenCppCoverage_.

Zestaw narzędzi ``VSPerf`` został utworzony na potrzeby zbierania danych o wydajności aplikacji. Można dzięki
nim wygenerować również dane dotyczące pokrycia kodu przez program. Aby to zrobić, należy oczywiście wcześniej
odpowiednio skompilować program. Wszystkie funkcje zamieszczone poniżej należy wywoływać w konsoli zawierającej
ustawione ścieżki w zmiennej środowiskowej. Skrypt *Visual Studio Command Prompt* dołączony do menu podczas
instalacji *Visual Studio* ustawia je automatycznie.

Poniżej przedstawiony został przykład kompilacji testu w ``cl``:

.. sourcecode:: bash

    cl "source.c" "test_source.c" /Fe"program.exe"
        /Zi                     # włącza debugowanie
        /Od                     # wyłącza optymalizację kodu przez kompilator
        /Wall                   # włącza raportowanie wszystkich błędów
        /link
        /Profile                # dodaje możliwość analizy kodu
        /OUT:NOREF              # pozostawia funkcje, które nie są w programie

Tak skompilowany program powinien dać się uruchomić. Wcześniej jednak, należy uruchomić narzędzie do instrumentacji
plików binarnych, ``VSInstr``, po czym użyć narzędzia ``VSPerfMon``.
Kolejność wywoływanych poleceń jest następująca:

.. sourcecode:: bash

    VSInstr /coverage "program.exe"
    start VSPerfMon /coverage /output:"program.coverage"
    program.exe
    VSPerfCmd /shutdown

Ta sekwencja instrukcji otwiera nowe okno linii poleceń.
Sytuacja ta jest uciążliwa z punktu widzenia przetwarzania instrukcji sekwencyjnie w plikach skryptowych, gdyż
program wykona się zanim narzędzie zostanie przygotowane do działania.
Aby temu zapobiec, zamiast ``VSPerfMon`` można użyć ``VSPerfCMD``:

.. sourcecode:: bash

    VSPerfCmd /start:coverage /output:"program.coverage"

Uruchomienie powyższej sekwencji z użyciem narzędzia ``VSPerfMon`` lub ``VSPerfCMD`` utworzy plik o rozszerzeniu *.coverage*,
który należy uruchomić w programie *Visual Studio*.
To jest właśnie ten moment w którym do uruchomienia pliku i wyświetlenia wyników należy posiadać odpowiednią wersję aplikacji.

.. _OpenCppCoverage:

OpenCppCoverage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ta darmowa aplikacja konsolowa potrafi w pełni zastąpić narzędzie ``VSPerfMon``, generując statystyki pokrycia
kodu do plików o rozszerzeniu *.html*. Program może również eksportować dane do plików *.xml* w takim układzie
w jakim tworzy je aplikacja *Cobertura*.
Możliwy jest również eksport do plików binarnych, które mogą być później łączone przez program w jeden plik wynikowy
zawierający wszystkie dostępne w nich statystyki.
Tworzenie i łączenie plików binarnych używane jest przez bibliotekę *Moss*, dzięki czemu wszystkie testowane
osobno moduły zostają połączone w jeden plik, zawierający statystyki pokrycia kodu dla całej biblioteki.

Przed użyciem programu z linii poleceń, należy dodać ścieżkę aplikacji do zmiennej środowiskowej *PATH*.
Można to oczywiście ominąć i zamiast samej nazwy programu podawać jego pełną ścieżkę podczas wywoływania.
Program wymaga, aby aplikacja, do której generowane będą statystyki pokrycia kodu, była już skompilowana.
Przykład kompilacji z użyciem kompilatora ``cl`` opisany został w rozdziale VSPerf_.

Aplikacja w trakcie działania wykorzystuje plików *.pdb*, generowane przez kompilator ``cl`` ustawiony na kompilację w trybie odpluskiwania.
Jest to naprawdę dobry odpowiednik narzędzia ``gcov`` dla kompilatora ``cl``.

Program wywołać można w następujący sposób:

.. sourcecode:: bash

    OpenCppCoverage --modules "directory" --sources "directory"
        --export_type html:program    # typ eksportu, tutaj HTML
        -- "program.exe"              # nazwa programu do uruchomienia

Powyższa komenda wpisana w linii poleceń zapisze do folderu o nazwie *program* statystyki pokrycia kodu w formacie *.html*
dla aplikacji o nazwie *program.exe*. Bardzo ważne są tutaj parametry ``--modules`` oraz ``--sources``, które pozwalają
na przetwarzanie tylko plików należących do programu. Dotyczy to zarówno plików *.pdb* jak i plików źródłowych programu.

Przykład łączenia statystyk pokrycia kodu generowanych przez aplikację:

.. sourcecode:: bash

    # pierwszy moduł
    OpenCppCoverage --modules "directory" --sources "directory"
        --export_type binary:module1.bin
        -- "module1.exe"

    # drugi moduł
    OpenCppCoverage --modules "directory" --sources "directory"
        --export_type binary:module2.bin
        -- "module2.exe"

    # łączenie plików binarnych
    OpenCppCoverage
        --input_coverage "module1.bin"
        --input_coverage "module2.bin"
        --export_type html:module

Dzięki temu zabiegowi, program wygeneruje jedną stronę zawierającą statystyki pokrycia kodu dla wszystkich modułów.
Prezentacja pokrycia kodu zapisana w plikach *.html*, przedstawia odpowiednio pokolorowane linie.
Te, które nie zostały wykorzystane w trakcie działania programu oznaczone są na czerwono, pozostałe zaś na zielono.

.. _MemoryLeaks:

Wycieki pamięci
---------------------------------------------------------

Jedną z najważniejszych kwestii w trakcie tworzenia oprogramowania jest wykrycie wszelkich wycieków pamięci.
Tworzą się one w przypadku, gdy ilość zwalnianej pamięci jest mniejsza niż ilość pamięci przydzielanej.
Język *C* jest jednym z języków, w których zwalnianie pamięci nie jest wykonywane automatycznie.
Każda funkcja przydzielająca zasoby musi posiadać odpowiednik, który te zasoby zwalnia.
Wycieki pamięci są jednymi z najgorszych błędów do wykrycia przez programistę, dlatego do ich wyszukiwania
powstały odpowiednie programy.

Valgrind
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Podstawowym narzędziem do sprawdzania wycieków pamięci w systemach z rodziny *Linux*, jest program o nazwie
*Valgrind*, choć oczywiście jest możliwość uruchomienia go na systemie *Windows*.
Przed użyciem narzędzia *Valgrind*, należy skompilować w trybie odpluskiwania program, który będzie sprawdzany.
Kompilację można wykonać zgodnie ze wskazówkami podanymi w rozdziałach GCOV_ oraz VSPerf_.

Uruchomienie narzędzia sprowadza się do podania jego nazwy wraz z nazwą programu:

.. sourcecode:: bash
    
    valgrind --leak-check=yes ./test

Po nazwie programu, w tym wypadku ``./test``, można podać dodatkowo argumenty przekazywane bezpośrednio do programu.
W zasadzie to wszystko, jeżeli chodzi o sprawdzanie wycieków pamięci narzędziem *Valgrind*.
Szczegóły dotyczące uruchamiania programu można znaleźć na stronie producenta.

Zakończenie programu powinien wieńczyć następujący komunikat:

.. sourcecode:: none

    ==3168== HEAP SUMMARY:
    ==3168==     in use at exit: 0 bytes in 0 blocks
    ==3168==   total heap usage: 41 allocs, 41 frees, 10,772 bytes allocated
    ==3168== 
    ==3168== All heap blocks were freed -- no leaks are possible
    ==3168== 
    ==3168== For counts of detected and suppressed errors, rerun with: -v
    ==3168== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Można z niego odczytać, że aplikacja nie ma żadnych wycieków pamięci, więc wszystko jest w porządku.
W przypadku gdyby wycieki pamięci się pojawiły, to w powyższym komunikacie wypisane będą wszystkie błędy,
które *Valgrind* zdołał wychwycić.
Oczywiście możliwe są też fałszywe alarmy *(false positives)*, ale lepiej dmuchać na zimne.

Dr. Memory
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Bardzo rozbudowanym i zarazem prostym w obsłudze narzędziem do wykrywania wycieków pamięci jest *Dr. Memory*,
działający zarówno na systemie *Microsoft Windows* jak i na systemach z rodziny *Linux*.
Aby zaprzęgnąć narzędzie do działania, należy skompilować program w trybie odpluskiwania zgodnie z instrukcjami
podanymi w rozdziałach GCOV_ oraz VSPerf_.
Dzięki temu *Dr. Memory* będzie mógł wykryć potencjalne błędy związane z zarządzaniem pamięcią.

Uruchomienie narzędzie jest banalnie proste:

.. sourcecode:: bash

    drmemory -- test.exe

Gdzie ``test.exe`` to nazwa testowanego programu.
Oczywiście po jego nazwie można podać dodatkowe argumenty, które zostaną przekazane bezpośrednio do niego.
W zasadzie to tyle, jeżeli chodzi o wykrywanie wycieków pamięci narzędziem *Dr. Memory*.
Szczegóły dotyczące uruchamiania programu można znaleźć na stronie producenta.

Zakończenie programu powinno wyglądać mniej więcej tak:

.. sourcecode:: none

    NO ERRORS FOUND:
          0 unique,     0 total unaddressable access(es)
          0 unique,     0 total uninitialized access(es)
          0 unique,     0 total invalid heap argument(s)
          0 unique,     0 total GDI usage error(s)
          0 unique,     0 total handle leak(s)
          0 unique,     0 total warning(s)
          0 unique,     0 total,      0 byte(s) of leak(s)
          0 unique,     0 total,      0 byte(s) of possible leak(s)
    ERRORS IGNORED:
         21 potential error(s) (suspected false positives)
             (details: %APPDATA%\Dr. Memory\DrMemory-array.exe.6304.000\potential_errors.txt)
        119 unique,   239 total,  39196 byte(s) of still-reachable allocation(s)
             (re-run with "-show_reachable" for details)
    Details: %APPDATA%\Dr. Memory\DrMemory-array.exe.6304.000\results.txt

Jak można zauważyć powyżej, program został wykonany poprawnie i nie posiada żadnych wycieków pamięci.
W przypadku gdy podczas działania, program wychwyci jiekolwiek błędy, to zostaną one wypisane
w powyższym komunikacie w konsoli.
Narzędzie *Dr. Memory* wykrywa również fałszywe alarmy, te, należące do bibliotek systemowych
lub używanych w programie bibliotek wstawiane są od razu w sekcję *ERRORS IGNORED*.

Testy biblioteki Moss
---------------------------------------------------------

Biblioteka *Moss* posiada zaprogramowane testy, oparte na module *MSTest*.
Każdy moduł posiada swój test, dzięki czemu można je testować osobno lub wszystkie na raz.
Uruchomienie któregokolwiek z utworzonych testów jest banalnie proste.
W folderze ``tst/run`` znajdują się skrypty odpowiadające za kompilacje testu i uruchomienie narzędzia
pozwalającego na wyświetlenie statystyk pokrycia kodu.
Skrypty utworzone zsotały z myślą o dwóch kompilatorach, ``gcc`` oraz ``cl``.
Dla systemu *Windows* są to pliki o rozszerzeniu *.bat* zaś dla systemu *Linux*, pliki o rozszerzeniu *.sh*.

Aby uruchomić test dla wybranego modułu (tutaj Array), wystarczy wpisać:

.. sourcecode:: bash

    ./array.sh   # dla systemu Linux
    array.bat    # dla systemu Windows

Można również uruchomić wszystkie testy po wpisaniu:

.. sourcecode:: bash

    ./all.sh     # dla systemu Linux
    all.bat      # dla systemu Windows

Po uruchomieniu testu lub wszystkich testów na raz, wyświetlone zostanie pokrycie kodu.
Błąd w jednym teście przerywa wykonywanie pozostałych, zarówno w wybranym zestawie dla modułu
jak i również podczas uruchamiania testów dla wszystkich modułów.
Tak więc w przypadku uruchomienia wszystkich testów, podczas wystąpienia błędu w pierwszym module,
pozostałe nie będą testowane do momentu naprawienia wykrytego błędu.
Testy nie sprawdzają programu pod kątem wycieków pamięci.

Dane generowane przez skrypty zapisywane są do plików w folderze ``tst/gen``.
Po uruchomieniu skryptu to właśnie tam znajdą się pliki zawierające informacje o pokryciu kodu
oraz skompilowane pliki zawierające testy, które można uruchomić samodzielnie.
Dzięki temu każda zmiana w module może być przetestowana pod kątem poprawności z zapisanym
w dokumentacji standardem.
