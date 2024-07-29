CFLAGS=-W -Wall -O2 -I../
LDLIBS=-lujson
LDFLAGS=-L../

all: dump skip filter
	@./run.sh

dump: dump.o
skip: skip.o
filter: filter.o

clean:
	rm -f dump skip filter *.o
