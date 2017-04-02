DIR="gen"

cd ../

# utwórz folder gdy nie istnieje i przejdź do niego
if [ ! -d $DIR ]; then
	mkdir $DIR
fi
cd $DIR

# kompiluj moduł i przetestuj go
if gcc \
    -Wall \
    ../hash_test.c \
    ../../src/hash/djb.c \
    ../../src/hash/fnv.c \
    ../../src/hash/joaat.c \
    ../../src/hash/murmur.c \
    ../../src/hash/sdbm.c \
    ../../src/hash/xxhash.c \
    -D MSD_HASH_SEED=1234 \
    -lcriterion \
    -o hash \
    -fprofile-arcs \
    -ftest-coverage;
then
	echo "Hash module test compiled successfully!"

	if ! ./hash; then
        exit 2
    fi
	gcov djb.c fnv.c joaat.c murmur.c sdbm.c xxhash.c hash_test.c
else
    echo "Failed to compile Hash module"
    exit 1
fi
