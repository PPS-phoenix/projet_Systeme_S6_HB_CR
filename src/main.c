#define _GNU_SOURCE
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
#include "touch.h"
#include "cat.h"
#include "parsing.h"



#if defined(__APPLE__)
#define HOME "/Users/"
#elif defined(__unix__)
#define HOME "/home/"
#endif

#define HISTORY_FILE    ".tpshell_history"

#define BUFF            1024
#define PATH_SIZE       1024
#define MAX_ARGS_SIZE   512
#define CYAN            "\033[1m\033[36m"
#define YELO            "\033[1m\033[33m"
#define WHIT            "\x1B[0m"
#define MAGE            "\x1B[35m"
#define RESET           "\033[0m"

#define true            1
#define false           0



int main(int argc, char *argv[]) {
   pid_t    pid;     /* pid du processus en cours */
   int      status;  /* statut du processus */
   int      nargs;   /* nombre d'aguments */
   int      npaths;
   char     *dir;    /* dossier courant à afficher */
   char     *buff;   /* buffer */
   char     **args;  /* tableau d'arguments */
   char     *host;   /* nom d'hôte de la machine */
   char     *login;  /* nom d'utilisateur */
   char     *path;
   char     **paths_exe;
   char     **paths;
   char     *home;
   char     *history_file_path;
   int      history_file_p;
   char     *car;
   FILE     *history_file_d;

   int      i, c = 0;

   
   buff = (char*) malloc(BUFF);
   dir = (char*) malloc(PATH_SIZE);
   host = (char*) malloc(BUFF);
   login = (char*) malloc(BUFF);
   path = (char*) malloc(BUFF);
   home = (char*) malloc(BUFF);
   car = (char*) malloc(BUFF);
   history_file_path = (char*) malloc(BUFF);





   gethostname(host, BUFF);
   strcpy(login, getlogin());
   strcpy(home, getenv("HOME"));
   strcpy(path, getenv("PATH"));

   getcwd(dir, PATH_SIZE);

   strcat(history_file_path, home);
   strcat(history_file_path, "/");
   strcat(history_file_path, HISTORY_FILE);

   while(printf("%s%s%s@%s%s%s%s%s:%s%s%s>%s ", MAGE, login, YELO, RESET, MAGE, host, MAGE, WHIT, CYAN, dir, YELO, WHIT), fgets(buff, BUFF, stdin)) {
      /* Si l'utilisateur n'écrit rien et appuie sur Entrée */

      if(!strcmp(buff, "\n")) {
         continue;
      }

      /* gestion de l'historique */
      history_file_p = open(history_file_path, O_CREAT, 0600);
      close(history_file_p);
      writehistory(buff, history_file_path);

      /* On supprime le retour à la ligne du buffer */
      buff[strlen(buff)-1] = '\0';
      /* On compte le nombre d'arguments dans la commande entrée */
      nargs = getnargs(buff);
      /* On alloue l'espace pour le tableau d'arguments et on les récupère */
      args = (char**) malloc((nargs+1)*sizeof(char*));
      for(i = 0; i < nargs+1; i++) args[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      args[1][0] = 0;
      getargs(buff, args);
      args[nargs] = NULL;

      /* gestion de CD */
      if(!strcmp(args[0], "cd")) {
         if(args[1] == NULL || !strcmp(args[1], "~")) { /* gestion du HOME */
            chdir(home);
         }
         else {
            chdir(args[1]);
         }
         getcwd(dir, PATH_SIZE); /* On met à jour le répertoire courant à afficher */
         continue;
      }

      if(!strcmp(args[0], "exit")) {
         exit(0);
      }

      if(!strcmp(args[0], "history")) {
         history_file_d = fopen(history_file_path , "r");
         while(fgets(car, BUFF, history_file_d)) {
            c++;
            printf("%4d    %s", c, car);
         }

         fclose(history_file_d);
         continue;
      }

      /* Gestion de Touch */
      if(!strcmp(args[0], "touch")) {
	 touch(args,nargs);
         continue;
      }


      if(!strcmp(args[0], "cat")) {
	 cat(args,buff,nargs);
         continue;
      }



      if(!strcmp(args[0], "copy")) {
         printf("Copy TD1\n");
         continue;
      }

      /* Gestion du PATH */

      npaths = getnpaths(path);

      paths = (char**) malloc((npaths+1)*sizeof(char*));
      for(i = 0; i < npaths+1; i++) paths[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      getpaths(path, paths);

      paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
      for(i = 0; i < npaths+1; i++) paths_exe[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      getexepaths(args[0], paths_exe, paths, npaths);

      /* On fork et on lance la commande dans le fils */
      pid = fork();
      if(pid == 0) {
         for(i = 0; i < npaths; i++) {
            printf("[%d] Try : %s\n", i, paths_exe[i]);
            if (execv(paths_exe[i], args)) {
               printf("%s: failed. Path or command not found\n", paths_exe[i]);
            }
         }

         exit(0);
      }

      waitpid(pid, &status, 0);

      /* On libère l'esapce occupé par les arguments et le buffer */
      for(i = 0; i < nargs+1; i++) free(args[i]);
      free(args);
      free(buff);
      buff = (char*) malloc (BUFF);
   }
   return -1;
}





