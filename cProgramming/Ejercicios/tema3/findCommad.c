#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error al usar findCommand %s\n", argv[1]);
        return 1;
    }

    char *ENV = getenv("PATH");
    printf("ENV: %s\n\n\n", ENV);

    char *result = strtok(ENV, ":");

    while(result != NULL)
    {
        printf("DIRECTORIO: %s\n", result);

        char* cat = strcat(result, "/");
        printf("%s\n", cat);

        result = strtok(NULL, ":");
    }

    return 0;
}
