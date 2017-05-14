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
if gcc -g -O0 -Wall \
	-I../../inc \
	$DIR2/array_test.c \
	$DIR2/../src/test.c \
	$DIR2/../src/array.c \
	-D MSD_CCMACRO \
	-lm \
	-o array \
	-fprofile-arcs \
	-ftest-coverage;
then
	echo "Dynamic Array module test compiled successfully!"
	echo "Running test..."

	if ! ./array; then
		exit 2
	fi

	echo "Checking code coverage..."
	gcov array.c array_test.c
else
	echo "Failed to compile Dynamic Array module"
	exit 1
fi
