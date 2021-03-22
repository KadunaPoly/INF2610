/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Init Lab - q2.c
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "q2.h"


/*
 * Vous devez imprimer le message indiqué dans l'énoncé:
 * - En exécutant un premier appel à printf AVANT l'appel à write
 * - Sans utiliser la fonction fflush
 * - En terminant chaque ligne par le caractère '\n' de fin de ligne
 */
void question2(void)
{
	static const char write_str[] = "df6ac4f7c02e (printed using write)\n";
	static const char printf_str[]= "df6ac4f7c02e (printed using printf)";
	printf("%s", printf_str);
	write(STDOUT_FILENO, write_str, strlen(write_str));
	printf("\n");
}
