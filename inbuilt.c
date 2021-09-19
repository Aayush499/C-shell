#include "headers.h"
#include "inbuilt.h"

extern int exitcode;


void echo(char **args, int counter)         //argscnt: no. of arguments, args: all my arguments
{   int i=1;
    while( i< counter)
    {
        printf("%s ", args[i]);
        i++;
    }
    puts("");
    exitcode = 1;
}

void pwd(char *dirname)
{
    printf("%s\n", dirname);
    exitcode = 1;
}


void cd(char **args, int counter, char *actdir, char *olddir)
{   
     char curdir[dirsize], newdir[dirsize];
    exitcode = 0;
    int chk =2;
    getcwd(curdir, dirsize);

    if(counter == 1)
    {
        if(chdir(actdir)==-1){
            perror("Error");
            return;
        }
        else
        strcpy(olddir, curdir);
    }
    else if(counter == chk /*2*/)
    {
        if(args[1][0] == '~')       //if new directory starts with root
        {
            strcpy(newdir, actdir);
            strcat(newdir, args[1] + 1);

        }
        else if(!strcmp(args[1], "-"))
        {
            if(!olddir[0] /*== '\0'*/)
            {
                fprintf(stderr, "OLDPWD not set yet\n");
                return;
            }
            else
            strcpy(newdir, olddir);

        }
        else
            strcpy(newdir, args[1]);

        if(chdir(newdir)==-1)
        {
            perror("Error in chdir");
            return;
        }
        else
        strcpy(olddir, curdir);
    }
    else
    {
        printf("Too many arguments");
        return;
    }

    exitcode = 1;
    
}


