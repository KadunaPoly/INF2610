/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Sync Lab - installer.c
 */


#include "libsynclab.h"
#include "installer/libinstaller.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_THREAD 10

static const char *pkg_names[MAX_THREAD] =
{
	"voompang-8.8",
	"smaf-2.9",
	"tresser-4.11",
	"libkelest-8.12",
	"guft-2.13",
	"hoov-2.15",
	"libdrump-2.16",
	"zufict-8.17",
	"cract-7.13",
	"clod-7.1"
};

static int pkg_sem[MAX_THREAD] =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	1,
	1
};

static int pkg_need[MAX_THREAD][MAX_THREAD] =
{
	{1, 2, 3, 5, 8, -1},
	{4, 5, -1},
	{4, 5, 6, -1},
	{5, 6, 7, -1},
	{7, 9, -1},
	{9, -1},
	{8, 9, -1},
	{-1},
	{-1},
	{-1}
};

static int pkg_children[MAX_THREAD][MAX_THREAD] =
{
	{-1},
	{0, -1},
	{0, -1},
	{0, -1},
	{1, 2, -1},
	{0, 1, 2, 3, -1},
	{2, 3, -1},
	{3, 4, -1},
	{0, 6, -1},
	{4, 5, 6, -1}
};


/*
 * Cette fonction alloue et initialise certains champs de la structure md qui
 * sont utilisés dans le reste des fonctions.
 */
void initializeManagementData(struct management_data *md)
{
	md->downloadSemaphore = malloc(sizeof(sem_t));
	sem_init(md->downloadSemaphore, 0, 4);
	for (int i=0; i<MAX_THREAD; ++i) {
		md->canInstallPackage[i] = malloc(sizeof(sem_t));
		sem_init(md->canInstallPackage[i], 0, pkg_sem[i]);
	}
}

/*
 * Cette fonction nettoie les champs de la structure md qui ont été initialisés
 * par la fonction initializeManagementData.
 */
void cleanupManagementData(struct management_data *md)
{
	sem_destroy(md->downloadSemaphore);
	free(md->downloadSemaphore);
	for (int i=0; i<MAX_THREAD; ++i) {
		sem_destroy(md->canInstallPackage[i]);
		free(md->canInstallPackage[i]);
	}
}

/*
 * Cette fonction télécharge et installe le paquet logiciel dont le numéro est
 * passé en argument.
 */
void installPackage(int packageNum, struct management_data *md)
{
	sem_wait(md->downloadSemaphore);
	doPackageDownload((char*)pkg_names[packageNum], md);
	sem_post(md->downloadSemaphore);
	sem_wait(md->canInstallPackage[packageNum]);
	doPackageInstall((char*)pkg_names[packageNum], md);
}

/*
 * Cette fonction vérifie si le paquet logiciel dont le numéro est passé en
 * argument est prêt pour l'installation. Si c'est le cas, la fonction débloque
 * le paquet pour que son installation puisse se lancer.
 *
 * NOTE: Cette fonction vous aidera à clarifier votre code pour la fonction
 * postPackageInstall. Il est fortement recommandée de l'utiliser, mais ce
 * n'est pas obligatoire.
 */
void wakePackage(int wokenPackageNum, struct management_data *md)
{
}

/*
 * Cette fonction est exécutée après l'installation du paquet logiciel dont
 * le numéro est passé en argument. Son rôleF est de marquer le paquet comme
 * installé dans la structure md, et également de débloquer l'installation des
 * paquets logiciels qui pourraient désormais être prêts pour installation.
 *
 * Afin de clarifier votre code, il est conseillé de compléter et d'utiliser la
 * fonction wakePackage définie juste au-dessus.
 *
 * ATTENTION: Cette fonction est testée de manière unitaire par le script de
 * notation. Vous devez vous assurer qu'elle a bien le comportement décrit plus
 * haut et qu'elle ne modifie pas d'autres variables que celles stockées dans
 * la structure md.
 */
void postPackageInstall(int packageNum, struct management_data *md)
{
	md->isPackageInstalled[packageNum] = 1;
	for (int i =0; i<MAX_THREAD; ++i) {
		int *need;
		if (md->isPackageInstalled[i])
			continue;
		need = pkg_need[i];
		while (*need != -1) {
			if (!md->isPackageInstalled[*need])
				goto fail;
			++need;
		}
		sem_post(md->canInstallPackage[i]);
	fail:
		{};
	}
}

/*
 * Cette fonction crée les dix fils d'exécution qui seront chargés de
 * l'installation des paquets logiciels.
 * Chaque fil d'exécution doit exécuter la fonction threadedPackageInstaller
 * en lui passant comme argument:
 * - le numéro du paquet logiciel à installer (compris entre 0 et 9)
 * - le pointeur md vers la structure passée en argument de la fonction
 *   installer
 */
struct arg {
	int num;
	void *md;
};

static void do_cleanup(void *_arg)
{
	struct arg *arg = _arg;
	struct management_data *md = arg->md;
	int *children = pkg_children[arg->num];
	doPackageCleanup(arg->num, arg->md);
	while (*children != -1) {
		pthread_cancel(md->tids[*children]);
		++children;
	}
	free(arg);
}

static void *thread_routine(void *_arg)
{
	struct arg *arg = _arg;
	void *ret;
	pthread_cleanup_push(do_cleanup, _arg);
	ret = threadedPackageInstaller(arg->num, arg->md);
	free(arg);
	pthread_cleanup_pop(0);
	return ret;
}

void installer(struct management_data *md)
{
	initializeManagementData(md);
	installerTestInit(md);
	for (int i=0; i<MAX_THREAD; ++i) {
		struct arg *arg = malloc(sizeof(struct arg));
		arg->num = i;
		arg->md = md;
		pthread_create(&md->tids[i], NULL, thread_routine, arg);
	}
	for (int i=0; i<MAX_THREAD; ++i)
		pthread_join(md->tids[i], NULL);

	cleanupManagementData(md);
}
