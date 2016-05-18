#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <time.h>
#include <utime.h>
#include "touch.h"



int touch(char **args, int nargs)
{
	 int checkM = 0, file = 0, i = 0, j = 0;
	 struct   stat infos;
	 struct   utimbuf buf;


	 /* on vérifie qu'il y a des arguments */
	 if(nargs == 1)
	 {
	    printf("manque d'arguments \n");
	    return -1;
	 }

	 for( i=1 ; i < nargs ; i++)
	 {
	    if( strcmp(args[i],"-m") == 0 )
	    {
	       /* option "-m" détectée */
	       checkM = 1;
	    }
	 }
	 
	 if(checkM == 1)
	 {
	 /* option "-m" detectée */
	       for (j=1 ; j < nargs ; j++)
	       {
		  if(strcmp( args[j] , "-m" ) != 0 )
		  {
		     /* Création des fichiers si ils n'existent pas */
		     file = open(args[j], O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR);
		     if(file != -1)
		     {
			printf("Le fichier %s a ete ouvert.\n",args[j]);
		     }
		     close(file);


		     if( stat( args[j], &infos) != 0)
			return -1;

		     /* on récupère le temps courant */
		     buf.modtime = time(NULL);
		     buf.actime = infos.st_atime;
		     utime(args[j] , &buf);
		  }
	       }
	       return 1;
	 }
	 else
	 {
	    for (j=1 ; j < nargs ; j++) 
	    {
	       /* Création des fichiers si ils n'existent pas */
	       file = open(args[j], O_WRONLY | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR);
	       if(file != -1)
	       {
		  printf("Le fichier %s a ete cree.\n",args[j]);
	       }
	       close(file);

	       
	       /* on récupère le temps courant */
	       buf.modtime = time(NULL);
	       buf.actime = time(NULL);
	       utime(args[j] , &buf);
	    }
	    return 1;
	 }
}
