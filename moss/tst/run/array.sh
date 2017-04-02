DIR="gen"

cd ../

# utwórz folder gdy nie istnieje i przejdź do niego
if [ ! -d $DIR ]; then
	mkdir $DIR
fi
cd gen

# kompiluj moduł i przetestuj go
if gcc -Wall ../array_test.c ../../src/array.c -lm -lcriterion -o array -fprofile-arcs -ftest-coverage; then
	echo "Dynamic Array module test compiled successfully!"

	./array
	gcov array.c array_test.c
else
    echo "Failed to compile Dynamic Array module"
    exit
fi
