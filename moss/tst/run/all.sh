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
