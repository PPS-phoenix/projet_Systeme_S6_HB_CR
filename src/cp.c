#include "cp.h"

int copy(char source[],char dest[])
{

    int encore;
    char buffer[SIZE];
    /* En lecture uniquement */
    int fichierSource=open(source, O_RDONLY);

    /* En écriture uniquement O_WRONLY, vide le fichier si il existe O_TRUNC, le crée sinon O_CREAT */
    /* mode S_IWUSR pour avoir les droits en écriture */
    int fichierDest=open(dest,O_WRONLY | O_TRUNC |O_CREAT,S_IWUSR);

    if(fichierSource == -1 || fichierDest == -1)
    {
        return 0;
    }
    copyPermissions(source,dest);

    /* tant qu'il y a quelque chose à lire */
    while ((encore = read(fichierSource, buffer, SIZE)) > 0 )
    {
        write(fichierDest, buffer, SIZE);
    }

    close(fichierDest);
    close(fichierSource);
    return 1;
}

int copyPermissions(char source[],char dest[])
{
    struct stat permissions;
    stat(source,&permissions);
    if(chmod(dest,permissions.st_mode) == 0)
    {
        return 1;
    }
    else
    {
        printf("\nErreur Permissions.\n");
        return 0;
    }
}



int checkString(char *str)
{
    if(*str && str[strlen(str + 1)] != '/')
    {
        strcat(str,"/");
        /* un '/' a été ajouté */
        return 1;
    }
    else
    {
        /* ici 0 n'est pas synonyme d'echec mais signifie qu'aucun '/' n'a été ajouté */
        return 0;
    }
}



void copyAll(char source[],char dest[])
{
    DIR *repertoire= NULL;
    struct dirent* fichierLu = NULL;
    struct stat type;
    char tempSource[SIZE_CHAR],tempDest[SIZE_CHAR];

    stat (source, &type);

    /* Le cas où le fichier source est un fichier */
    if(S_ISREG (type.st_mode))
    {
        printf("\nCopie Fichier.");
        copy(source,dest);
    }

    /* Le cas où le fichier source est un répertoire */
    if(S_ISDIR (type.st_mode))
    {
        printf("\nCopie Repertoire.");
        mkdir(dest,S_IRWXU);
        copyPermissions(source,dest);

        repertoire = opendir(source);

        if(repertoire)
        {
            while((fichierLu = readdir(repertoire)) != NULL)
            {
                if(!strcmp(fichierLu->d_name, ".") || !strcmp(fichierLu->d_name, ".."))
                {
                }
                else
                {
                    /* On passe par une variable temporaraire pour ne pas perdre l'information */
                    strcpy(tempSource,source);
                    strcpy(tempDest,dest);

                    /* On vérifie et on ajoute l'antislash si il n'y est pas */
                    checkString(tempSource);
                    checkString(tempDest);

                    /* On concatène les chaines */
                    strcat(tempSource,fichierLu->d_name);
                    strcat(tempDest,fichierLu->d_name);

                    /* Copie récursive */
                    copyAll(tempSource,tempDest);
                }
            }

            closedir(repertoire);
        }
        else
        {
            printf("\nErreur Copie Repertoire.\n");
        }
    }
}

int main(int argc,char *argv[]){

	if(argc<=2)
	{	
		printf("\n\n\n2 argmuments minimum, veuillez relancer.\n\n\n");
	}

	copyAll(argv[1],argv[2]);
	return 0;
}
