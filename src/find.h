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

/* find et find . : affiche tout ce qui est contenu dans le répertoire courant 
 * find fichier : affiche le fichier si il est dans le répertoire courante
 * find dossier : affiche tout ce qui est dans le dossier 
 * aucune option n'est traitee
 */

void find(char *path, char *parent);



#endif

