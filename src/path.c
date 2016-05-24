







/* Gestion du PATH */
      int      npaths, i = 0;
      char     *path;
      char     **paths_exe;
      char     **paths;
	  
      path = (char*) malloc(BUFF);
      strcpy(path, getenv("PATH"));

      npaths = getnpaths(path);

      paths = (char**) malloc((npaths+1)*sizeof(char*));
      for(i = 0; i < npaths+1; i++) paths[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      getpaths(path, paths);

      paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
      for(i = 0; i < npaths+1; i++) paths_exe[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      getexepaths(args[0], paths_exe, paths, npaths);


         for(i = 0; i < npaths; i++) {
            if (execv(paths_exe[i], args)) {
            }
         }

         exit(0);
      }
      

