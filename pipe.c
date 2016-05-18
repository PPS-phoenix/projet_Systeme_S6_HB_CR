#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include <fcntl.h>

int  getcommand(char **args, char ***command){
   
   int i=0, j=0, npipe=0, k=0;

   while(args[i] != NULL)
   {
      if(args[i] == "|")
      {
	 i++;
	 j++;
	 npipe++;
	 k=0;

      }
      strcpy(command[j][k],args[i]);
      k++;
      i++;
   }

   return npipe;
}

void pipe(){

   int pipes[npipe], j=1;
   pipe(pipes);
   if(npipe > 2)
   {
      for(i=1 ; i < pipe ; i++)  
      {
	 pipe(pipes+ (i*2)); 
      }  
   }

   /* premiere commande, avant le premier pipe*/
   if((dup2(pipes[i], 1)) == -1)
   {
      perror("dup2 failed");
      exit(EXIT_FAILURE);	 
   }
   
   execute(command[0]);
   /* On ferme tous les pipes */
   for(j=0; j < (npipe*2) ; j++)
   {
      close(pipes[i]);
   }

   for(i = 1 ; i < (npipe*2) ; i=i+2)
   { 
	 /* derniere commande */
	 /* on ne fait que lire sur la commande précédente */
	 if(i == npipe-1)
	 {
	    if((dup2(pipes[i-1],0) == -1))
	    {
	       perror("dup2 failed");
	       exit(EXIT_FAILURE);
	    }
	       

	    execute(command[j]);

	    /* On ferme tous les pipes */
	    for(j=0; j < (npipe*2) ; j++)
	    {
	       close(pipes[i]);
	    }
	 }
	 else
	 {
	    /* entre la premiere et dernière commande */
	    /* on lit sur la précédente et écrit sur la suivante */
	    if((dup2(pipes[i-1],O)) == -1)
	    {
	       perror("dup2 failed");
	       exit(EXIT_FAILURE);
	    }


	    if((dup2(pipes[i+2],1)) == -1)
	    {
	       perror("dup2 failed");
	       exit(EXIT_FAILURE);
	    }


	    execute(command[j]);

	    /* On ferme tous les pipes */
	    for(j=0; j < (npipe*2) ; j++)
	    {
	       close(pipes[i]);
	    }
	 }
      j++;
   }
   }
