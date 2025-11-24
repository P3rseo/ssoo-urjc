#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    if (argc < 1 || argc > 2)
    {
        printf("ERROR. Uso incorrecto de argumentos en mycd\n%s\n", argv[0]);
        return 1;
    }

    char *DIR = getenv("HOME");
    if (argc == 2)
    {
        DIR = argv[1]; 
    }

    printf("Directorio seleccionado: %s\n", DIR);

    if (chdir(DIR) == -1)
    {
        printf("Error al ejecutar chdir\n");
        return 2;
    }
    else
    {
        printf("Se ha cambiado de directorio correctamente\n");
    }

    return 0;
}