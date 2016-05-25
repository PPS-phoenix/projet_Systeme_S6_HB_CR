#ifndef _TOUCH_H
#define _TOUCH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <time.h>
#include <utime.h>


/* Fonction : touch
 * Entrées : une commande (chaine d'args) et le nombre d'arguments 
 * Sortie : ---
 * Reproduit la commande touch avec l'option -m
 */

void touch(char **args, int nargs);

#endif
