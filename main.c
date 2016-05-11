#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#if defined(__APPLE__)
#define HOME "/Users/"
#elif defined(__unix__)
#define HOME "/home/"
#endif

#define HISTORY_FILE ".tpshell_history"
#define HISTORY_SIZE 500

#define BUFF            1024
#define PATH_SIZE       1024
#define MAX_ARGS_SIZE   512
#define CYAN  "\033[1m\033[36m"
#define YELO  "\033[1m\033[33m"
#define WHIT  "\x1B[0m"
#define MAGE  "\x1B[35m"
#define RESET "\033[0m"

int getnargs(char *buff);
void getargs(char *buff, char **args);

int main(int argc, char *argv[]) {
   pid_t    pid;     /* pid du processus en cours */
   int      status;  /* statut du processus */
   int      nargs;   /* nombre d'aguments */
   char     *dir;    /* dossier courant à afficher */
   char     *buff;   /* buffer */
   char     **args;  /* tableau d'arguments */
   char     *temp;   /* */
   char     *host;   /* nom d'hôte de la machine */
   char     *login;  /* nom d'utilisateur */
   int      i;       /* */

   buff = (char*) malloc(BUFF);
   temp = (char*) malloc(BUFF);
   dir = (char*) malloc(PATH_SIZE);
   host = (char*) malloc(BUFF);
   login = (char*) malloc(BUFF);

   gethostname(host, BUFF);
   strcpy(login, getlogin());

   getcwd(dir, PATH_SIZE);

   while(printf("%s%s%s@%s%s%s%s%s:%s%s%s>%s ", MAGE, login, YELO, RESET, MAGE, host, MAGE, WHIT, CYAN, dir, YELO, WHIT), fgets(buff, BUFF, stdin)) {
      /* Si l'utilisateur n'écrit rien et appuie sur Entrée */
      if(!strcmp(buff, "\n")) {
         continue;
      }

      /* On supprime le retour à la ligne du buffer */
      buff[strlen(buff)-1] = '\0';
      /* On compte le nombre d'arguments dans la commande entrée */
      nargs = getnargs(buff);
      /* On alloue l'espace pour le tableau d'arguments et on les récupère */
      args = (char**) malloc((nargs+1)*sizeof(char*));
      for(i = 0; i < nargs+1; i++) args[i] = (char*) malloc(MAX_ARGS_SIZE*sizeof(char));
      args[1][0] = 0;
      getargs(buff, args);
      args[nargs] = NULL;

      /* gestion de CD */
      if(!strcmp(args[0], "cd")) {
         if(args[1] == NULL || !strcmp(args[1], "~")) { /* gestion du HOME */
            strcpy(temp, HOME);
            strcat(temp, login);
            chdir(temp);
         }
         else {
            chdir(args[1]);
         }
         getcwd(dir, PATH_SIZE); /* On met à jour le répertoire courant à afficher */
         continue;
      }

      if(!strcmp(args[0], "exit")) {
         exit(0);
      }

      if(!strcmp(args[0], "history")) {
         /* Créer fichier s'il n'existe pas */
         /* Compter les lignes dans le fichier */
         /* Si lignes > HISTORY_SIZE, on supprime la première ligne et on ajoute la nouvelle à la fin */
         /* Sinon on ajoute juste à la fin */
         printf("Historique :\n");
         continue;
      }

      if(!strcmp(args[0], "touch")) {
         printf("Create file\n");
         continue;
      }

      if(!strcmp(args[0], "cat")) {
         printf("Cat sur un fichier ou sur stdin\n");
         continue;
      }

      if(strcmp(args[0], "copy")) {
         printf("Copy TD1\n");
         continue;
      }

      /* On fork et on lance la commande dans le fils */
      pid = fork();
      if(pid == 0) {
         if (execvp(args[0], args) < 0) {
            printf("%s: command not found\n", args[0]);
         }
         exit(0);
      }
      waitpid(pid, &status, WUNTRACED | WCONTINUED);
      /* On libère l'esapce occupé par les arguments et le buffer */
      for(i = 0; i < nargs+1; i++) free(args[i]);
      free(args);
      free(buff);
      buff = (char*) malloc (BUFF);

   }
   return -1;
}


/* getnargs : compte le nombre d'argument dans buff, en se basant sur le nombre d'espaces *
 * Entrée : une chaine de caractères buff
 * Sortie : un entier */
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