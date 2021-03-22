/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Comm Lab - puzzle2.c
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void panic(const char *str);


static const char msg[] = "token is ca848bd28f5e end";

void send_msg(int fd, const char *str, size_t len)
{
	for (size_t i=0; i<len; ++i) {
		unsigned char c;
		switch (str[i]) {
		case ' ':
			c = 33;
			break;
		case 'a' ... 'z':
			c = 113 + str[i] - 'a';
			break;
		case '0' ... '9':
			c = 148 + str[i] - '0';
			break;
		default:
			c = 0;
		}
		write(fd, &c, 1);
	}
}

void puzzle2()
{
	int fds[2];
	char buf[80];
	if (pipe(fds))
		panic("pipe");

	snprintf(buf, sizeof(buf), "%d", fds[0]);

	switch (fork()) {
	case -1:
		panic("fork");

	case 0:
		close(fds[1]);
		execl("./puzzle2/telegraph", "telegraph", buf, (char*)NULL);
		panic("execl");
	}
	close(fds[0]);
	send_msg(fds[1], msg, sizeof(msg));
	close(fds[1]);
	wait(NULL);
}
