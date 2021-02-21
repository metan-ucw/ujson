CFLAGS=-W -Wall -O2

all: ujson.o

test: ujson.o
	cd tests && make

clean:
	rm -f *.o
