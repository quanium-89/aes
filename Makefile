HDR=aes.h util.h
SRC=aes.c util.c test.c
CLEAN=a.out aes

all: $(HDR) $(SRC)
	gcc $(SRC) -lcrypto -o aes -O3

clean:
	rm -f $(CLEAN)
