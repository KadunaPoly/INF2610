/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Init Lab - q1.c
 */


#include <stdio.h>


static inline void print_argc(int argc)
{
	printf("Program received %d argument", argc);
	if (argc > 1)
		printf("s");
	printf(":\n");
}

static inline void print_argv(int argc, char **argv)
{
	for (int i=0; i<argc; ++i)
		printf("%s\n", argv[i]);
}

int main(int argc, char *argv[])
{
	print_argc(argc);
	print_argv(argc, argv);
	return 0;
}
