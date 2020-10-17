#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    int second1 = current_time.tv_sec;
    int second2;
    int microsec1 = current_time.tv_usec;
    int microsec2;
    int code = fork();
    switch (code)
    {
    case 0:
        // Codigo del proceso hijo
        system(argv[1]);
    case -1:
        // Error
        break;
    }
    wait(NULL); // Ensayarlo sin comentarlo y comentandolo a ver que pasa
    if (code != 0)
    {
        gettimeofday(&current_time, NULL);
        second2 = current_time.tv_sec;
        microsec2 = current_time.tv_usec;
        int secondsTime = second2 - second1;
        int microsecTime = microsec2 - microsec1;
        printf("time:%i,%i\n", secondsTime, microsecTime);
    }

    return 0;
}