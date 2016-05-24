#include "pipe.h"

void execPipe(char ***command, int npipe, int *tabArgs, char *history_file_path, char* dir ){

   int pipes[npipe*2];
   int j = 0, i = 0, k = 0;
   int status, stdOut;
   pid_t pid;
   int      npaths;
   char     *path;
   char     **paths_exe;
   char     **paths;	


   stdOut = dup(1);

   pipe(pipes);
   if(npipe > 2)
   {
      for(i=1 ; i < npipe ; i++)  
      {
	 if(pipe(pipes + (i*2)) < 0)
	 {
	    perror("error while piping");
	    exit(EXIT_FAILURE);
	 }
      }  
   }
   
      
   for(i = 0 ; i < npipe+1 ; i=i+1)
   {

      pid=fork();
      if(pid == 0)
      {
	 if(i < npipe)
	 {
	    if(dup2(pipes[(i*2) +1],1) == -1)
	    {
	       perror("read - dup2 failed");
	       exit(EXIT_FAILURE);
	    }
	 }

	 if(i != 0)
	 {
	    if(dup2(pipes[(i*2) -2],0) == -1)
	    {
	       perror("write - dup2 failed");
	       exit(EXIT_FAILURE);
	    }
	 }
	 
	 for(k=0; k < (npipe*2) ; k++)
	 {
	    close(pipes[k]);
	 }
            
      /* Ici on execute la commande */
   
      /* gestion de CD */
      if(!strcmp(command[i][0],"cd")){
         getcwd(dir, PATH_SIZE); /* On met à jour le répertoire courant à afficher */
	 exit(EXIT_SUCCESS);
      }
     

      /* fonction exit */
      else if(!strcmp(command[i][0], "exit")) {
         exit(0);
      }

            
      /* gestion de history */
      else if(!strcmp(command[i][0], "history")) {
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
	 exit(EXIT_SUCCESS);
      }
      
      else if(!strcmp(command[i][0], "copy")) {
	 printf("Copy TD1\n");
	 exit(EXIT_SUCCESS);
      }
      

      /* fonction cat */
      else if(!strcmp(command[i][0], "cat")) {
	 cat(command[j],tabArgs[j]);
	 exit(EXIT_SUCCESS);
      }

      /* Gestion de touch */
      else if(!strcmp(command[i][0], "touch")) {
	 touch(command[j],tabArgs[j]);
	 exit(EXIT_SUCCESS);
      }

      /* Gestion du PATH */
	   
      else
      {
	 path = (char*) malloc(BUFF);
	 strcpy(path, getenv("PATH"));

	 npaths = getnpaths(path);

	 paths = (char**) malloc((npaths+1)*sizeof(char*));
	 for(k = 0; k < npaths+1; k++) paths[k] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getpaths(path, paths);
	 
	 paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
	 for(k = 0; k < npaths+1; k++) paths_exe[k] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getexepaths(command[i][0], paths_exe, paths, npaths);

	 for(k = 0; k < npaths; k++) {
	    if (execv(paths_exe[k], command[i])) {
	    }
	 }

	 free(path);
	 for(k = 0; k < npaths+1; k++) free(paths[k]);
	 free(paths);


	 for(k = 0; k < npaths+1; k++) free(paths_exe[k]);
	 free(paths_exe);

	 exit(EXIT_SUCCESS);
      }
      }
      else if(pid < 0)
      {
	 printf("\nErreur process.\n");
	 exit(EXIT_FAILURE);
      }

   }

      

   /* On ferme tous les pipes */
   for(k=0; k < (npipe*2) ; k++)
   {
      close(pipes[k]);
   }

   for(k = 0; k < npipe +1 ; k++)
   {
      wait(&status);
   }


   dup2(stdOut,1);
   close(stdOut);
}
