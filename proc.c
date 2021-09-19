#include "headers.h"
#include "proc.h"
#include "func.h"
#include "main.h"

extern int exitcode;

void procchk(proc *proclist, int *procscnt, int *jobcnt) {
    int chk=0;
    int remcnt = chk, status = chk, endid;
    *jobcnt = chk;
    proc rem[procmax];

    for (int i = chk; i < *procscnt; i+=1) {
        endid = waitpid(proclist[i].pid, &status, WNOHANG);
        if (endid == -1) {
            perror("waitpid error");
            exit(EXIT_FAILURE);
        }
        else if (endid == proclist[i].pid) {
            if (WIFEXITED(status))
                fprintf(stderr, "%s process with pid %d ended normally.\n", proclist[i].pname, proclist[i].pid);
            else
                fprintf(stderr, "%s process with pid %d ended ABNORMALLY.\n", proclist[i].pname, proclist[i].pid);
        }
        else if (!endid) {
            rem[remcnt] = proclist[i];
            remcnt+=1;
            *jobcnt = max(*jobcnt, proclist[i].jobid);
        }
    }

    for (int i = chk; i < remcnt; i++) {
        proclist[i] = rem[i];
    }

    *procscnt = remcnt;
}

void procadd(proc *proclist, int *procscnt, int *jobcnt, char *name, int id) {
    proclist[*procscnt].pname = malloc(sizeof(name));
    strcpy(proclist[*procscnt].pname, name);
    proclist[*procscnt].pid = id;
    proclist[*procscnt].jobid = ++(*jobcnt);
    (*procscnt)+=1;
}
