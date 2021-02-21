// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2021 Cyril Hrubis <metan@ucw.cz>
 */

#include <stdio.h>
#include "../ujson.h"

static void do_padd(unsigned int padd)
{
	while (padd-- > 0)
		putchar(' ');
}

static void dump_arr(struct ujson_buf *buf, unsigned int padd, const char *id);

static void dump_obj(struct ujson_buf *buf, unsigned int padd, const char *id)
{
	char sbuf[128];
	struct ujson_val json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

	do_padd(padd);
	if (id)
		printf("%s: {\n", id);
	else
		printf("{\n");

	UJSON_OBJ_FOREACH(buf, &json) {
		switch(json.type) {
		case UJSON_ARR:
			dump_arr(buf, padd + 1, json.id);
		break;
		case UJSON_OBJ:
			dump_obj(buf, padd + 1, json.id);
		break;
		case UJSON_INT:
			do_padd(padd + 1);
			printf("%s: %li\n", json.id, json.val_int);
		break;
		case UJSON_STR:
			do_padd(padd + 1);
			printf("%s: %s\n", json.id, json.val_str);
		break;
		case UJSON_VOID:
		break;
		}
	}

	do_padd(padd);
	printf("}\n");
}

static void dump_arr(struct ujson_buf *buf, unsigned int padd, const char *id)
{
	char sbuf[128];
	struct ujson_val json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

	do_padd(padd);
	if (id)
		printf("%s: [\n", id);
	else
		printf("[\n");

	UJSON_ARR_FOREACH(buf, &json) {
		switch(json.type) {
		case UJSON_ARR:
			dump_arr(buf, padd + 1, NULL);
		break;
		case UJSON_OBJ:
			dump_obj(buf, padd + 1, NULL);
		break;
		case UJSON_INT:
			do_padd(padd + 1);
			printf("%li\n", json.val_int);
		break;
		case UJSON_STR:
			do_padd(padd + 1);
			printf("%s\n", json.val_str);
		break;
		case UJSON_VOID:
		break;
		}
	}

	do_padd(padd);
	printf("]\n");
}

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
		dump_arr(buf, 0, NULL);
	break;
	case UJSON_OBJ:
		dump_obj(buf, 0, NULL);
	break;
	default:
	break;
	}

	if (ujson_is_err(buf))
		ujson_err_print(stderr, buf);

	return 0;
}
