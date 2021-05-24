CFLAGS=-W -Wall -O2

all: dump skip filter
	@./run.sh

dump: dump.o ../ujson.o
skip: skip.o ../ujson.o
filter: filter.o ../ujson.o

clean:
	rm -f dump *.o
