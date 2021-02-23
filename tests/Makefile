CFLAGS=-W -Wall -O2

all: dump skip
	@./run.sh

dump: dump.o ../ujson.o
skip: skip.o ../ujson.o

clean:
	rm -f dump *.o
