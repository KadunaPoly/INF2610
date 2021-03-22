/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Clone Lab - part2.c
 */


#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "libclonelab.h"

#if 0
#define registerProc r
void r(int level, int num, int pid, int ppid)
{
	printf("%d.%d %d %d\n", level, num, pid, ppid);
}
#endif

FILE *fp;

static void split(int offset, int num)
{
	char buf[1024];
	char *argv[] = {buf, (char*)NULL};
	for (int i=0; i<num; ++i) {
		pid_t pid = fork();
		if (pid == 0) {
			registerProc(2, i+offset+1, getpid(), getppid());
			snprintf(buf, sizeof(buf), "part2/level2.%d", i+offset+1);
			switch (i+offset+1) {
			case 1:
			case 4:
			case 5:
				fprintf(fp, "level2.%d)\n", i+offset+1);
				fflush(NULL);
				break;
			case 6:
				putenv("NO_FORK_TOKEN=9d37beaee069d48411f11297");
				break;
			}
			execvp(argv[0], argv);
		}
		wait(NULL);
	}
}

extern char **environ;

void part2()
{
	fp = fopen("part2Output.txt", "a");
	char msg_fmt[] = "Root process has pid %d (message from process ";
	char *token = "bc6c065267f1bc54f30d7228";
 	char lvl00[] = "part2/level0";
	char lvl11[] = "part2/level1.1";
	char lvl12[] = "part2/level1.2";
	char *argv[] = {NULL, NULL, (char*)NULL};
	char buf[1024];
	pid_t p00 = getpid();
	pid_t p11;
	pid_t p12;
	snprintf(buf, sizeof(buf), "%d", p00);
	fprintf(fp, msg_fmt, p00, environ[0]);
	argv[1] = buf;

	p11 = fork();
	if (p11) {
		wait(NULL);
	}
	else {
		split(0, 4);
		registerProc(1, 1, getpid(), getppid());
		argv[0] = lvl11;
		fprintf(fp, "level1.1)\n");
		fflush(NULL);
		execvp(argv[0], argv);
	}


	p12 = fork();
	if (p12) {
		wait(NULL);
	}
	else {
		split(4, 3);
		registerProc(1, 2, getpid(), getppid());
		argv[0] = lvl12;
		fprintf(fp, "level1.2)\n");
		fflush(NULL);
		execvp(argv[0], argv);
	}
	argv[0] = lvl00;
	argv[1] = token;
	fprintf(fp, "level0)\n");
	fflush(NULL);
	execvp(argv[0], argv);
}
