// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2024 Cyril Hrubis <metan@ucw.cz>
 */

#include <stdio.h>
#include <ujson.h>

int main(int argc, char *argv[])
{
	struct ujson_reader *reader;

	if (argc != 2) {
		fprintf(stderr, "usage: %s foo.json\n", argv[0]);
		return 1;
	}

	reader = ujson_reader_load(argv[1]);
	if (!reader)
		return 1;

	switch (ujson_reader_start(reader)) {
	case UJSON_ARR:
		ujson_arr_skip(reader);
	break;
	case UJSON_OBJ:
		ujson_obj_skip(reader);
	break;
	default:
	break;
	}

	ujson_reader_finish(reader);

	return 0;
}
