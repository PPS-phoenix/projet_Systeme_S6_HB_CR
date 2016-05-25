#include "pipe.h"

void execPipe(char *buff, int npipes, char *history_file_path, char *dir) {
   int      pipes[npipes*2];
   int      i, j, c;
   int      npaths;
   int      nargs;
   int      status;
   pid_t    pid;
   char     *car;
   char		*current;
   char     **args;
   char     *path;
   char     **paths_exe;
   char     **paths;
   char     **cmds;
   FILE     *history_file_d;

   car = (char*) malloc(BUFF);
   current = (char*) malloc(2048*sizeof(char));
   getcwd(current,2048);
   cmds = (char**) malloc((npipes+1)*sizeof(char*));
   for(i = 0; i <= npipes; i++) cmds[i] = (char*) malloc(BUFF*sizeof(char));
   getcmds(buff, cmds);

   for(i = 0; i <= ((npipes*2)-2); i+=2) {
      pipe(pipes + i);
   }

   for(i = 0; i <= npipes; i++) {
      if((pid = fork()) == 0) {
         if(i == 0) {
            dup2(pipes[1], 1);
         }
         else if(i == npipes) {
            dup2(pipes[(i*2)-2], 0);
         }
         else {
            dup2(pipes[(i*2)-2], 0);
            dup2(pipes[(i*2)+1], 1);
         }

         for(j = 0; j < npipes*2; j++) {
            close(pipes[j]);
         }

         nargs = getnargs(cmds[i]); /* On compte le nombre d'arguments dans la commande entrée */
         args = (char**) malloc((nargs+1)*sizeof(char*)); /* On alloue l'espace pour le tableau d'arguments et on les récupère */
         for(j = 0; j < nargs+1; j++) args[j] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));

         getargs(cmds[i], args);
         args[nargs] = NULL;

         if(!strcmp(args[0], "history")) {
            c = 0;
            history_file_d = fopen(history_file_path , "r");
            while(fgets(car, BUFF, history_file_d)) {
               c++;
               printf("%4d    %s", c, car);
            }
            fclose(history_file_d);
            exit(0);
         }

         if(!strcmp(args[0], "cat")) {
            cat(args,nargs);
            exit(0);
         }

         if(!strcmp(args[0], "touch")) {
            touch(args,nargs);
            exit(0);
         }

         if(strcmp(args[0], "copy") == 0 || strcmp(args[0],"cp") == 0) {
			if(nargs == 3)
			{
				copyAll(args[1],args[2]);
			}
			else
			{
				printf("Mauvais arguments.\n");
				exit(EXIT_FAILURE);
			}
			exit(0);
         }

         if(!strcmp(args[0],"find"))
         {
			 if(nargs > 2)
			 {
				 printf("Mauvais arguments.\n");
				 exit(EXIT_FAILURE);
			 }
			 else
			 {
				 if(nargs == 2 && strcmp(args[1], ".") != 0)
				 {
					find(args[1],".");
				 }
				 else
				 {
					 find(current,".");
				 }

			 }
       }

         /* Gestion du PATH */
         path = (char*) malloc(BUFF);
         strcpy(path, getenv("PATH"));

         npaths = getnpaths(path);

         paths = (char**) malloc((npaths+1)*sizeof(char*));
         for(j = 0; j < npaths+1; j++) paths[j] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
         getpaths(path, paths);

         paths_exe = (char**) malloc((npaths+1)*sizeof(char*));
         for(j = 0; j < npaths+1; j++) paths_exe[j] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
         getexepaths(args[0], paths_exe, paths, npaths);

         for(j = 0; j < npaths; j++) {
            execv(paths_exe[j], args);
         }

         exit(0);
      }
   }

   for(j = 0; j < npipes*2; j++) {
      close(pipes[j]);
   }

   for(j = 0; j <= npipes; j++) {
      wait(&status);
   }
}
