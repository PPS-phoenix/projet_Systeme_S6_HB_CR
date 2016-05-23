if(i == -1)
	 {
	    /* premiere commande, avant le premier pipe*/
	    if((dup2(pipes[i], 1)) == -1)
	    {
	       perror("first dup2 failed");
	       exit(EXIT_FAILURE);	 
	    } 
	 }
	 else if(i == ((npipe*2)-1))
	 {	 
	    /* derniere commande */
	    /* on ne fait que lire sur la commande précédente */

	    printf("indice de i dans le last pipe : %d\n",i);
	    if((dup2(pipes[i-1],0) == -1))
	    {
	       perror("last dup2 failed");
	       exit(EXIT_FAILURE);
	    }

	 }
	 else
	 {
	    /* entre la premiere et dernière commande */
	    /* on lit sur la précédente et écrit sur la suivante */
	       
	    if((dup2(pipes[i+2],1)) == -1)
	    {
	       perror("write - dup2 failed");
	       exit(EXIT_FAILURE);
	    }


	    if((dup2(pipes[i-1],0)) == -1)
	    {
	       perror("read - dup2 failed");
	       exit(EXIT_FAILURE);
	    }
	 }

