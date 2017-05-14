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
	$DIR2/hash_test.c \
	$DIR2/../src/test.c \
	$DIR2/../src/hash/djb.c \
	$DIR2/../src/hash/fnv.c \
	$DIR2/../src/hash/joaat.c \
	$DIR2/../src/hash/murmur.c \
	$DIR2/../src/hash/sdbm.c \
	$DIR2/../src/hash/xxhash.c \
	-D MSD_CCMACRO \
	-D MSD_HASH_MBS_FUNCTIONS \
	-D MSD_HASH_WCS_FUNCTIONS \
	-D MSD_HASH_MURMUR \
	-D MSD_HASH_JOAAT \
	-D MSD_HASH_FNV \
	-D MSD_HASH_SDBM \
	-D MSD_HASH_DJB \
	-D MSD_HASH_XXHASH \
	-D MSD_HASH_SEED=1234 \
	-o hash \
	-fprofile-arcs \
	-ftest-coverage;
then
	echo "Hash module test compiled successfully!"
	echo "Running test..."

	if ! ./hash; then
		exit 2
	fi

	echo "Checking code coverage..."
	gcov djb.c fnv.c joaat.c murmur.c sdbm.c xxhash.c hash_test.c
else
	echo "Failed to compile Hash module"
	exit 1
fi
