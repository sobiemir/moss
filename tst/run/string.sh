#
##  Moss Library >>> http://moss.aculo.pl
##
##     /'\_/`\                           
##    /\      \    ___     ____    ____  
##    \ \ \__\ \  / __`\  /',__\  /',__\ 
##     \ \ \_/\ \/\ \L\ \/\__, `\/\__, `\
##      \ \_\\ \_\ \____/\/\____/\/\____/
##       \/_/ \/_/\/___/  \/___/  \/___/ 
##
##  This file is part of Moss Library.
##  See LICENSE file for copyright information.
#

DIR1="../gen"
DIR2=".."

# nazwa folderu możliwa do przekazania poprzez argument
if [ $# -gt 0 ]; then
	DIR1=$1
fi
# relatywna ścieżka do folderu w którym znajdują się pliki testowe
if [ $# -gt 1 ]; then
	DIR2=$2
fi

# utwórz folder gdy nie istnieje i przejdź do niego
if [ ! -d $DIR1 ]; then
	mkdir $DIR1
fi
cd $DIR1

# kompiluj moduł i przetestuj go
# należy przetestować wszystkie dostępne funkcje skrótu
if gcc -g -O0 -Wall \
	-I../../inc \
	$DIR2/string_test.c \
	$DIR2/../src/test.c \
	$DIR2/../src/string.c \
	$DIR2/../src/array.c \
	$DIR2/../src/hash/sdbm.c \
	-D MSD_CCMACRO \
	-D MSD_HASH_MBS_FUNCTIONS \
	-D MSD_HASH_WCS_FUNCTIONS \
	-D MSD_HASH_SDBM \
	-D MSD_HASH_SEED=1234 \
	-D MSD_STRING_HASH=sdbm \
	-lm \
	-o string \
	-fprofile-arcs \
	-ftest-coverage;
then
	echo "String module test compiled successfully!"
	echo "Running test..."

	if ! ./string; then
		exit 2
	fi

	echo "Checking code coverage..."
	gcov string.c array.c sdbm.c string_test.c
else
	echo "Failed to compile String module"
	exit 1
fi
