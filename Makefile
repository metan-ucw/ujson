CFLAGS=-Wextra -Wall -O2 -I.
CSOURCES=ujson_reader.c ujson_writer.c ujson_common.c ujson_utf.c
OBJS=$(CSOURCES:.c=.o)
LIB=libujson.a

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

test: $(LIB)
	cd tests && make

clean:
	rm -rf *.o $(LIB) docs
