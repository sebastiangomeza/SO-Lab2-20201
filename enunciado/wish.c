#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

char** separador(char *lineaComando, char **comandReturn);
void saludo(char **nombre);

int main(int argc, char *argv[])
{
    char error_message[30] = "An error has occurred\n";
    char *strExit = "exit";
    char *strCd = "cd";
    char *strPath = "path";
    //char commandsArguments[size];
    //
    /*
    char *mypath[] = {
        "/bin/",
        "./",
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

                if(strcmp(commandsAndArguments[0], strCd) == 0){
                    //char *nombres[2];
                    //nombres[0]="carlos";

                    //saludo(nombres);

                    char *argument = NULL;
                    //argument[strlen(argument) - 1] = '\0';
                    argument = strdup(commandsAndArguments[1]);
                    if(chdir(argument) == -1 ){
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }else{
                        char s[100];
                        printf("Ubicación después de CD: %s\n", getcwd(s, 100));
                    }

                }else if(strcmp(commandsAndArguments[0], strPath) == 0){
                    printf("Comando PATH\n");
                }else{
                    //mypath[1] = commandsAndArguments[0];
                    //int code = fork();
                    
                    //for(int i=0; my; i++){
                        //char *pathExec = strcat("bonm/", commandsAndArguments[0]);
                        //printf("Path %s\n", pathExec);
                        /*
                        if(fork() == 0){
                            execv(pathExec, commandsAndArguments);
                            exit(1);
                        }*/
                    //}
                    
                    //Esta parte funciona
                    //Path quemado
                    

                    if(fork() == 0){
                        execv("/bin/ls", commandsAndArguments);
                        exit(1);
                    }
                    wait(NULL);
                    
                }
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
        comandReturn[i+1] = NULL;
    }

    return comandReturn;
}

void saludo(char **nombre){
    printf("Hola %s\n", nombre[0]);
}
