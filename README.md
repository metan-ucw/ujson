UJSON
=====

A simple JSON parser written in C.

The key point of this parser is that:

- works on a buffer in a memory that is _not_ modified during the parsing
- parser does not allocate memory
- nearly stateless, the only state the parser maintains is an offset to the
  string buffer and error flag
- if error flag is raised all parser functions return immediatelly with error

The parser is started by a call to `ujson_start()` function in order to
determine if first element in the buffer is object or an array.

```c
	switch (ujson_start(buf)) {
	case UJSON_ARR:
		//parse array
	break;
	case UJSON_OBJ:
		//parse object
	break;
	default:
	break;
	}

	if (ujson_is_err(buf))
		ujson_err_print(stderr, buf);

```

Then you can loop recursively over arrays and objects with:

```c
        char sbuf[128];
        struct ujson json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

        UJSON_OBJ_FOREACH(buf, &json) {
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
