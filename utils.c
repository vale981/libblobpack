/*
 * utils - misc libubox utility functions
 *
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define foreach_arg(_arg, _addr, _len, _first_addr, _first_len) \
	for (_addr = (_first_addr), _len = (_first_len); \
		_addr; \
		_addr = va_arg(_arg, void **), _len = _addr ? va_arg(_arg, size_t) : 0)

void *__calloc_a(size_t len, ...)
{
	va_list ap, ap1;
	void *ret;
	void **cur_addr;
	size_t cur_len;
	int alloc_len = 0;
	char *ptr;

	va_start(ap, len);

	va_copy(ap1, ap);
	foreach_arg(ap1, cur_addr, cur_len, &ret, len)
		alloc_len += cur_len;
	va_end(ap1);

	ptr = calloc(1, alloc_len);
	alloc_len = 0;
	foreach_arg(ap, cur_addr, cur_len, &ret, len) {
		*cur_addr = &ptr[alloc_len];
		alloc_len += cur_len;
	}
	va_end(ap);

	return ret;
}
