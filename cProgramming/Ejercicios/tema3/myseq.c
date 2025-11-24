#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    if (argc < 2 || argc > 4)
    {
        printf("Error. El numero de argumentos es incorrecto\n");
        return 1;
    }

    int last = 1, first = 1, increment = 1;
    switch (argc)
    {
        case 2:
            last = atoi(argv[1]);
            break;
        case 3:
            first = atoi(argv[1]);
            last = atoi(argv[2]);
            break;
        case 4:
            first = atoi(argv[1]);
            increment = atoi(argv[2]);
            last = atoi(argv[3]);
            break;
        default:
            printf("Error en la estructura del switch\n");
            break; 
    }

    // printf("First: %d\nIncrement: %d\nLast: %d\n", first, increment, last);

    for (int i = first; i <= last; (i += increment))
    {
        printf("%d\n", i);
    }

    return 0;
}