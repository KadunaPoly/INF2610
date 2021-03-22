/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Comm Lab - puzzle3.c
 */


#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "libcommlab.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

extern void panic(const char *str);

void puzzle3()
{
	int fds[2];
	/* ./exc1 2> tmpfile */
	switch (fork()) {
	case -1:
		panic("fork");
	case 0: {


		int tmp = open("./puzzle3/tmpfile", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		dup2(tmp, STDERR_FILENO);
		execl("./puzzle3/exc1", "exc1", (char*)NULL);
		panic("execl");
	}
	}
	wait(NULL);

	/* ./exc2 2>&1 | ./exc3 */
	if (pipe(fds))
		panic("panic");
	switch (fork()) {
	case -1:
		panic("fork");
	case 0:
		dup2(fds[1], STDOUT_FILENO);
		dup2(fds[1], STDERR_FILENO);
		close(fds[0]);
		execl("./puzzle3/exc2", "exc2", (char*)NULL);
		panic("execl");
	}

	switch (fork()) {
	case -1:
		panic("fork");
	case 0:
		dup2(fds[0], STDIN_FILENO);
		close(fds[1]);
		execl("./puzzle3/exc3", "exc3", (char*)NULL);
		panic("execl");
	}
	close(fds[0]);
	close(fds[1]);
	wait(NULL);
	wait(NULL);

	/* ./exc4 < tmpfile 2>&1 */
	switch (fork()) {
	case -1:
		panic("fork");
	case 0: {
		int tmp = open("./puzzle3/tmpfile", O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
		if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0)
			panic("dup2");
		if (dup2(tmp, STDIN_FILENO) < 0)
			panic("dup2");
		execl("./puzzle3/exc4", "exc4", (char*)NULL);
		panic("execl");
	}
	}
	wait(NULL);
}
