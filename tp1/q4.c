/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Lab - q4.c
 */


#include <stdlib.h>
#include <stdint.h>

#include "q4/libq4.h"


typedef uint64_t u64;


static const u64 A = 26;
static const u64 B = 4;
static const u64 C = 2477;


u64 question4A(u64 x)
{
	return x * (A * x + B) + C;
}

void question4B(u64 x, u64 *result)
{
	*result = question4A(x);
}

u64 *question4C(u64 x)
{
	u64 *ret = malloc(sizeof(u64));
	if (!ret)
		exit(1);
	question4B(x, ret);
	return ret;
}

void question4D(u64 x, u64 **pp)
{
	u64 *p = question4C(x);
	*pp = p;
}


u64 question4E(u64 x)
{
	u64 ret;
	_question4B(x, &ret);
	return ret;
}
