DIR1="gen"
DIR2="run"

# funkcja wyszukująca błędy na wyjściu
function check_retval {
    local retval=$?
    if [ $retval -ne 0 ]; then
        exit 1
    fi
    cd ../$DIR2
}

# wywołuj po koleji wszystkie testy
source array.sh
check_retval
source hash.sh
check_retval

echo ""
echo ""
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo "Checking code coverage for all source files..."
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
echo ""
echo ""

# w tym momencie program jest w folderze ../run
cd ../$DIR1

# sprawdź pokrycie kodu całości
gcov \
    array.c \
	djb.c fnv.c joaat.c murmur.c sdbm.c xxhash.c
