/*
 *  Moss Library >>> http://moss.aculo.pl
 *
 *     /'\_/`\                           
 *    /\      \    ___     ____    ____  
 *    \ \ \__\ \  / __`\  /',__\  /',__\ 
 *     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
 *      \ \_\\ \_\ \____/\/\____/\/\____/
 *       \/_/ \/_/\/___/  \/___/  \/___/ 
 *
 *  Header file for Unit Test (MSTest) module...
 *
 *  This file is part of Moss Library
 *  Copyright (c) by sobiemir <sobiemir@aculo.pl>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MSH_MSTEST__
#define __MSH_MSTEST__

#include <msconf.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <wchar.h>

/* ================================================================================================================== */

/* wstawia do tablicy funkcję i zamienia jej nazwę na ciąg znaków */
#define MST_STRINGIFY(x) x, STRINGIFY(x)

/* inicjalizacja ostatniego rekordu tablicy funkcji testw */
#define MST_LASTRECORD NULL, NULL, NULL, NULL, 0, NULL

/* ================================================================================================================== */

/**
 * Struktura zawierająca informacje o funkcji testowej.
 * Część z tych pól jest obowiązkowa, część opcjonalna, a część uzupełniana automatycznie.
 * Szczegóły dostępne w opisach poszczególnych pól.
 */
typedef struct MSST_FUNCTION
{
	/**
	 * Funkcja testująca.
	 * Uruchamiana przez funkcję mst_run_test.
	 * W trakcie wystąpienia błędu w funkcji, powinna ona zwrócić jego treść.
	 * Pole to jest obowiązkowe.
	 * 
	 * @param  info Struktura funkcji testującej.
	 * @return      Treść błędu lub NULL.
	 */
	int (*Function)(struct MSST_FUNCTION *info);

	/**
	 * Nazwa testu, wyświetlana w konsoli.
	 * Nazwy pozwalają na łatwą identyfikację testu wśród innych.
	 * Pole to jest obowiązkowe.
	 */
	char *Name;

	/**
	 * Opis testu.
	 * Pod nazwą testu wyświetlany jest opis dla szybkiego przypomnienia, co dany test robi.
	 * Jest to dosyć wygodna funkcjonalność, jednak pole to nie jest obowiązkowe.
	 */
	char *Desc; 

	/**
	 * Dane, wykorzystywane przez funkcję.
	 * Generalnie funkcja może z nich korzystać, ale nie musi.
	 * Dostępne są z poziomu struktury przekazywanej jako argument funkcji.
	 * Pole to nie jest obowiązkowe.
	 */
	void *Data;

	/**
	 * Ilość asercji, która została wykonana poprawnie.
	 * W przypadku funkcji, która zakończyła się sukcesem, jest to również ilość wywołań wszystkich
	 * makr z rodziny mst_assert w danej funkcji podczas testowania.
	 * Wartość tego pola jest uzupełniana automatycznie z poziomu funkcji testującej.
	 */
	size_t PassedAsserts;

	/**
	 * Treść wiadomości.
	 * Zmienna ta jest uzupełniana, gdy podczas działania funkcji wystąpi bład.
	 * Pamięć dla tej zmiennej jest przydzielana, więc po wypisaniu błędu należy ją zwolnić,
	 * aby zapobiec wyciekom pamięci.
	 */
	char *ErrorMessage;
}
MST_FUNCTION;

/**
 * Struktura zawierająca informacje o zestawie funkcji testujących dany moduł.
 * Ułatwia grupowanie funkcji, dzięki czemu ich identyfikacja w konsoli jest znacznie przyjemniejsza.
 * Struktura zawiera również wskaźniki na funkcje, które mogą być uruchomione przed uruchomieniem
 * funkcji testujących, aby przygotować dane, oraz zaraz po ich zakończeniu, aby dane usunąć z pamięci.
 */
