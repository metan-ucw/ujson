UJSON
=====

UJSON is:

- Complete and simple JSON reader and writer written in C
- Produces nice and human readable error messages
- Passes [JSONTestSuite](https://github.com/nst/JSONTestSuite)

The key point of this parser is that:

- works on a buffer in a memory that is _not_ modified during the parsing
- parser does not allocate memory
- nearly stateless, the only state the parser maintains is an offset to the
  string buffer, recursion depth and error flag
- if error flag is raised all parser functions return immediatelly with error

The parser is started by a call to `ujson_start()` function in order to
determine if first element in the buffer is object or an array.

```c
	switch (ujson_reader_start(reader)) {
	case UJSON_ARR:
		//parse array
	break;
	case UJSON_OBJ:
		//parse object
	break;
	default:
	break;
	}

	if (ujson_reader_err(reader))
		ujson_err_print(reader);

```

Then you can loop recursively over arrays and objects with:

```c
	char sbuf[128];
	struct ujson_val json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

    UJSON_OBJ_FOREACH(reader, &json) {
        switch (json.type) {
		case UJSON_OBJ:
			//parse object
		break;
		case UJSON_ARR:
			//parse array
		break;
                case UJSON_STR:
		...

		}
	}
```


The supported types are:

* UJSON\_ARR - an array, needs to be parsed recursivelly
* UJSON\_OBJ - an object, needs to be parsed recursivelly
* UJSON\_INT - an integer number stored as val\_int (also sets val\_float for convinience)
* UJSON\_FLOAT - a floating point number stored as val\_float
* UJSON\_BOOL - a boolean stored as val\_bool
* UJSON\_NULL - a null has no value
* UJSON\_STR - a string, stored in user supplied buffer
