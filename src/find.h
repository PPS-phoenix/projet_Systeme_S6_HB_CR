#ifndef _FIND_H
#define _FIND_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <time.h>
#include <utime.h>

#define MAX_PATH 1024

/* Fonction : find
 * Entrées : le chemin du répertoire où find doit etre executer
 * Sortie : ---
 * Reproduit find sans arguments, affiche tous les fichiers contenus dans le répertoire
 */

void find(char *path, char *parent);



#endif

