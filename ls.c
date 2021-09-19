#include "headers.h"
#include "ls.h"

extern int exitcode;

//dirname is directory im in rn
//actdir is the directory I activated the shell or my root
//args is the list or arguments
//cnt is the number of arguments
void printlist(char **dir, int cnt, char *dirname, char *actdir) {

    unsigned int flags = 0, l= 1U,a= 2U,al = 3U;
    int dircnt = 0;
    char newdir[dirsize];
    //isolate each argument and check if its a flag or directory name

    for(int i=1 ; i< cnt; i++)
    {
        if(strcmp(dir[i], "-l")==0)
            flags|= l;
        else if(strcmp(dir[i], "-a")==0)
            flags |= a;
        else if(strcmp(dir[i], "-la")==0 || strcmp(dir[i],"-al")==0)
            flags |= al;
        else
            dircnt++;
    }

    if(dircnt == 0)
        printentity(".", flags);            // list out the current directory

    else
    {
        int j=1;
        while(j<cnt)
        {
            if(dir[j][0]=='-')        //skip the flags
            {
                j++;
                continue;
            }
            else if(dir[j][0]=='~')      // identitfying an absolute path from the root
            {
                strcpy(newdir, actdir);
                strcat(newdir, dir[j]+1);
            }
            else
            {
                strcpy(newdir, dir[j]);
            }
            if (stat(newdir, &statbuf)) {
                perror("error in stat");
                exitcode = 0;
                j++;
                continue;
            }
            if(S_ISREG(statbuf.st_mode))
            {
                printf("%s\n\n", newdir);
            }
            else if(S_ISDIR(statbuf.st_mode))
            {
                printf("%s:\n", dir[j]);
                if(strcmp(newdir, "~")==0)
                printentity(dirname, flags);
                else
                    printentity(newdir, flags);
            }
            else
                 printf("%s\n\n", newdir);

            j++;
        }
    }
    

}

void printentity(char *name, ui flags)
{
    unsigned int  l= 1U,a= 2U,al = 3U;
    int counter = scandir(name, &file, NULL, alphasort/*alphasort()*/);       //number of files in my directory!!!!
    char current[dirsize];
    getcwd(current, dirsize);
    int j=0, i=0;
    chdir(name);

    ll total =0;

    while(j<counter)
    {
        if(!(flags & 2UL) && (file[j]->d_name[0]=='.'))
        {
             j++;
            continue;
        }
        if(stat(file[j]->d_name, &statbuf)!=0)
        {
            printf("%s\n", file[j]->d_name);
            perror("stat");
            exitcode =0;
            j++;
            continue;
        }
        total+= statbuf.st_size;
        j++;
    }
    total /= 1024;              // The apparent memory, NOT the disk usage
    if(flags & 1UL)
    printf("total %lld\n", total);

    while(i<counter)
    {   
        if(!(flags &2UL) && (file[i]->d_name)[0]=='.')
        {
            i++;
            continue;
        }
        if(flags & 1UL)
        {
            if(stat(file[i]->d_name, &statbuf)!=0)
        {
            printf("%s\n", file[i]->d_name);
            perror("stat");
            exitcode =0;
            i++;
            continue;
        }

        long int mode = statbuf.st_mode;
        char perm[11] = "----------";
        if (S_ISDIR(mode))perm[0] = 'd';
            else if (S_ISBLK(mode))perm[0] = 'b';
            else if (S_ISCHR(mode))perm[0] = 'c';
            else if (S_ISLNK(mode))perm[0] = 'l';
            else if (S_ISSOCK(mode))perm[0] = 's';
            else if (S_ISFIFO(mode))perm[0] = 'p';

            perm[1] = (mode & S_IRUSR)?'r':'-';
            perm[2] = (mode & S_IWUSR)?'w':'-';
            perm[3] = (mode & S_IXUSR)?'x':'-';

            perm[4] =  (mode & S_IRGRP)?'r':'-';
            perm[5] = (mode & S_IWGRP)?'w':'-';
            perm[6] = (mode & S_IXGRP)?'x':'-';

            perm[7] = (mode & S_IROTH)? 'r':'-';
            perm[8] = (mode & S_IWOTH)?'w':'-';
            perm[9] = (mode & S_IXOTH)?'x':'-';

            char time[36];
            user = getpwuid(statbuf.st_uid);
            grp = getgrgid(statbuf.st_gid);

            printf(CYAN"%s \t %lu \t %s \t %s \t %ld \t %s \t %s\n"RESET, perm, statbuf.st_nlink, user->pw_name,
                   grp->gr_name, statbuf.st_size, formatdate(time, statbuf.st_mtime), file[i]->d_name);
        }
        else
        printf("%s\n", file[i]->d_name);

        i++;
    }

    chdir(current);
    printf("\n");

}
char *formatdate(char *str, time_t val) {
    strftime(str, 36, "%b %e %H:%M", localtime(&val));
    return str;
}