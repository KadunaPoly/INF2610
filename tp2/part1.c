/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * part1.c
 */


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void part1()
{
	char chung[] = "part1/chung";
	char hui[] = "part1/hui";
	char *argv[] = {NULL, "-processId", NULL, (char*)NULL};
	char buf[80];
	char *file = hui;
	pid_t ppid = getpid();
	pid_t cpid = fork();
	pid_t pid = cpid;

	switch (cpid) {
	case -1:
		perror("fork");
		exit(1);
		break;
	case 0:
		file = chung;
		pid = ppid;
		break;
	default:
		/* Empty */
	}
	snprintf(buf, sizeof(buf), "%d", pid);
	argv[0] = file;
	argv[2] = buf;
	execvp(file, (char **const)argv);
}
