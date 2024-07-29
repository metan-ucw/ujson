// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2021-2024 Cyril Hrubis <metan@ucw.cz>
 */

#include <stdio.h>
#include "ujson.h"

static void do_padd(unsigned int padd)
{
	while (padd-- > 0)
		putchar(' ');
}

static void dump_arr(struct ujson_reader *reader, unsigned int padd, const char *id);

static void dump_obj(struct ujson_reader *reader, unsigned int padd, const char *id)
{
	char sbuf[128];
	struct ujson_val json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

	do_padd(padd);
	if (id)
		printf("%s: {\n", id);
	else
		printf("{\n");

	UJSON_OBJ_FOREACH(reader, &json) {
		switch(json.type) {
		case UJSON_ARR:
			dump_arr(reader, padd + 1, json.id);
		break;
		case UJSON_OBJ:
			dump_obj(reader, padd + 1, json.id);
		break;
		case UJSON_INT:
			do_padd(padd + 1);
			printf("%s: %lli\n", json.id, json.val_int);
		break;
		case UJSON_FLOAT:
			do_padd(padd + 1);
			printf("%s: %f\n", json.id, json.val_float);
		break;
		case UJSON_BOOL:
			do_padd(padd + 1);
			printf("%s: %s\n", json.id, json.val_bool ? "true" : "false");
		break;
		case UJSON_NULL:
			do_padd(padd + 1);
			printf("%s: null\n", json.id);
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

static void dump_arr(struct ujson_reader *reader, unsigned int padd, const char *id)
{
	char sbuf[128];
	struct ujson_val json = {.buf = sbuf, .buf_size = sizeof(sbuf)};

	do_padd(padd);
	if (id)
		printf("%s: [\n", id);
	else
		printf("[\n");

	UJSON_ARR_FOREACH(reader, &json) {
		switch(json.type) {
		case UJSON_ARR:
			dump_arr(reader, padd + 1, NULL);
		break;
		case UJSON_OBJ:
			dump_obj(reader, padd + 1, NULL);
		break;
		case UJSON_INT:
			do_padd(padd + 1);
			printf("%lli\n", json.val_int);
		break;
		case UJSON_FLOAT:
			do_padd(padd + 1);
			printf("%f\n", json.val_float);
		break;
		case UJSON_BOOL:
			do_padd(padd + 1);
			printf("%s\n", json.val_bool ? "true" : "false");
		break;
		case UJSON_NULL:
			do_padd(padd + 1);
			printf("null\n");
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
		dump_arr(reader, 0, NULL);
	break;
	case UJSON_OBJ:
		dump_obj(reader, 0, NULL);
	break;
	default:
	break;
	}

	ujson_reader_finish(reader);

	return 0;
}
