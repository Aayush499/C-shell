#include "headers.h"
#include "prompt.h"

extern int exitcode;
//GET CURRENT DIRECTORY
char *curdir(char *dirname, char *actdir)
{
    if(strncmp(dirname,actdir, strlen(actdir)))  // if they are not equal, then our current direcotry is the directory entered by user
        return dirname;
    
    else if(strlen(actdir) == strlen(dirname))   // If they are equal, that means we're back int he directory that activated the shell
        return "~";
    else
    {
    char *str = (char*)malloc(dirsize);
    memset(str, '\0', dirsize);
    str[0] = '~';

    return strcat(str, dirname  + strlen(actdir));
    }

}

int prompt(char *dirname, char *actdir, char **buf, size_t *bufsize)
{
         if (!exitcode)
        printf(RED":O"RESET);
        else 
        printf(GRAY":D"RESET);

        char *loginname, hostname[hostsize];
        memset(dirname, '\0', dirsize);
        memset(hostname, '\0', hostsize);

        loginname = getlogin();
        getcwd(dirname, dirsize);
        gethostname(hostname, hostsize);
        printf(GREEN"%s@%s\e[0m:\e[0;34m%s\e[0m>"RESET, getlogin(), hostname, curdir(dirname, actdir));
        exitcode = 1;

        *buf = NULL;
        *bufsize = 0;

    int ret;
        if(getline(buf, bufsize, stdin)== -1)
        ret= -1;
        else
        {
        (*buf)[strlen(*buf)-1]= '\0';
        ret =0;
        }
    return ret;
}