typedef struct MSST_SUITE
{
	/**
	 * Opis zestawu funkcji.
	 * Wyświetlany jest w konsoli przed uruchomieniem jakiejkolwiek funkcji z zestawu.
	 * Pole jest obowiązkowe.
	 */
	char *Desc;

	/**
	 * Przerwanie serii testów w przypadku błędu.
	 * W przypadku gdy pole to jest ustawione na wartość TRUE i jakakolwiek funkcja z zestawu zwróci w trakcie
	 * działania błąd, przetwarzanie zestawu zostanie przerwane i dalsze funkcje testujące nie będą wywoływane.
	 * Przydaje się to w trakcie, gdy jedna funkcja zależy od drugiej.
	 * Pole jest obowiązkowe.
	 */
	bool BreakOnError;

	/**
	 * Funkcja wywoływana przed rozpoczęciem testów.
	 * Idelna dla przygotowania danych potrzebnych dla testów.
	 * Pole nie jest obowiązkowe.
	 * 
	 * @param info Zbiór, w którym znajduje się funkcja.
	 */
	int (*Setup)(struct MSST_SUITE *info);

	/**
	 * Funkcja wywoływana zaraz po zakończeniu testów.
	 * Idelna dla zwalniania pamięci po danych przydzielonych na potrzeby testu.
	 * Pole nie jest obowiązkowe.
	 * 
	 * @param info Zbiór, w którym znajduje się funkcja.
	 */
	void (*TearDown)(struct MSST_SUITE *info);

	/**
	 * Dane globalne dla zbioru.
	 * Dane są przekazywane do funkcji w przypadku gdy nie ma własnych.
	 * Pole nie jest obowiązkowe.
	 */
	void *Data;

	/**
	 * Lista struktur zawierających informacje o funkcjach testowych.
	 * Ostatni element tej listy powinien mieć wszystkie wartości równe ZERO.
	 * Dla tego celu zostało utworzone makro MST_LASTRECORD, które można podać jako ostatni element tablicy.
	 * Funkcje testowe wywoływane są po kolei.
	 * Pole jest obowiązkowe.
	 */
	MST_FUNCTION *Tests;
}
MST_SUITE;

/* ================================================================================================================== */

/**
 * Przygotowuje funkcję testową do używania asercji.
 * To makro należy wywołać na samym początku, bezpośrednio po deklarajach zmiennych ale przed użyciem
 * jakiejkolwiek funkcji.
 * 
 * @param teststr Struktura z informacjami o funkcji testowej.
 */
#define mst_prepare(teststr) \
	MST_FUNCTION *_TestFunc_ = teststr; \
	assert( teststr ); \
	assert( teststr->PassedAsserts == 0 ); \
	assert( !teststr->ErrorMessage )

/**
 * Sprawdza dwie liczby całkowite używając podanego operatora.
 * Sprawdzane wyrażenie formułowane jest z dwóch argumentów porównywanych podawanym operatorem.
 * Wywołuje makro mst_assert_integer z prefiksem s.
 * W przypadku gdy wyrażenie będzie niepoprawne, wywołana zostanie odpowiednia funkcja raportu.
 * 
 * @param  left    Lewa strona wyrażenia.
 * @param  compare Komparator - operator porównania.
 * @param  right   Prawa strona wyrażenia.
 * @return         Kod błędu lub nic.
 */
#define mst_assert_sint(left, compare, right) \
	/* sprawdź prawą i lewą stronę względem komparatora */ \
	if( left compare right ) \
		_TestFunc_->PassedAsserts++; \
	/* jeżeli się nie zgadzają, zapisz błąd do zmiennej */ \
	else \
		return mst_report_sint( _TestFunc_, EXPRESSIONMAKE(left, compare, right), \
					__FILE__, __LINE__, left, right )

/**
 * Sprawdza dwie liczby naturalne używając podanego operatora.
 * Sprawdzane wyrażenie formułowane jest z dwóch argumentów porównywanych podawanym operatorem.
 * Wywołuje makro mst_assert_integer z prefiksem u.
 * W przypadku gdy wyrażenie będzie niepoprawne, wywołana zostanie odpowiednia funkcja raportu.
 * 
 * @param  left    Lewa strona wyrażenia.
 * @param  compare Komparator - operator porównania.
 * @param  right   Prawa strona wyrażenia.
 * @return         Kod błędu lub nic.
 */
#define mst_assert_uint(left, compare, right) \
	/* sprawdź prawą i lewą stronę względem komparatora */ \
	if( left compare right ) \
		_TestFunc_->PassedAsserts++; \
	/* jeżeli się nie zgadzają, zapisz błąd do zmiennej */ \
	else \
		return mst_report_uint( _TestFunc_, EXPRESSIONMAKE(left, compare, right), \
					__FILE__, __LINE__, left, right )

