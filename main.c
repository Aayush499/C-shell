#include "headers.h"
#include "func.h"
#include "prompt.h"
#include "inbuilt.h"
#include "ls.h"
#include "pinfo.h"
#include "proc.h"
#include "history.h"

proc proclist[procmax];

int procscnt = 0, jobcnt = 0, childid = 0;

int exitcode = 2;

char **args;

sigjmp_buf ctrlc_buf;

void sig_handler(int signum) {
    switch (signum) {
        case SIGCHLD:
            signal(SIGCHLD, sig_handler);
            procchk(proclist, &procscnt, &jobcnt);
            break;
        default:
            printf("unhandled signal\n");
    }
}

int main() {
    const int chk =0; //aayush
     clearScreen();
    printf(BLUE "Aayush's shell \n\n"RESET);
    int std_out = dup(STDOUT_FILENO), std_in = dup(STDIN_FILENO);
    size_t argscnt, inpbufsz = 0;
    char *inpbuf = NULL, *cmd, *subcmd;
    char dirname[dirsize], actdir[dirsize], olddir[dirsize];
    int forkstatus = 0, rep =1;
    int st = 0; //status
    // int pipefd[2], pipeprev;

    memset(actdir, 0/*\0*/, sizeof(actdir));
    getcwd(actdir, dirsize);
    olddir[0] = 0;

    char histfile[dirsize + 10];
    strcpy(histfile, actdir);
    strcat(histfile, "/history.txt");

    if (signal(SIGCHLD, sig_handler) == SIG_ERR)            //child process death error handler
        perror("Error in signal syscall" RED"DEATH"RESET);
    

    args = (char **) malloc(arglimit * sizeof(char *));
    
    while (1) {
        rep =1;
        while (sigsetjmp(ctrlc_buf, 1));

        if (prompt(dirname, actdir, &inpbuf, &inpbufsz) == -1) {
            quit(procscnt, std_out, std_in);
            break;
        }

        addhistory(inpbuf, histfile);

        cmd = strtok_r(inpbuf, ";", &inpbuf);
        while (cmd != NULL) {
            // pipeprev = dup(STDIN_FILENO);

            

            subcmd = strtok_r(cmd, "|", &cmd);
            while (subcmd != NULL) {
                splitargs(args, &argscnt, " ", subcmd);
                subcmd = strtok_r(cmd, "|", &cmd);

                if (subcmd == NULL)
                    dup2(std_out, STDOUT_FILENO);

                 //st = io_redirect(args, &argscnt);
                 st =0;

                 //repeat checker
                 if(!strcmp(args[0], "repeat"))
                 {
                     rep = numval(args[1]);
                     args+=2;
                     argscnt -=2;
                 }
                 for(int repeater =1; repeater<=rep; repeater++)
                    if (!st) {
                    if (args[0] == NULL) continue;
                    else if (!strcmp(args[0], "echo")) 
                    echo(args, argscnt);
                    else if (!strcmp(args[0], "pwd")) pwd(dirname);
                    else if (!strcmp(args[0], "cd") ) cd(args, argscnt, actdir, olddir);
                    else if (!strcmp(args[0], "ls") ) 
                    printlist(args, argscnt, dirname, actdir);
                    else if (!strcmp(args[0], "pinfo")) pinfo(args, argscnt);
                    else if (!strcmp(args[0], "history")) 
                    printhistory(args, argscnt, histfile);
                    else if (!strcmp(args[0], "quit")) {
                        quit(procscnt, std_out, std_in);
                        return 0;
                    }
                    else {
                        childid = fork();                                   // a positive number for the parent
                        if (setpgid(childid, childid)==-1)
                            perror("Unsuccessful bg process");

                        if (childid < chk) {
                            perror("Error while forking");
                            exitcode = 0;
                        }
                        //SUCCESS
                        else if (!childid) {            //inside the child
                            // signal(SIGINT, SIG_DFL);
                            
                            signal(SIGTSTP, SIG_DFL);
                            signal(SIGCHLD, SIG_DFL);
                            if (!strcmp(args[argscnt - 1], "&")) {
                                args[argscnt - 1] = NULL;
                            }
                            printf("can you see me?");
                            execvp(args[0], args);
                            
                            perror("execvp");
                            exit(1);
                        }
                        else {
                            io_restore(std_out, std_in);
                            puts("and we're back");
                            if (strcmp(args[argscnt - 1], "&"))    // foreground process
                            {
                                signal(SIGTTOU, SIG_IGN);
                                tcsetpgrp(0, getpgid(childid));
                                signal(SIGTTOU, SIG_DFL);

                                if (waitpid(childid, &forkstatus, WUNTRACED) < chk)
                                    perror("Error in waiting");

                                if (WIFSTOPPED(forkstatus)!=chk /*0*/) {
                                    procadd(proclist, &procscnt, &jobcnt, args[chk]/*chk*/, childid);
                                    printf("\n[%d] Stopped %d : %s\n", jobcnt, childid, args[chk]/*0*/);
                                    exitcode = chk;
                                }

                                signal(SIGTTOU, SIG_IGN);
                                tcsetpgrp(0, getpgid(getpid()));
                                signal(SIGTTOU, SIG_DFL);
                            }
                            else {
                                procadd(proclist, &procscnt, &jobcnt, args[chk]/*0*/, childid);
                                printf("[%d] %d\n",  jobcnt, getpid());
                            }
                        }
                        childid = chk;
                    }
                }
                // close(pipefd[0]);
                // close(pipefd[1]);
            }
            // close(pipeprev);

            cmd = strtok_r(inpbuf, ";", &inpbuf);
            io_restore(std_out, std_in);
        }
    }
}