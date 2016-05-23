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

#define BUFF 1024 
#define PATH_SIZE 1024
#define MAX_ARGS_SIZE  512

int execute(char **args, int nargs, char *history_file_path, char * dir)
{
      
      /* gestion de CD */
      if(!strcmp(args[0], "cd")) {
      
	 char     *home;
	 home = (char*) malloc(BUFF);
	 strcpy(home, getenv("HOME"));


         if(args[1] == NULL || !strcmp(args[1], "~")) { /* gestion du HOME */
            chdir(home);
         }
         else {
            chdir(args[1]);
         }
         getcwd(dir, PATH_SIZE); /* On met à jour le répertoire courant à afficher */
         return 1;
      }
     

      /* fonction exit */
      if(!strcmp(args[0], "exit")) {
         exit(0);
      }

            
      /* gestion de history */
      if(!strcmp(args[0], "history")) {
	 int c = 0;
	 FILE *history_file_d;
	 char     *car;
	 car = (char*) malloc(BUFF);

	 history_file_d = fopen(history_file_path , "r");
	 while(fgets(car, BUFF, history_file_d)) {
	    c++;
	    printf("%4d    %s", c, car);
	 }

	 fclose(history_file_d);
	 return 1;
      }
      
      if(!strcmp(args[0], "copy")) {
	 printf("Copy TD1\n");
	 return 1;
      }
      

      /* On fork et on lance la commande dans le fils */
      int status;   /* statut du processus */
      pid_t pid;     
      pid = fork();
      
      if(pid == 0) 
      {
	  
	 /* fonction cat */
	 if(!strcmp(args[0], "cat")) {
	    cat(args,nargs);
	    return 1;
	 }

	 /* Gestion de touch */
	 if(!strcmp(args[0], "touch")) {
	    touch(args,nargs);
	    return 1;
	 }

	 /* Gestion du PATH */
	 int      npaths, i = 0;
	 char     *path;
	 char     **paths_exe;
	 char     **paths;	  

	 path = (char*) malloc(BUFF);
	 strcpy(path, getenv("PATH"));

	 npaths = getnpaths(path);

	 paths = (char**) malloc((npaths+1)*sizeof(char*));
	 for(i = 0; i < npaths+1; i++) paths[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getpaths(path, paths);

	 paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
	 for(i = 0; i < npaths+1; i++) paths_exe[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getexepaths(args[0], paths_exe, paths, npaths);

	 for(i = 0; i < npaths; i++) {
	    if (execv(paths_exe[i], args)) {
	       }
	 }

	exit(1);
      }
      
      waitpid(pid, &status, 0); 


      return 0;
}
