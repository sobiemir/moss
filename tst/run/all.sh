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

# funkcja wyszukująca błędy na wyjściu
function check_retval {
	local retval=$?
	if [ $retval -ne 0 ]; then
		exit 1
	fi
	cd "$@"
}

# wywołuj po koleji wszystkie testy
source array.sh "../gen" ".."
check_retval "../run"
source hash.sh "../gen" ".."
check_retval "../run"

echo ""
echo ""
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo "Checking code coverage for all source files..."
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo ""
echo ""

# w tym momencie program jest w złym folderze, przejdź do poprawnego
cd "../gen"

# sprawdź pokrycie kodu całości
gcov \
	array.c \
	djb.c fnv.c joaat.c murmur.c sdbm.c xxhash.c
