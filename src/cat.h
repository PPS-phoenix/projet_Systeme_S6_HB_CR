#ifndef _CAT_H
#define _CAT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#define BUFF 1024

/* Fonction : cat 
 * Entrees : une chaine d'arguments,  nombre d'arguments 
 * Sortie : aucune
 * 
 * Execute la commande cat, gère l'option -n
 */

void cat(char **args, int nargs);

#endif
