#ifndef _PARSING_H_
#define _PARSING_H_

void eraseSpace(char *word);
/* getnargs : compte le nombre d'argument dans buff, en se basant sur le nombre d'espaces *
 * Entrée : une chaine de caractères buff
 * Sortie : un entier */
int getnargs(char *buff);

void getargs(char *buff, char **args);

void writehistory(char *buff, char *history_file_path);

void replacechar(char *src, char *dst, char c, char r);

void getpaths(char *path, char **paths);

int getnpaths(char *path);

void getexepaths(char *cmd, char **paths_exe, char **paths, int npaths);

int getnpipes(char *buff);

void getcmds(char *buff, char **cmds);

void removechar(char* str, char c);
#endif
