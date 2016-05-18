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
   dup2(pipes[i], 1);
   execute(command[0]);
   close(pipes[i]);

   for(i = 1 ; i < (npipe*2) ; i=i+2)
   { 
	 /* dernier pipe / derniere commande */
	 /* read only */
	 if(i == npipe-1)
	 {
	    dup2(pipes[i-1],0); 
	    execute(command[j]);
	    close(pipes[i-1]);
	 }
	 else
	 {
	    /* entre la premiere et dernière commande */
	    /* read and write */
	    dup2(pipes[i-1],O);
	    dup2(pipes[i+2],1);
	    execute(command[j]);
	    close(pipes[i-1]);
	    close(pipes[i+2]);
	 }
      j++;
   }

}
