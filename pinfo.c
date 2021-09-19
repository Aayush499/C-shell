#include "headers.h"
#include "pinfo.h"
#include "func.h"

extern int exitcode;

void pinfo(char **args, int cntr)
{
    exitcode = 0;
    int pid;
    if(cntr == 1)
    pid = getpid();
    else
    pid = numval(args[1]);

    char procfile[procmax];
    sprintf(procfile,"/proc/%d/stat", pid);

    FILE *fp = fopen(procfile, "r");            // ALL OF OUR PINFO IS STORED IN THE /PROC DIRECTORY
    if(fp==NULL)
    {   
        printf("%s", procfile);
        perror("Invalid pid");
        return;
    }

     buf = NULL;
    stats = malloc(arglimit * sizeof(char *));
    size_t bufsz = 0, statscnt = 0;
    getline(&buf, &bufsz, fp);
    splitargs(stats, &statscnt, " ", buf);
                                                    // HERE WE ACCESS THE PINFO
    printf(MAGENTA"Pid -- %d\n"RESET, pid);
    printf(MAGENTA"Process Status -- %s\n"RESET, stats[2]);
    printf(MAGENTA"Memory -- %s {Virtual Memory}\n"RESET, stats[22]);

    memset(buf, '\0', bufsz);
    sprintf(procfile, "/proc/%d/exe", pid);
    if (readlink(procfile, buf, bufsz) == -1)
        printf(MAGENTA"Executable Path NOT found.\n"RESET);
    else
    {
        printf(MAGENTA"Executable Path -- %s\n"RESET, buf);
        exitcode = 1;
    }

    fclose(fp);
    free(stats);
}