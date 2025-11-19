#include <stdio.h>
int main ()
{
    FILE* prueba = fopen("/home/samuel/practica1/prueba.txt", "a");
    if (fputs("hoa que hase\n", prueba) == EOF)
    {
        printf("HAY algun error");
    }
    fputs("Are ", stdout);
    fputs("you ", stdout);
    fputs("hUNgry?\n", stdout);
    puts("This is other one");
    puts("This is another one");
    puts("This is annnn one");
    fclose(prueba);
}