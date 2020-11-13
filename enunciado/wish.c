#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *mypath[] = {
        "/bin/",
        NULL};

    char error_message[30] = "An error has occurred\n";

    //Modo interactivo
    if (argc == 1)
    {

        while (1)
        {
            //Wait for input
            printf("prompt> ");
            char *line = NULL;
            size_t len = 0;
            ssize_t read;

            //Leemos por linea
            if ((read = getline(&line, &len, stdin)) == -1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue;
            }

            //Eliminar espacio al final
            line[strlen(line) - 1] = '\0';

            //Guardar cada comando en una matriz
            char *word;
            char *arguments[100];
            for (int i = 0; (word = strsep(&line, "&")) != NULL; i++)
            {
                if (strcmp(word, "") != 0 && strcmp(word, "\t") != 0)
                {
                    arguments[i] = word;
                    arguments[i + 1] = NULL;
                }
                else
                {
                    i = i - 1;
                };
            };

            //Recorrer los comandos
            for (int i = 0; arguments[i] != NULL; i++)
            {

                //Separar las palabras de cada comando
                char *word2;
                char *word3;
                char *arguments2[100];
                char *file = NULL;
                char *file2 = NULL;
                int isRedirection = 0;
                int error = 0;
                char *args = strdup(arguments[i]);
                for (int j = 0; (word2 = strsep(&args, ">")) != NULL; j++)
                {
                    if (j == 0)
                    {
                        arguments[i] = word2;
                    }
                    else if (j == 1)
                    {
                        isRedirection = 1;
                        file = word2;
                        if (file == NULL)
                        {
                            error = 1;
                        }
                    }
                    else
                    {
                        error = 1;
                        break;
                    }
                }
                if (isRedirection == 1)
                {
                    if (file != NULL)
                    {
                        int escrito = 0;
                        char *word6;
                        char *word7;

                        for (int j = 0; (word6 = strsep(&file, " ")) != NULL; j++)
                        {
                            for (int k = 0; (word7 = strsep(&word6, "\t")) != NULL; k++)
                            {
                                if (strcmp(word7, "") != 0)
                                {

                                    if (escrito == 0)
                                    {

                                        escrito = 1;
                                        file2 = word7;
                                        j = j + 1;
                                    }
                                    else
                                    {
                                        error = 1;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if (error == 1 || (file2 == NULL && isRedirection == 1))
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    break;
                }

                printf("imprimir file 2 porq toca %s\n", file2);
                //Quito los espacios
                for (int j = 0; (word2 = strsep(&arguments[i], " ")) != NULL; j++)
                {
                    //Quito las tabulaciones
                    for (int k = 0; (word3 = strsep(&word2, "\t")) != NULL; k++)
                    {
                        //Confirmar que no este vacia la palabra
                        if (strcmp(word3, "") != 0)
                        {
                            arguments2[j] = word3;
                            arguments2[j + 1] = NULL;
                            j = j + 1;
                        }
                    }
                    j = j - 1;
                };

                //Validamos si el comando es exit, cd o path
                if (i == 0)
                {

                    // EXIT
                    if (strcmp(arguments2[0], "exit") == 0)
                    {
                        if (arguments2[1] == NULL)
                        {
                            exit(0);
                            break;
                        }
                        else
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            break;
                        }
                    };
                    // CD
                    if (strcmp(arguments2[0], "cd") == 0)
                    {

                        if (chdir(arguments2[1]) == -1)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }

                        break;
                    };
                    //PATH
                    if (strcmp(arguments2[0], "path") == 0)
                    {
                        if (arguments2[1] == NULL)
                        {
                            mypath[0] = NULL;
                        }

                        for (int k = 1; arguments2[k] != NULL; k++)
                        {
                            char *palabra = arguments2[k];
                            char primerc = palabra[0];
                            char pathAct[200] = "./";
                            char pathAct2[200] = "";
                            char *slash = "/";
                            if (primerc == 47)
                            {
                                strcat(pathAct2, arguments2[k]);
                                strcat(pathAct2, slash);
                                mypath[k - 1] = pathAct2;
                                mypath[k] = NULL;
                            }
                            else
                            {
                                strcat(pathAct, arguments2[k]);
                                strcat(pathAct, slash);
                                mypath[k - 1] = pathAct;
                                mypath[k] = NULL;
                            }
                        }
                        break;
                    };
                };
                int a = 1;
                //Recorremos la ruta que nos mandaron
                for (int j = 0; mypath[j] != NULL; j++)
                {
                    char *path = strdup(mypath[j]);
                    strcat(path, arguments2[0]);

                    //Validamos si el comando existe en la ruta
                    if (access(path, F_OK) == 0)
                    {
                        a = 0;
                        if (fork() == 0)
                        {
                            execv(path, arguments2);
                            return (0);
                        };
                        wait(NULL);
                        break;
                    };
                };
                if (a == 1)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
            };
        };
    };

    //Modo batch
    if (argc == 2)
    {
        //Leemos el archivo
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            printf("wunzip: cannot open file\n");
            exit(1);
        };
        FILE *fp2 = fopen(argv[1], "r");

        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        //Leemos por linea del archivo
        while ((read = getline(&line, &len, fp)) != -1)
        {
            if (strcmp(line, "") != 0)
            {

                line[strlen(line) - 1] = '\0';
                //Guardar cada comando en una matriz
                char *word;
                char *arguments[100];
                for (int i = 0; (word = strsep(&line, "&")) != NULL; i++)
                {
                    if (strcmp(word, "") != 0 && strcmp(word, "\t") != 0)
                    {
                        arguments[i] = word;
                        arguments[i + 1] = NULL;
                    }
                    else
                    {
                        i = i - 1;
                    };
                };

                //Recorrer los comandos
                for (int i = 0; arguments[i] != NULL; i++)
                {

                    //Separar las palabras de cada comando
                    char *word2;
                    char *word3;
                    char *arguments2[100];

                    //Quito los espacios
                    for (int j = 0; (word2 = strsep(&arguments[i], " ")) != NULL; j++)
                    {
                        //Quito las tabulaciones
                        for (int k = 0; (word3 = strsep(&word2, "\t")) != NULL; k++)
                        {
                            //Confirmar que no este vacia la palabra
                            if (strcmp(word3, "") != 0)
                            {
                                arguments2[j] = word3;
                                arguments2[j + 1] = NULL;
                                j = j + 1;
                            }
                        }
                        j = j - 1;
                    };

                    //Validamos si el comando es exit, cd o path
                    if (i == 0)
                    {

                        // EXIT
                        if (strcmp(arguments2[0], "exit") == 0)
                        {
                            if (arguments2[1] == NULL)
                            {
                                exit(0);
                                break;
                            }
                            else
                            {
                                write(STDERR_FILENO, error_message, strlen(error_message));
                                break;
                            }
                        };
                        // CD
                        if (strcmp(arguments2[0], "cd") == 0)
                        {
                            if (chdir(arguments2[1]) == -1)
                            {
                                write(STDERR_FILENO, error_message, strlen(error_message));
                            }

                            break;
                        };
                        //PATH
                        if (strcmp(arguments2[0], "path") == 0)
                        {
                            if (arguments2[1] == NULL)
                            {
                                mypath[0] = NULL;
                            }

                            for (int k = 1; arguments2[k] != NULL; k++)
                            {
                                char *palabra = arguments2[k];
                                char primerc = palabra[0];
                                char pathAct[200] = "./";
                                char pathAct2[200] = "";
                                char *slash = "/";
                                if (primerc == 47)
                                {
                                    strcat(pathAct2, arguments2[k]);
                                    strcat(pathAct2, slash);
                                    mypath[k - 1] = pathAct2;
                                    mypath[k] = NULL;
                                }
                                else
                                {
                                    strcat(pathAct, arguments2[k]);
                                    strcat(pathAct, slash);
                                    mypath[k - 1] = pathAct;
                                    mypath[k] = NULL;
                                }
                            }
                            break;
                        };
                    };
                    int a = 1;
                    //Recorremos la ruta que nos mandaron
                    for (int j = 0; mypath[j] != NULL; j++)
                    {
                        char *path = strdup(mypath[j]);
                        strcat(path, arguments2[0]);
                        //Validamos si el comando existe en la ruta
                        if (access(path, F_OK) == 0)
                        {
                            a = 0;
                            if (fork() == 0)
                            {
                                execv(path, arguments2);
                                return (0);
                            };
                            wait(NULL);
                            break;
                        };
                    };
                    if (a == 1)
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                };
            };
        };
        fclose(fp);
        fclose(fp2);
        exit(0);
    };

    //Mando algo erroneo
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
}
