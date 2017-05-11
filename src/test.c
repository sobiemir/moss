#include <mstest.h>

/* ================================================================================================================== */

#define __MST_REPORT_NUMBER_BODY(sign, elemsize) \
	char tpl[] = \
		TERMCOLORMAGNETA("#") " Error in " TERMCOLORYELLOW("%s") " on line " TERMCOLORBLUE("%d") "\n" \
		TERMCOLORMAGNETA("#") " %s\n" \
		TERMCOLORMAGNETA("#") " " TERMCOLORBLUE("Where") ": L = " sign " " TERMCOLORBLUE("and") " R = " sign; \
	/* przydziel miejsce na komunikat o błędzie */ \
	info->ErrorMessage = malloc( sizeof(char) * (sizeof tpl \
		+ strlen(exp)            /* wyrażenie */ \
		+ strlen(file)           /* nazwa pliku */ \
		+ ESTIMATEDINTSIZE(int)  /* linia */ \
		+ elemsize               /* lewa strona */ \
		+ elemsize               /* prawa strona */ \
	) ); \
	/* zwróć ilość znaków zapisanych w zmiennej */ \
	return sprintf( info->ErrorMessage, tpl, file, line, exp, a, b )

/* ================================================================================================================== */

int mst_report( MST_TESTFUNC *info, char *exp, char *file, int line )
{
	char tpl[] =
		TERMCOLORMAGNETA("#") " Error in " TERMCOLORYELLOW("%s") " on line " TERMCOLORBLUE("%d") "\n"
		TERMCOLORMAGNETA("#") " %s";
	/* przydziel miejsce na komunikat o błędzie */
	info->ErrorMessage = malloc( sizeof(char) * (sizeof tpl
		+ strlen(exp)
		+ strlen(file)
		+ ESTIMATEDINTSIZE(int)
	) );
	/* zwróć ilość znaków zapisanych w zmiennej */
	return sprintf( info->ErrorMessage, tpl, file, line, exp );
}

/* ================================================================================================================== */

int mst_report_sint( MST_TESTFUNC *info, char *exp, char *file, int line, int a,  int b )
{
	__MST_REPORT_NUMBER_BODY( "%d", ESTIMATEDINTSIZE(int) );
}

/* ================================================================================================================== */

int mst_report_slong( MST_TESTFUNC *info, char *exp, char *file, int line, long a,  long b )
{
	__MST_REPORT_NUMBER_BODY( "%ld", ESTIMATEDINTSIZE(long) );
}

/* ================================================================================================================== */

int mst_report_sllong( MST_TESTFUNC *info, char *exp, char *file, int line, llong a,  llong b )
{
	__MST_REPORT_NUMBER_BODY( "%lld", ESTIMATEDINTSIZE(llong) );
}

/* ================================================================================================================== */

int mst_report_uint( MST_TESTFUNC *info, char *exp, char *file, int line, uint a, uint b )
{
	__MST_REPORT_NUMBER_BODY( "%u", ESTIMATEDINTSIZE(uint) );
}

/* ================================================================================================================== */

int mst_report_ulong( MST_TESTFUNC *info, char *exp, char *file, int line, ulong a,  ulong b )
{
	__MST_REPORT_NUMBER_BODY( "%lu", ESTIMATEDINTSIZE(ulong) );
}

/* ================================================================================================================== */

int mst_report_ullong( MST_TESTFUNC *info, char *exp, char *file, int line, ullong a,  ullong b )
{
	__MST_REPORT_NUMBER_BODY( "%llu", ESTIMATEDINTSIZE(ullong) );
}

/* ================================================================================================================== */

int mst_report_double( MST_TESTFUNC *info, char *exp, char *file, int line, double a,  double b )
{
	__MST_REPORT_NUMBER_BODY( "%f", 14 );
}

/* ================================================================================================================== */

int mst_report_ldouble( MST_TESTFUNC *info, char *exp, char *file, int line, ldouble a,  ldouble b )
{
	__MST_REPORT_NUMBER_BODY( "%Lf", 24 );
}

/* ================================================================================================================== */