/**
 * Sprawdza dwie liczby zmiennoprzecinkowe używając podanego operatora.
 * Sprawdzane wyrażenie formułowane jest z dwóch argumentów porównywanych podawanym operatorem.
 * W przypadku gdy wyrażenie będzie niepoprawne, wywołana zostanie odpowiednia funkcja raportu.
 * Makro do poprawy z racji błędnego porównywania liczb zmiennoprzecinkowych.
 * 
 * @param  left    Lewa strona wyrażenia.
 * @param  compare Komparator - operator porównania.
 * @param  right   Prawa strona wyrażenia.
 * @return         Kod błędu lub nic.
 */
#define mst_assert_float(left, compare, right) \
	/* tak się nie powinno porównywać liczb zmiennoprzecinkowych */ \
	if( left compare right ) \
		_TestFunc_->PassedAsserts++; \
	/* jeżeli się nie zgadzają, zapisz błąd do zmiennej */ \
	else \
		return mst_report_float( _TestFunc_, EXPRESSIONMAKE(left, compare, right), \
			__FILE__, __LINE__, left, right )

/**
 * Sprawdza czy wyrażenie jest poprawne.
 * W przypadku gdy nie jest poprawne, wywoływana jest funkcja mst_report, formułująca treść błędu.
 * 
 * @param  exp Wyrażenie do sprawdzenia.
 * @return     Kod błędu lub nic.
 */
#define mst_assert(exp) \
	if( exp ) \
		_TestFunc_->PassedAsserts++; \
	else \
		return mst_report( _TestFunc_, STRINGIFY(exp), __FILE__, __LINE__ )

/**
 * Porównuje dwa ciągi znaków, składających się ze znaków jednobajtowych.
 * W przypadku gdy dwa ciągi nie są takie same, wywoływana jest funkcja mst_report_cs,
 * odpowiedzialna za formowanie treści błędu.
 * 
 * @param  left  Pierwszy ciąg do sprawdzenia.
 * @param  right Drugi ciąg do sprawdzenia.
 * @return       Kod błędu lub nic.
 */
#define mst_assert_cs(left, right) \
	if( strcmp(left, right) == 0 ) \
		_TestFunc_->PassedAsserts++; \
	else \
		return mst_report_cs( _TestFunc_, __FILE__, __LINE__, left, right )

/**
 * Porównuje dwa ciągi znaków, składających się ze znaków wielobajtowych.
 * Na ten moment wywołuje makro mst_assert_cs.
 * 
 * @param  left  Pierwszy ciąg do sprawdzenia.
 * @param  right Drugi ciąg do sprawdzenia.
 * @return       Kod błędu lub nic.
 */
#define mst_assert_mbs(left, right) mst_assert_cs(left, right)

/**
 * Porównuje dwa ciągi znaków, składających się ze znaków o typie wchar_t.
 * W przypadku gdy dwa ciągi nie są takie same, wywoływana jest funkcja mst_report_wcs,
 * odpowiedzialna za formowanie treści błędu.
 * 
 * @param  left  Pierwszy ciąg do sprawdzenia.
 * @param  right Drugi ciąg do sprawdzenia.
 * @return       Kod błędu lub nic.
 */
#define mst_assert_wcs(left, right) \
	if( wcscmp(left, right) == 0 ) \
		_TestFunc_->PassedAsserts++; \
	else \
		return mst_report_wcs( _TestFunc_, __FILE__, __LINE__, left, right )

/* ================================================================================================================== */

