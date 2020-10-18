#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char error_message[30] = "An error has occurred\n";
    char strExit[6] = "exit";
    /*
    char *mypath[] = {
        "./",
        "/usr/bin/",
        "/bin/",
        NULL
        };
    */
   
    switch(argc){
    case 1:
        while(1){
            /* Wait for input */
            printf ("prompt> ");

            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            if((read = getline(&line, &len, stdin)) == -1 ){
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue;
            }

            /*Se elimina /n del text obtenido en getLine*/
            line[strlen(line) - 1] = '\0';
            
            /* Empezamos a leer los comandos */
            if(strcmp(line, strExit) == 0){
                exit(0);
            }else{
                printf ("Sebas es puto\n");
            }


        }

        break;
    case 2:

        break;

    default:
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
    }

    return 0;
}