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
 *  Header for Unit Test module...
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
#include <stdio.h>

/* ================================================================================================================== */

/**
 * Alias do wartości NULL.
 * Makro używane w testach w przypadku gdy funkcja nie zwraca żadnej wiadomości,
 * a więc z definicji jest poprawna, gdyż każde makro zostało wywołane poprawnie.
 */
#define MST_SUCCESS NULL

/* wstawia do tablicy funkcję i zamienia jej nazwę na ciąg znaków */
#define MST_TFSTRINGIFY(X) X, STRINGIFY(X)

/* inicjalizacja ostatnich wartości dla tablicy funkcji testów */
#define MST_TFSYSINIT 0, 0

/* inicjalizacja wartości bez danych tablicy funkcji testw */
#define MST_TFNODATA NULL, MST_TFSYSINIT

/* inicjalizacja ostatniego rekordu tablicy funkcji testw */
#define MST_TFLASTRECORD NULL, NULL, NULL, MST_TFNODATA

/* ================================================================================================================== */

/**
 * Struktura zawierająca informacje o funkcji testowej.
 * Część z tych pól jest obowiązkowa, część opcjonalna, a część uzupełniana automatycznie.
 * Szczegóły dostępne w opisach poszczególnych pól.
 */
typedef struct MSST_TESTFUNCT
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
	char *(*Function)(struct MSST_TESTFUNCT *info);

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
	 * Indeks testu w zbiorze.
	 * Wartość tego pola jest uzupełniana automatycznie z poziomu funkcji wywołującej dany zbiór.
	 */
	size_t Index;
}
MST_TESTFUNC;

/**
 * Struktura zawierająca informacje o zestawie funkcji testujących dany moduł.
 * Ułatwia grupowanie funkcji, dzięki czemu ich identyfikacja w konsoli jest znacznie przyjemniejsza.
 * Struktura zawiera również wskaźniki na funkcje, które mogą być uruchomione przed uruchomieniem
 * funkcji testujących, aby przygotować dane, oraz zaraz po ich zakończeniu, aby dane usunąć z pamięci.
 */
typedef struct MSST_TESTSUITE
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
	bool_mst BreakOnError;

	/**
	 * Funkcja wywoływana przed rozpoczęciem testów.
	 * Idelna dla przygotowania danych potrzebnych dla testów.
	 * Pole nie jest obowiązkowe.
	 * 
	 * @param info Zbiór, w którym znajduje się funkcja.
	 */
	void (*Setup)(struct MSST_TESTSUITE *info);

	/**
	 * Funkcja wywoływana zaraz po zakończeniu testów.
	 * Idelna dla zwalniania pamięci po danych przydzielonych na potrzeby testu.
	 * Pole nie jest obowiązkowe.
	 * 
	 * @param info Zbiór, w którym znajduje się funkcja.
	 */
	void (*TearDown)(struct MSST_TESTSUITE *info);

	/**
	 * Dane globalne dla zbioru.
	 * Dane są przekazywane do funkcji w przypadku gdy nie ma własnych.
	 * Pole nie jest obowiązkowe.
	 */
	void *Data;

	/**
	 * Lista struktur zawierających informacje o funkcjach testowych.
	 * Ostatni element tej listy powinien mieć wszystkie wartości równe ZERO.
	 * Dla tego celu zostało utworzone makro MST_TFLASTRECORD, które można podać jako ostatni element tablicy.
	 * Funkcje testowe wywoływane są po kolei.
	 * Pole jest obowiązkowe.
	 */
	MST_TESTFUNC *Tests;
}
MST_TESTSUITE;

/* ================================================================================================================== */

/**
 * Sprawdza czy podane wyrażenie jest prawdą.
 * Jeżeli wyrażenie jest prawdą, zwiększa licznik poprawnych asercji.
 * W przeciwnym wypadku makro wychodzi z funkcji, zwracając treść błędu.
 * Błąd formowany jest względem asercji oraz nazwy pliku i linii w którym wystąpił.
 * 
 * @param  Wyrażenie do sprawdzenia.
 * @return Treść błędu w przypadku gdy wyrażenie jest fałszywe.
 */
