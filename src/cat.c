void cat(char **args, char *buff, int nargs)
{


 /* cat sur stdin */
	 if(nargs == 1)
	 {
	    while(fgets(buff,BUFF,stdin)) 
	    {
	      printf("%s\n",buff); 
	    }
	 }
	 else
	 {
	    /* cat sur des fichiers */

	    for(j=1 ; j < nargs ; j++)
	    {
	       /* Si l'option -n est activee */
	       if(strcmp(args[j] ,"-n") == 0 || strcmp( args[j],"--number") == 0)
	       {  
		  checkN = 1;
	       }
	    }
	    
	    /* l'option -n est présente */
	    if(checkN == 1)
	    {
	       count = 1;
	       for( j=1 ; j < nargs ; j++)
	       {
		  /* on ne traite pas l'option en tant que fichier */
		  if( strcmp(args[j] ,"-n") != 0 &&  strcmp( args[j],"--number") != 0)
		  {
		     /* on lit ligne par ligne pour pouvoir ajouter au début le numéro de ligne **/
		     file = fopen(args[j], "r");
		     if(file  != NULL )
		     {
			/* On écrit lit le fichier ligne par ligne */	
			while( fgets ( line, sizeof line, file) != NULL )
			{
			   printf("\t %d\t%s",count,line);
			   count++;
			}
			fclose(file);
		     }
		     else
		     {
			/* n'affichera que des erreurs disant que le fichier n'existe pas */
			perror(args[j]);
		     }
		  }
	       }
	    }
	    else
	    {
	       /* l'option -n n'est pas présente */
	       for( j=1 ; j < nargs ; j++)
	       {

		  /* on vérifie que le fichier n'est pas un répertoire */
		  stat(args[j], &infos);
		  if(S_ISDIR(infos.st_mode))
		  {
		     /* il s'agit d'un répertoire et on ne cat pas un répertoire ! */ 
			printf("cat : %s: Is a directory\n",args[j]);

		  }
		  else
		  {
		     /* il ne s'agit pas d'un répertoire */ 
		     fichier=open(args[j], O_RDONLY);
		     
		     /* on vérifie si le fichier existe */
		     if(fichier != -1)
		     {
			/* tant qu'il reste des choses à écrire */
			while(( encore = read(fichier, buff, BUFF)) > 0)
			{
			   write(STDIN_FILENO, buff, BUFF);
		     	}
			close(fichier);
		     }
		     else
		     {
			/* il ne s'agit pas d'un fichier */
			printf("cat : %s : No such file\n",args[j]);
		     }

		  }
	       }
	    }
	 }
}