/**
 * Funkcja formułuje błąd przy porównywaniu dwóch liczb całkowitych o typie char/short/int/long/llong.
 * Błąd zapisywany jest do zmiennej ErrorMessage w strukturze, podawanej w parametrze info.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert_sint lub mst_assert_integer.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  exp  Sprawdzane wyrażenie w postaci tekstu.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @param  a    Wartość wyrażenia z lewej strony równania.
 * @param  b    Wartość wyrażenia z prawej strony równania.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report_sint( MST_FUNCTION *info, const char *exp, const char *file, int line, llong a, llong b );

/**
 * Funkcja formułuje błąd przy porównywaniu dwóch liczb naturalnych o typie uchar/ushort/uint/luong/ullong.
 * Błąd zapisywany jest do zmiennej ErrorMessage w strukturze, podawanej w parametrze info.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert_uint lub mst_assert_integer.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  exp  Sprawdzane wyrażenie w postaci tekstu.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @param  a    Wartość wyrażenia z lewej strony równania.
 * @param  b    Wartość wyrażenia z prawej strony równania.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report_uint( MST_FUNCTION *info, const char *exp, const char *file, int line, ullong a, ullong b );

/**
 * Funkcja formułuje błąd przy porównywaniu dwóch liczb zmiennoprzecinkowych o typie float/double/ldouble.
 * Błąd zapisywany jest do zmiennej ErrorMessage w strukturze, podawanej w parametrze info.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert_float.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  exp  Sprawdzane wyrażenie w postaci tekstu.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @param  a    Wartość wyrażenia z lewej strony równania.
 * @param  b    Wartość wyrażenia z prawej strony równania.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report_float( MST_FUNCTION *info, const char *exp, const char *file, int line, ldouble a, ldouble b );

/**
 * Funkcja formułuje błąd asercji.
 * Jest to odmiana raportu, gdzie nie są wypisywane wartości prawej i lewej strony.
 * Do błędu, zapisywanego w zmiennej ErrorMessage zapisywane jest tylko wyrażenie.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  exp  Sprawdzane wyrażenie w postaci tekstu.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report( MST_FUNCTION *info, const char *exp, const char *file, int line );

/**
 * Funkcja formułuje błąd przy porównywaniu dwóch ciągów znaków o jendno lub wielobajtowych znakach.
 * Błąd zapisywany jest do zmiennej ErrrorMessage w strukturze, podawanej w parametrze info.
 * W jego treści wypisywane są pod sobą dwa ciągi znaków do porównania.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert_cs oraz mst_assert_mbs.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @param  a    Ciąg znaków z lewej strony równania.
 * @param  b    Ciąg znaków z prawej strony równania.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report_cs( MST_FUNCTION *info, const char *file, int line, const char *a, const char *b );

/**
 * Funkcja formułuje błąd przy porównywaniu dwóch ciągów znaków o typie wchar_t.
 * Błąd zapisywany jest do zmiennej ErrrorMessage w strukturze, podawanej w parametrze info.
 * W jego treści wypisywane są pod sobą dwa ciągi znaków do porównania.
 * Funkcja wywoływana jest automatycznie przez makro mst_assert_wcs.
 * 
 * @param  info Struktura informacyjna z testem jednostkowym.
 * @param  file Nazwa pliku w którym błąd jest raportowany.
 * @param  line Linia błędu.
 * @param  a    Ciąg znaków z lewej strony równania.
 * @param  b    Ciąg znaków z prawej strony równania.
 * @return      Kod błędu lub wartość 0.
 */
int mst_report_wcs( MST_FUNCTION *info, const char *file, int line, const wchar_t *a, const wchar_t *b );

/**
 * Uruchamia podany w argumencie test.
 * Podczas uruchamiania wyświetla komunikaty w konsoli.
 * 
 * @param  info    Struktura zawierająca funkcję testu do uruchomienia.
 * @param  current Indeks aktualnego testu (informacyjnie), 0 gdy brak.
 * @param  count   Ilość wszystkich testów (informacyjnie), 0 gdy brak.
 * @return         Gdy w funkcji wystąpił błąd, zwraca wartość różną od 0.
 */
int mst_run_test( MST_FUNCTION *info, size_t current, size_t count );

/**
 * Uruchamia podany w argumencie zestaw testów.
 * Testy uruchamiane są jeden po drugim, przy czym w przypadku błędu można przerwać je wszystkie.
 * Dane przekazywane do zestawu traktowane są jako dane zapasowe.
 * W przypadku gdy test nie posiada danych, do jego funkcji przekazywane są dane z zestawu.
 * 
 * @param  suite Struktura zawierająca zestaw testów do uruchomienia.
 * @return       Gdy w funkcji wystąpił błąd, zwraca wartość różną od 0.
 */
int mst_run_suite( MST_SUITE *suite );

#endif
