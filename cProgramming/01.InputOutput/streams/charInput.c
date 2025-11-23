#include <stdio.h>
int main ()
{
    FILE* prueba = fopen("banana.txt", "a");
    char vous[3];
    char* nous = fgets(vous, 3, prueba);
    printf("%s", nous);
    fclose(prueba);
}