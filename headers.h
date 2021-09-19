
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <setjmp.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <stdio.h>

struct stat statbuf;
#define arglimit 100    // assuming max 100 arguments
#define procmax 100     // max number of processes at any time
#define hostsize 500
#define dirsize 500

//screen sweeper
#define clearScreen() printf("\033[H\033[J")

//colors
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define GRAY "\x1b[1;30m"
#define ui unsigned int
#define ll long long