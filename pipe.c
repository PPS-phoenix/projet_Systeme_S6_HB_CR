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

#include "pipe.h"
#include "execute.h"
#include "parsing.h"


void execPipe(char ***command, int npipe, int *tabArgs, char *history_file_path, char* dir ){

   int pipes[npipe*2];
   int j = 0, i = 0, k = 0;
   int status;
   pid_t pid;


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
      }
      else if(pid < 0)
      {
	 printf("\nErreur process.\n");
	 exit(EXIT_FAILURE);
      }

      printf("La commande %d est %s et a %d arguments.\n",j,command[j][0],tabArgs[j]);

      /* mettre execute ici */
      if(execute(command[j],tabArgs[j],history_file_path,dir) == 0)
      {
	 printf("Commande non executee.\n");
	 /* exit(EXIT_FAILURE);*/
      }
      
      j++;
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
}
