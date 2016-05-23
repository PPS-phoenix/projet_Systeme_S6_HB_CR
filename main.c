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
#include "parsing.h"
#include "pipe.h"
#include "execute.h"


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
   char	    ***command;

   char     *home;
   char     *history_file_path;
   int	    *tabArgs;
   int      history_file_p;
   int	    npipe;

   int      i,j;

   
   buff = (char*) malloc(BUFF);
   dir = (char*) malloc(PATH_SIZE);
   host = (char*) malloc(BUFF);
   login = (char*) malloc(BUFF);
   home = (char*) malloc(BUFF);
   history_file_path = (char*) malloc(BUFF);
   tabArgs = (int*) malloc(BUFF*sizeof(int));


   gethostname(host, BUFF);
   strcpy(login, getlogin());
   strcpy(home, getenv("HOME"));

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


      getargs(buff, args);
      args[nargs] = NULL;

      /* Allocation mémoire du tableau commande */
      command = (char***) malloc((nargs)*sizeof(char**));
      for(i=0 ; i < nargs ;  i++)
      {
	 command[i] = (char**) malloc (BUFF*sizeof(char*));
	 for(j = 0;  j < nargs  ; j++)
	 {
	    command[i][j] = (char*) malloc(BUFF*sizeof(char));
	 }
      }
      
      npipe = getnpipe(buff);
      printf("nbPipe = %d\n",npipe);
      getcommand(buff,command,tabArgs);
      
      i=0;
      while(i < npipe +1)
      {
	 printf("NB_ARGS __ tabArgsMain[%d] : %d\n", i, tabArgs[i]);
	 i++;
      }

      if(npipe > 0){
	 execPipe(command, npipe, tabArgs, history_file_path, dir);
	 continue;
      }
      else
      {
	 if(execute(args,nargs,history_file_path, dir)){
	    continue;
	 }
      }


      /* On libère l'espace occupé par les arguments et le buffer */
      for(i = 0; i < nargs+1; i++) free(args[i]);
      free(args);
      free(buff);
      buff = (char*) malloc (BUFF);
   }
   return -1;
}
