#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

char** separador(char *lineaComando, char **comandReturn);

int main(int argc, char *argv[])
{
    char error_message[30] = "An error has occurred\n";
    char *strExit = "exit";
    char *strCd = "cd ";
    //int size = 1;
    //char commandsArguments[size];
    //
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

                //Aquí va el método separador
                char **commandsAndArguments;
                commandsAndArguments = separador(line, commandsAndArguments);

                if(strcmp(commandsAndArguments[0], strCd)){

                    char *argument = NULL;
                    argument = strdup(commandsAndArguments[1]);
                    printf("word: %s\n", argument);
                    if(chdir(argument) == -1 ){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }else{
                        char s[100];
                        printf("Ubicación después de CD: %s\n", getcwd(s, 100));
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

//Método para separar la línea de comando en un arreglo de palabras
char** separador(char *lineaComando, char *comandReturn[]){
    char *word;
    int count = 0;
    char *lineCopy = NULL;
    lineCopy = strdup(lineaComando);

    while ((word = strsep(&lineCopy, " ")) != NULL) {
        count++;
    }

    //comandReturn[count];
    for(int i=0; (word = strsep(&lineaComando, " ")) != NULL; i++ ){
        comandReturn[i] = word;
    }

    return comandReturn;
}
