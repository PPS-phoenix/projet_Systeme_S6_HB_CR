#ifndef _CP_h
#define _CP_h
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define SIZE 32
#define SIZE_CHAR 1024

/*
* Entrée : nom du fichier source, nom de la destination
* Sortie : entier pour le contrôle d'erreur
* Copie un fichier
*/
int copy(char source[],char dest[]);

/*
* Entrée : nom du fichier source, nom de la destination
* Sortie : entier de contrôle d'erreur : 1 == SUCCES
* Permet de copier les permissions des fichiers
*/
int copyPermissions(char source[],char dest[]);

/*
* Entrée : un char
* Sortie : Précise ce qu'a fait la fonction 1 == un '/' est ajouté, 0 == aucun '/' ajouté
* Permet de rajouter un '/' à la fin d'une chaine si il n'y est pas
* 0 n'est pas un échec
*/
int checkString(char *str);

/*
* Entrée : nom du fichier source, nom de la destination
* Copie quelle que soit l'entrée (fichier ou répertoire)
*/
void copyAll(char source[],char dest[]);

#endif
