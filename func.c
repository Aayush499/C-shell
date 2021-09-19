#include "headers.h"
#include "func.h"


extern int exitcode;

void splitargs(char **args, size_t *argscnt, char* delim, char *cmd)
{
    *argscnt = 0;
    args[*argscnt]= strtok(cmd, delim);
    while(args[*argscnt]!=NULL)
    {
        (*argscnt)++;
        args[*argscnt]= strtok(NULL, delim);
    }
}


//calling gedit changes teh std_out and std_in
void io_restore(int std_out, int std_in) {
    dup2(std_out, STDOUT_FILENO);
    dup2(std_in, STDIN_FILENO);
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int numval(char *str) {
    int i = (int) strlen(str) - 1;
    int baseten = 1, sum = 0;
    while (i >= 0) {
        sum += (str[i] - '0') * baseten;
        baseten *= 10;
        i--;
    }
    return sum;
}

void quit(int procscnt, int std_out, int std_in) {
    if (procscnt != 0)
        printf(YELLOW"\nWARNING: "RESET "There are stopped jobs");

    close(std_out);
    close(std_in);
    printf(MAGENTA"\nExiting shell... "YELLOW" BYE!\n"RESET);
}

int syntaxchk(char *cmd) {
    
      int  f = 0;

    return f;
}
