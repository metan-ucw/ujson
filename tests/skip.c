// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2021 Cyril Hrubis <metan@ucw.cz>
 */

#include <stdio.h>
#include "../ujson.h"

int main(int argc, char *argv[])
{
	struct ujson_buf *buf;

	if (argc != 2) {
		fprintf(stderr, "usage: %s foo.json\n", argv[0]);
		return 1;
	}

	buf = ujson_load(argv[1]);
	if (!buf)
		return 1;

	switch (ujson_start(buf)) {
	case UJSON_ARR:
		ujson_arr_skip(buf);
	break;
	case UJSON_OBJ:
		ujson_obj_skip(buf);
	break;
	default:
	break;
	}

	if (ujson_is_err(buf)) {
		ujson_err_print(stderr, buf);
		return 1;
	}

	return 0;
}
