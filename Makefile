CC = gcc
CFLAGS = -Wall

all: dhat.o test

dhat.o: dhat.c
	$(CC) $(CFLAGS) -o dhat.o -c dhat.c

test: dhat.o main.c
	$(CC) $(CFLAGS) -o test dhat.o main.c
	./test

clean:
	rm -f dhat.o test