int mst_report_cs( MST_TESTFUNC *info, char *file, int line, const char *a, const char *b )
{
	/* szablon wiadomości */
	char tpl[] =
		TERMCOLORMAGNETA("#") " Error in " TERMCOLORYELLOW("%s") " on line " TERMCOLORBLUE("%d") "\n"
		TERMCOLORMAGNETA("#") " Function " TERMCOLORBLUE("strcmp") "( L, R ) failed for sigle byte string... \n"
		TERMCOLORMAGNETA("#") " L = %s\n"
		TERMCOLORMAGNETA("#") " R = %s";
	/* przydziel miejsce na komunikat o błędzie */
	info->ErrorMessage = malloc( sizeof(char) * (sizeof tpl
		+ strlen(a) + sizeof(TERMCOLORYELLOW("a"))
		+ strlen(b) + sizeof(TERMCOLORYELLOW("a"))
		+ strlen(file)
		+ ESTIMATEDINTSIZE(int)
	) );
	/* zwróć ilość znaków zapisanych w zmiennej */
	return sprintf( info->ErrorMessage, tpl, file, line, a, b );
}

/* ================================================================================================================== */

int mst_report_wcs( MST_TESTFUNC *info, char *file, int line, const wchar_t *a, const wchar_t *b )
{
	/* szablon wiadomości */
	char tpl[] =
		TERMCOLORMAGNETA("#") " Error in " TERMCOLORYELLOW("%s") " on line " TERMCOLORBLUE("%d") "\n"
		TERMCOLORMAGNETA("#") " Function " TERMCOLORBLUE("strcmp") "( L, R ) failed for sigle byte string... \n"
		TERMCOLORMAGNETA("#") " L = %ls\n"
		TERMCOLORMAGNETA("#") " R = %ls";
	/* przydziel miejsce na komunikat o błędzie */
	info->ErrorMessage = malloc( sizeof(char) * (sizeof tpl
		+ wcslen(a) + sizeof(TERMCOLORYELLOW("a"))
		+ wcslen(b) + sizeof(TERMCOLORYELLOW("a"))
		+ strlen(file)
		+ ESTIMATEDINTSIZE(int)
	) );
	/* zwróć ilość znaków zapisanych w zmiennej */
	return sprintf( info->ErrorMessage, tpl, file, line, a, b );
}

/* ================================================================================================================== */

int mst_run_test( MST_TESTFUNC *info, size_t current, size_t count )
{
	int ercode;

	assert( info );
	assert( info->Name );
	assert( info->Function );

	info->PassedAsserts = 0;

	/* nazwa testu i numer */
	if( current == 0 || count == 0 )
		IGRET printf( "-------------------------------------------------------------------------------\n" );
	else
		IGRET printf( "--------------------------------------------------------------------- ["
			TERMCOLORMAGNETA("%03" PFSIZET "/%03" PFSIZET) "]\n", current, count );
	IGRET printf( "[" TERMCOLORCYAN("TEST") "] %s\n", info->Name );

	/* opis testu */
	if( info->Desc )
		IGRET printf( "[" TERMCOLORCYAN("DESC") "] %s\n", info->Desc );

	/* wywołaj funkcję */
	ercode = info->Function( info );

	/* sprawdź czy nie było błędów */
	if( ercode )
		IGRET printf( "[" TERMCOLORCYAN("STAT") "] " TERMCOLORRED("FAILED!") " > Passed asserts: %" PFSIZET "\n",
			info->PassedAsserts );
	else
		IGRET printf( "[" TERMCOLORCYAN("STAT") "] " TERMCOLORGREEN("SUCCESS!") " > Passed asserts: %" PFSIZET "\n",
			info->PassedAsserts );

	/* wyświetl błąd w przypadku gdy funkcja została zakończona niepowodzeniem */
	if( ercode && info->ErrorMessage )
	{
		printf( "------\n" );
		printf( "%s\n", info->ErrorMessage );

		free( info->ErrorMessage );
		info->ErrorMessage = NULL;

		return ercode;
	}

	return MSEC_OK;
}

/* ================================================================================================================== */

int mst_run_suite( MST_TESTSUITE *suite )
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
		if( (error = suite->Setup(suite)) != 0 )
			return error;

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

	return error;
}
