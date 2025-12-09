#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error al usar findCommand\n");
        return 1;
    }

    char *ENV = getenv("PATH");
    printf("ENV: %s\n\n\n", ENV);

    char *path = strdup(ENV);
    char *result = strtok(path, ":");
    

    while(result != NULL)
    {
        printf("DIRECTORIO: %s\n", result);
        size_t len = strlen(result) + strlen(argv[1]) + 2;
        char buff[len];
        buff = strcat(result, "/");
        buff = strcat(buff, argv[1]);
        printf("BUFF vale: %s\n", buff);

        if (access(buff, X_OK) == 0)
        {
            printf("Se ha encontrado %s en %s\n", argv[1], buff);
        }
        else
        {
            printf("NOPE\n");
        }
        result = strtok(NULL, ":");
    }

    free(path);
    return 0;
}
