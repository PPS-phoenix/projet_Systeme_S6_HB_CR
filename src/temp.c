int execute(char **args, int nargs, char *history_file_path, char * dir)
{

      /* gestion de CD */
      if(!strcmp(command[j][0],
         getcwd(dir, PATH_SIZE); /* On met à jour le répertoire courant à afficher */
      }
     

      /* fonction exit */
      if(!strcmp(command[j][0], "exit")) {
         exit(0);
      }

            
      /* gestion de history */
      if(!strcmp(command[j][0], "history")) {
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
      }
      
      if(!strcmp(command[j][0], "copy")) {
	 printf("Copy TD1\n");
      }
      

      /* On fork et on lance la commande dans le fils */
	 /* fonction cat */
	 if(!strcmp(command[j][0], "cat")) {
	    cat(command[j],tabArgs[j]);
	 }

	 /* Gestion de touch */
	 if(!strcmp(args[0], "touch")) {
	    touch(args,nargs);
	 }

	 /* Gestion du PATH */
	   

	 path = (char*) malloc(BUFF);
	 strcpy(path, getenv("PATH"));

	 npaths = getnpaths(path);

	 paths = (char**) malloc((npaths+1)*sizeof(char*));
	 for(i = 0; i < npaths+1; i++) paths[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getpaths(path, paths);

	 paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
	 for(i = 0; i < npaths+1; i++) paths_exe[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
	 getexepaths(command[j][0], paths_exe, paths, npaths);

	 for(i = 0; i < npaths; i++) {
	    if (execv(paths_exe[i], args)) {
	       }
	 }

}
