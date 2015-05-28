all: aes.c
	gcc aes.c -lcrypto -o aes

clean:
	rm -f a.out aes
