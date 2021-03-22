/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Init Lab - q3.c
 */


#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


static inline void print_msg(int fd, pid_t pid)
{
	char buf[80];
	int len = snprintf(buf, sizeof(buf), "This file has been opened by process ID %d.\n", pid);
	write(fd, buf, len);
}


void question3(void)
{
	static const char fname[] = "q3Output-621531667113.txt";
	int fd = open(fname, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	pid_t pid = getpid();
	print_msg(fd, pid);
	close(fd);
}
