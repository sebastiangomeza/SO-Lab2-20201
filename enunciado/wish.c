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
    char *strExit = "exit";
    char *strCd = "cd ";
    //int size = 1;
    //char commandsArguments[size];
    char *word;
    /*
    char *mypath[] = {
        "/bin",
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

                int count = 0;
                char *lineCopy = NULL;
                lineCopy = strdup(line);

                while ((word = strsep(&lineCopy, " ")) != NULL) {
                    count++;
                    //printf("W1 word: %s\n", word);
                    //printf("Count: %i\n", count);
                }

                char *commandsAndArguments[count];
                for(int i=0; (word = strsep(&line, " ")) != NULL; i++ ){
                    commandsAndArguments[i] = word;
                    //printf("word: %s\n", word);
                    //printf("in array: %s\n", commandsAndArguments[i]);
                }

                //printf("Array %s: \n", commandsAndArguments);

                printf("in array 0: %s\n", commandsAndArguments[0]);
                printf("in array 1: %s\n", commandsAndArguments[1]);

                if(strcmp(commandsAndArguments[0], strCd)){
                    char *argument = NULL;
                    argument = strdup(commandsAndArguments[1]);
                    //argument[strlen(argument) - 1] = '\0';
                    printf("word: %s\n", argument);
                    //[strlen(commandsAndArguments[1]) - 1];
                    if(chdir(argument) == -1 ){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                }else{
                    printf("Comando diferente");
                }


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