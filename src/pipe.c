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

   int pipes[npipe*2], j = 1, i = 0, k;
   pipe(pipes);
   if(npipe > 2)
   {
      for(i=1 ; i < npipe ; i++)  
      {
	 pipe(pipes+ (i*2)); 
      }  
   }
   
      i=1;
      /* premiere commande, avant le premier pipe*/
      if((dup2(pipes[i], 1)) < 0)
      {
	 perror("first dup2 failed");
	 exit(EXIT_FAILURE);	 
      }
   
      execute(command[0],tabArgs[0],history_file_path, dir);

   
      /* On ferme tous les pipes 
      * for(k=0; k < (npipe*2) ; k++)
      * {
      *	  close(pipes[k]);
      * }
      */

      for(i = 1 ; i < (npipe*2) ; i=i+2)
      { 
	    /* derniere commande */
	    /* on ne fait que lire sur la commande précédente */
	    if(i == ((npipe*2)-1))
	    {
	       printf("i du last pipe : %d",i);
	       if((dup2(pipes[i-1],0) == -1))
	       {
		  perror("last dup2 failed");
		  exit(EXIT_FAILURE);
	       }

	       execute(command[j],tabArgs[j],history_file_path, dir);
	    

	       /* On ferme tous les pipes 
	       * for(k=0; k < (npipe*2) ; k++)
	       * {
	       * close(pipes[k]);
	       * }
	       */
	    }
	    else
	    {
	       /* entre la premiere et dernière commande */
	       /* on lit sur la précédente et écrit sur la suivante */
	       if((dup2(pipes[i-1],0)) == -1)
	       {
		  perror("read - je dois pas etre ici pour un pipe dup2 failed");
		  exit(EXIT_FAILURE);
	       }


	       if((dup2(pipes[i+2],1)) == -1)
	       {
		  perror("write - je ne dois pas etre ici dup2 failed");
		  exit(EXIT_FAILURE);
	       }


	       execute(command[j],tabArgs[j],history_file_path, dir);

	       /* On ferme tous les pipes */
	       for(k=0; k < (npipe*2) ; k++)
	       {
		  close(pipes[k]);
	       }
	    }
	 j++;
      }
}
