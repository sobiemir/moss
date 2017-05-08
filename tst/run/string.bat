::
::  Moss Library >>> http://moss.aculo.pl
::
::     /'\_/`\                           
::    /\      \    ___     ____    ____  
::    \ \ \__\ \  / __`\  /',__\  /',__\ 
::     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
::      \ \_\\ \_\ \____/\/\____/\/\____/
::       \/_/ \/_/\/___/  \/___/  \/___/ 
::
::  This file is part of Moss Library.
::  See LICENSE file for copyright information.
::

@ECHO OFF

SET dir1=../gen
SET dir2=..

:: metoda tworzenia statystyki pokrycia kodu
:: 1 - VSPerfMon
:: 2 - OpenCppCoverage - html
:: 3 - OpenCppCoverage - cobertura
:: 4 - OpenCppCoverage - binary
SET method=1

:: ścieżka do skryptu
SET scriptdir=%~dp0

:: sprawdź ile argumentów zostało przekazanych do programu
SET argc=0
FOR %%x IN (%*) DO (
	SET /A argc += 1
)

:: nazwa folderu możliwa do przekazania poprzez argument
IF /I "%argc%" GEQ "1" (
	SET dir1=%~1
)
:: relatywna ścieżka do folderu w którym znajdują się pliki testowe
IF /I "%argc%" GEQ "2" (
	SET dir2=%~2
)

:: metoda tworzenia pokrycia kodu
IF /I "%argc%" GEQ "3" (
	SET method=%~3
)

:: utwórz folder gdy nie istnieje i przejdź do niego
IF NOT EXIST "%dir1%" (
	mkdir "%dir1%"
)
cd "%dir1%"

:: kompiluj test
cl /Zi /Od /W4^
	/I../../inc^
	"%dir2%/string_test.c"^
	"%dir2%/../src/string.c"^
	"%dir2%/../src/array.c"^
	"%dir2%/../src/hash/sdbm.c"^
	/DMSD_CCMACRO^
	/DMSD_HASH_MBS_FUNCTIONS^
	/DMSD_HASH_WCS_FUNCTIONS^
	/DMSD_HASH_SDBM^
	/DMSD_HASH_SEED=1234^
	/DMSD_STRING_HASH=sdbm^
	/DMSD_NOTERMCOLOR^
	/Fe"string.exe"^
	/link^
	/Profile^
	/OPT:NOREF

IF %ERRORLEVEL% NEQ 0 (
	cd "%scriptdir%"
	EXIT /B %ERRORLEVEL%
)

:: uruchom sekwencje odpowiedzialną za pokrycie kodu (VSPerf)
IF /I "%method%" EQU "1" (
	VSInstr /coverage string.exe
	VSPerfCmd /start:coverage /output:"string.coverage"
	string.exe
	IF %ERRORLEVEL% NEQ 0 (
		cd "%scriptdir%"
		EXIT /B %ERRORLEVEL%
	)
	VSPerfCmd /shutdown
)
:: OpenCppCoverage - html
IF /I "%method%" EQU "2" (
	OpenCppCoverage --modules "moss" --sources "moss"^
		--export_type html:string -- string.exe
	IF %ERRORLEVEL% NEQ 0 (
		cd "%scriptdir%"
		EXIT /B %ERRORLEVEL%
	)
)
:: OpenCppCoverage - cobertura
IF /I "%method%" EQU "3" (
	OpenCppCoverage --modules "moss" --sources "moss"^
		--export_type cobertura:string.ser -- string.exe
	IF %ERRORLEVEL% NEQ 0 (
		cd "%scriptdir%"
		EXIT /B %ERRORLEVEL%
	)
)
:: OpenCppCoverage - binary
IF /I "%method%" EQU "4" (
	OpenCppCoverage --modules "moss" --sources "moss"^
		--export_type binary:string.covbin -- string.exe
	IF %ERRORLEVEL% NEQ 0 (
		cd "%scriptdir%"
		EXIT /B %ERRORLEVEL%
	)
)

:: skrypt musi powrócić do wcześniejszego folderu
cd "%scriptdir%"
EXIT /B 0
