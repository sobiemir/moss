DIR="gen"

cd ../

# utwórz folder gdy nie istnieje i przejdź do niego
if [ ! -d $DIR ]; then
	mkdir $DIR
fi
cd $DIR

# kompiluj moduł i przetestuj go
if gcc -Wall ../array_test.c ../../src/array.c -lm -lcriterion -o array -fprofile-arcs -ftest-coverage; then
	echo "Dynamic Array module test compiled successfully!"

	if ! ./array; then
		exit 2
	fi
	gcov array.c array_test.c
else
    echo "Failed to compile Dynamic Array module"
    exit 1
fi