#define mst_assert(_Test_) \
	if( _Test_ ) \
		info->PassedAsserts++; \
	else \
		return \
			TERMCOLORMAGNETA("#") " Error in " TERMCOLORYELLOW(__FILE__) " on line " \
				TERMCOLORBLUE(DBLSTRINGIFY(__LINE__)) "\n" \
			TERMCOLORMAGNETA("#") " " STRINGIFY(_Test_)

/**
 * Uruchamia podany w argumencie test.
 * Podczas uruchamiania wyświetla komunikaty w konsoli.
 * 
 * @param  func    Struktura zawierająca funkcję testu do uruchomienia.
 * @param  current Indeks aktualnego testu (informacyjnie), 0 gdy brak.
 * @param  count   Ilość wszystkich testów (informacyjnie), 0 gdy brak.
 * @return         Gdy w funkcji wystąpił błąd, zwraca wartość różną od 0.
 */
INLINE static int mst_run_test( MST_TESTFUNC *func, size_t current, size_t count )
{
	char *message;

	assert( func );
	assert( func->Name );
	assert( func->Function );

	func->Index         = current;
	func->PassedAsserts = 0;

	/* nazwa testu i numer */
	if( current == 0 || count == 0 )
		IGRET printf( "-------------------------------------------------------------------------------" );
	else
		IGRET printf( "--------------------------------------------------------------------- ["
			TERMCOLORMAGNETA("%03" PFSIZET "/%03" PFSIZET) "]\n", current, count );
	IGRET printf( "[" TERMCOLORCYAN("TEST") "] %s\n", func->Name );

	/* opis testu */
	if( func->Desc )
		IGRET printf( "[" TERMCOLORCYAN("DESC") "] %s\n", func->Desc );

	/* wywołaj funkcję */
	message = func->Function( func );

	/* wypisz rezultat */
	if( message )
		IGRET printf( "[" TERMCOLORCYAN("STAT") "] " TERMCOLORRED("FAILED!") " > Passed asserts: %" PFSIZET "\n",
			func->PassedAsserts );
	else
		IGRET printf( "[" TERMCOLORCYAN("STAT") "] " TERMCOLORGREEN("SUCCESS!") " > Passed asserts: %" PFSIZET "\n",
			func->PassedAsserts );

	/* wyświetl błędy w przypadku gdy funkcja została zakończona niepowodzeniem */
	if( message )
		/* tu jest w porządku, printf zawsze coś wypisze więc zawsze coś zwróci
		 * nawet w przypadku błędu zwrócona zostanie wartość ujemna */
		return printf( "------\n%s\n", message );

	return 0;
}

/**
 * Uruchamia podany w argumencie zestaw testów.
 * Testy uruchamiane są jeden po drugim, przy czym w przypadku błędu można przerwać je wszystkie.
 * Dane przekazywane do zestawu traktowane są jako dane zapasowe.
 * W przypadku gdy test nie posiada danych, do jego funkcji przekazywane są dane z zestawu.
 * 
 * @param  suite 
 * @return       Gdy w funkcji wystąpił błąd, zwraca wartość różną od 0.
 */
INLINE static int mst_run_suite( MST_TESTSUITE *suite )
{
	MST_TESTFUNC *tests;
	size_t        ammount = 0;
	size_t        current = 1;
	int           error   = 0;

	assert( suite );
	assert( suite->Tests );
	assert( suite->Desc );

	tests = suite->Tests;

	/* oblicz ilość wszystkich testów */
	while( (tests++)->Function )
		ammount++;

	tests = suite->Tests;

	/* inicjalizacja */
	if( suite->Setup )
		suite->Setup( suite );

	IGRET printf( "===============================================================================\n" );
	IGRET printf( "%s\n", suite->Desc );

	while( tests->Function )
	{
		/* przypisz domyślne dane jeżeli ich nie podano */
		if( tests->Data == NULL )
			tests->Data = suite->Data;

		/* przerwij działanie funkcji w przypadku błędu tylko wtedy, gdy takie zachowanie zostało ustalone */
		if( (error |= mst_run_test(tests++, current++, ammount)) && suite->BreakOnError )
			break;
	}
	IGRET printf( "===============================================================================\n" );

	/* zakończenie */
	if( suite->TearDown )
		suite->TearDown( suite );

	return 0;
}

#endif
