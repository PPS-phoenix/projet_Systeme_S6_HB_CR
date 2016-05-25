#include "find.h"

/* quand c'est . : == c'est la fin */

void find(char * path, char *parent)
{
   DIR *repertoire;
   struct dirent  *fichier = NULL;
   struct stat type;
   char *temp;
   char *tempparent;

   tempparent = malloc(MAX_PATH*sizeof(char));
   

   repertoire = opendir(path);

   if(!repertoire)
   {
      perror("erreur durant l'ouverture du repertoire");
      exit(EXIT_FAILURE);
   }

   while((fichier = readdir(repertoire)) != NULL)
   {
      /* on malloc le temp à la taille du répertoire courant + / + repertoire que l'on vient d'ouvrir */ 
      temp = malloc((strlen(path) + strlen(fichier->d_name) + 2)*sizeof(char));
     
      sprintf(temp,"%s%s%s",path,"/",fichier->d_name);
    
      /* on passe type que l'on vient de créer dans stat() pour récuperer les infos du chemin courant */
      if(stat(temp,&type) < 0)
      {
	 perror("stat failed");
	 exit(EXIT_FAILURE);
      }

      if(strcmp(fichier->d_name,".."))
      {
	 /* Si il s'agit d'un répertoire S_ISDIR, on affiche son nom, et on refait appel à find  */
	 if(S_ISDIR (type.st_mode) && strcmp (fichier -> d_name,"."))
	 {
	    /* Affichage */
	    printf("%s/%s\n",parent,fichier->d_name);

	    /* On concatene dans temp */
	    if( parent[strlen(parent)-1] == '/' )
	    {
	       strcat(tempparent,parent);
	       strcat(tempparent,fichier->d_name);
	    }
	    else
	    {
	       strcat(tempparent,parent);
	       strcat(tempparent,"/");
	       strcat(tempparent,fichier->d_name);
	    }
	 
	    printf("%s\n",tempparent);

	    find(temp,tempparent);

	 }
	 else if(strcmp(fichier -> d_name,"."))
	 {
	    /* il s'agit d'un fichier et on va l'afficher */
	    printf("%s/%s\n",parent,fichier->d_name); 

	 }
      }

      free(temp);
   }

   if(closedir(repertoire) != 0)
   {
      perror("closedir failed");
      exit(EXIT_FAILURE);
   }
   free(tempparent);
}
