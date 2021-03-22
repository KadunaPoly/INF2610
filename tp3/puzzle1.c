/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Comm Lab - puzzle1.c
 */

#include "libcommlab.h"

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

static const char token[] = "b1cf201ffa61";

void panic(const char *str)
{
	perror(str);
	exit(1);
}

static const char PIPE1[] = "/tmp/pipe_8f99c269";
static const char PIPE2[] = "/tmp/pipe_d1d81bb6";

void puzzle1()
{
	mkfifo(PIPE1, S_IRUSR | S_IWUSR);
	mkfifo(PIPE2, S_IRUSR | S_IWUSR);

	pid_t pid = fork();

	switch (pid) {
	case -1:
		panic("fork");
		break;

	case 0:
		execl("./puzzle1/exchanger", "exchanger", (char*)NULL);
	}

	int w = open(PIPE1, O_WRONLY);
	int r = open(PIPE2, O_RDONLY);

	kill(pid, SIGUSR2);
	write(w, token, sizeof(token));

	char buf[80];
	ssize_t len;

	while ((len = read(r, buf, sizeof(buf))) > 0) {
		checkExchangerMessage(buf);
	}

	wait(NULL);
	unlink(PIPE1);
	unlink(PIPE2);
}
