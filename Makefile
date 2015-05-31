HDR=aes.h util.h
SRC=aes.c util.c
CLEAN=test a.out *.o *.a

all: lib

lib: $(HDR) $(SRC)
	gcc -c $(SRC) -O3
	ar -cvq libcrypt.a *.o

test: lib
	gcc test.c -lcrypto -L./ -lcrypt -o test

clean:
	rm -f $(CLEAN)
