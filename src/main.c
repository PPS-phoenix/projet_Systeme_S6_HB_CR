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
#include <math.h>

#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include "parsing.h"
#include "pipe.h"
#include "find.h"
#include "cp.h"


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
   int      nargs; /* nombre d'aguments */
   char     *dir;    /* dossier courant à afficher */
   char     *buff;   /* buffer */
   char     **args;  /* tableau d'arguments */
   char     *host;   /* nom d'hôte de la machine */
   char     *login;  /* nom d'utilisateur */
   char     *home;
   char     *car;
   char     *history_file_path;
   int      history_file_p;
   int      npipes;
   int      i, j, c;
   int      nhistory;
   int      foundhistory;
   FILE     *history_file_d;

   int      o_stdin;
   int      o_stdout;

   buff = (char*) malloc(BUFF);
   dir = (char*) malloc(PATH_SIZE);
   host = (char*) malloc(BUFF);
   car = (char*) malloc(BUFF);
   login = (char*) malloc(BUFF);
   home = (char*) malloc(BUFF);
   history_file_path = (char*) malloc(BUFF);

   gethostname(host, BUFF);
   strcpy(login, getlogin());
   strcpy(home, getenv("HOME"));

   getcwd(dir, PATH_SIZE);

   strcat(history_file_path, home);
   strcat(history_file_path, "/");
   strcat(history_file_path, HISTORY_FILE);

   while(printf("%s%s%s@%s%s%s%s%s:%s%s%s>%s ", MAGE, login, YELO, RESET, MAGE, host, MAGE, WHIT, CYAN, dir, YELO, WHIT), fgets(buff, BUFF, stdin)) {
      o_stdin = dup(0);
      o_stdout = dup(1);

      if(!strcmp(buff, "\n")) {
         continue;
      }

      /* gestion de l'historique */
      history_file_p = open(history_file_path, O_CREAT, 0600);
      close(history_file_p);
      writehistory(buff, history_file_path);

      buff[strlen(buff)-1] = '\0'; /* On supprime le retour à la ligne du buffer */

      while(buff[0] == ' ') {
         for(i = 0; i < strlen(buff); i++) {
            buff[i] = buff[i+1];
         }
      }

      npipes = getnpipes(buff);

      if(npipes == 0) {
         nhistory = 0;
         foundhistory = 0;
         if(buff[0] == '!') {
            j = 0;
            for(i = strlen(buff)-1; i > 0; i--) {
               nhistory += ((int) buff[i]-'0')*pow(10, j);
               j++;
            }

            history_file_d = fopen(history_file_path , "r");
            c = 0;
            while(fgets(car, BUFF, history_file_d)) {
               c++;
               if(c == nhistory) {
                  strcpy(buff, car);
                  printf("%s\n", car);
                  buff[strlen(buff)-1] = '\0';
                  foundhistory = 1;
                  break;
               }
            }
            if(!foundhistory) {
               printf("-shell: !%d: event not found\n", nhistory);
               continue;
            }
         }
         nargs = getnargs(buff); /* On compte le nombre d'arguments dans la commande entrée */
         args = (char**) malloc((nargs+1)*sizeof(char*)); /* On alloue l'espace pour le tableau d'arguments et on les récupère */
         for(i = 0; i < nargs+1; i++) args[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));

         getargs(buff, args);
         args[nargs] = NULL;

         if(!strcmp(args[0], "cd")) {
            strcpy(home, getenv("HOME"));
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

         execPipe(buff, npipes, history_file_path, dir);
      }
      else {
         execPipe(buff, npipes, history_file_path, dir);
      }

      dup2(o_stdin, 0);
      close(o_stdin);
      dup2(o_stdout, 1);
      close(o_stdout);
   }
   return -1;
}
