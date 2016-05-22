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
#include "parsing.h"


int getnargs(char *buff) {
   int i = 0;
   int nargs = 0;
   char c;
   while((c = buff[i]) == ' ') {
      i++;
   }
   while((c = buff[i]) != '\0') {
      if(c == ' ' && i != 0) {
         nargs++;
         while((c = buff[i] == ' ')) {
            i++;
         }
      }
      else {
         i++;
      }
   }
   return nargs+1;
}

void getargs(char *buff, char **args) {
   int i = 0;
   int k = 0;
   int j = 0;
   char c;
   while((c = buff[i]) == ' ') {
      i++;
   }
   while((c = buff[i]) != '\0') {
      if(c == ' ') {
         k = 0;
         while((c = buff[i]) == ' ') {
            i++;
         }
         j++;
      }
      else {
         args[j][k] = buff[i];
         args[j][k+1] = '\0';
         k++;
         i++;
      }
   }
}

int getnpipe(char *buff){
   
   int  npipe=0, i=0;
   while(buff[i] != '\0') 
   {
      if(buff[i] == '|')
      {
	 npipe++;
      }
      i++;
   }
   return npipe;
}

void getcommand(char *buff, char ***command, int *tabArgs){
   
   int i=0;
   const char s[2] = "|";

   char *temp;
   temp = strtok(buff,s);

   while(temp != NULL)
   {	 
      printf("\nDebug getcommand : %s\n",temp);
      tabArgs[i]=getnargs(temp);
      printf("nbArgs[%d] = %d",i,tabArgs[i]);
      getargs(temp,command[i]);
      i++;

      temp = strtok(NULL,s);
   }
}

void writehistory(char *buff, char *history_file_path) {
   FILE *history_file_d;

   history_file_d = fopen(history_file_path, "a");
   fprintf(history_file_d, "%s", buff);
   fclose(history_file_d);
}

int getnpaths(char *path) {
   int i;
   int paths = 0;

   for(i = 0; i < strlen(path) ; i++) {
      if(path[i] == ':') {
         paths++;
      }
   }

   return paths;
}

void getpaths(char *path, char **paths) {
   int i;
   int j = 0;
   int p = 0;

   for(i = 0; i < strlen(path) ; i++) {
      if(path[i] == ':') {
         p++;
         j = 0;
      }
      else {
         paths[p][j] = path[i];
         j++;
      }
   }
}

void getexepaths(char *cmd, char **paths_exe, char **paths, int npaths) {
   int i;

   for(i = 0; i < npaths; i++) {
      strcpy(paths_exe[i], paths[i]);
      strcat(paths_exe[i], "/");
      strcat(paths_exe[i], cmd);
   }
}

void replacechar(char *src, char *dst, char c, char r) {
   int i;

   for(i = 0; i < strlen(src) ; i++) {
      if(src[i] == c) {
         dst[i] = r;
      }
      else {
         dst[i] = src[i];
      }
   }
}

