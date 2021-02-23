// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Copyright (C) 2021 Cyril Hrubis <metan@ucw.cz>
 */

#ifndef UJSON_H
#define UJSON_H

#include <stdio.h>

#define UJSON_ERR_MAX 128
#define UJSON_ID_MAX 64

enum ujson_type {
	UJSON_VOID = 0,
	UJSON_INT,
	UJSON_STR,
	UJSON_OBJ,
	UJSON_ARR,
};

struct ujson_buf {
	/** Pointer to a null terminated JSON string */
	char *json;
	/** A length of the JSON string */
	size_t len;
	/** A current offset into the JSON string */
	size_t off;

	char err[UJSON_ERR_MAX];
	char buf[];
};

struct ujson_val {
	enum ujson_type type;

	/** An user supplied buffer and size to store a string values to. */
	char *buf;
	size_t buf_size;

	/** An union to store the parsed value into. */
	union {
		long val_int;
		const char *val_str;
	};

	/** An ID for object values */
	char id[UJSON_ID_MAX];
};

/*
 * @brief Fills the buffer error.
 *
 * Once buffer error is set all parsing functions return immediatelly with type
 * set to UJSON_VOID.
 *
 * @buf An ujson buffer
 * @fmt A printf like format string
 * @... A printf like parameters
 */
void ujson_err(struct ujson_buf *buf, const char *fmt, ...)
               __attribute__((format (printf, 2, 3)));

/*
 * @brief Prints error into a file.
 *
 * The error takes into consideration the current offset in the buffer and
 * prints a few preceding lines along with the exact position of the error.
 *
 * @f A file to print the error to.
 * @buf An ujson buffer.
 */
void ujson_err_print(FILE *f, struct ujson_buf *buf);

/*
 * @brief Returns true if error was encountered.
 *
 * @bfu An ujson buffer.
 * @return True if error was encountered false otherwise.
 */
static inline int ujson_is_err(struct ujson_buf *buf)
{
	return !!buf->err[0];
}

/*
 * @brief Checks is result has valid type.
 *
 * @res An ujson value.
 * @return Zero if result is not valid, non-zero otherwise.
 */
static inline int ujson_valid(struct ujson_val *res)
{
	return !!res->type;
}

/*
 * @brief Returns the type of next element in buffer.
 *
 * @buf An ujson buffer.
 * @return A type of next element in the buffer.
 */
enum ujson_type ujson_next_type(struct ujson_buf *buf);

/*
 * @brief Returns if first element in JSON is object or array.
 *
 * @buf An ujson buffer.
 * @return On success returns UJSON_OBJ or UJSON_ARR. On failure UJSON_VOID.
 */
enum ujson_type ujson_start(struct ujson_buf *buf);

/*
 * @brief Starts parsing of an JSON object.
 *
 * @buf An ujson buffer.
 * @res An ujson result.
 */
int ujson_obj_first(struct ujson_buf *buf, struct ujson_val *res);
int ujson_obj_next(struct ujson_buf *buf, struct ujson_val *res);

#define UJSON_OBJ_FOREACH(buf, res) \
	for (ujson_obj_first(buf, res); ujson_valid(res); ujson_obj_next(buf, res))

/*
 * @brief Skips parsing of an JSON object.
 *
 * @buf An ujson buffer.
 * @return Zero on success, non-zero otherwise.
 */
int ujson_obj_skip(struct ujson_buf *buf) __attribute__ ((warn_unused_result));

int ujson_arr_first(struct ujson_buf *buf, struct ujson_val *res);
int ujson_arr_next(struct ujson_buf *buf, struct ujson_val *res);

#define UJSON_ARR_FOREACH(buf, res) \
	for (ujson_arr_first(buf, res); ujson_valid(res); ujson_arr_next(buf, res))

/*
 * @brief Skips parsing of an JSON array.
 *
 * @buf An ujson buffer.
 * @return Zero on success, non-zero otherwise.
 */
int ujson_arr_skip(struct ujson_buf *buf) __attribute__ ((warn_unused_result));

/*
 * @brief Loads a file into an ujson buffer.
 *
 * @path A path to a file.
 * @return An ujson buffer or NULL in a case of a failure.
 */
struct ujson_buf *ujson_load(const char *path);

/*
 * @brief Frees an ujson buffer.
 *
 * @buf An ujson buffer allcated by ujson_load() function.
 */
void ujson_free(struct ujson_buf *buf);

#endif /* UJSON_H */
