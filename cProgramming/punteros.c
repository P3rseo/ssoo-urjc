#include <stdio.h>

int main(void)
{
    float datoReal = 23.34;
    float* punteroDatoReal = &datoReal;
    float** punteroPunteroDatoReal = &punteroDatoReal;

    printf("datoReal: %f\n", datoReal);                    // valor
    printf("&datoReal: %p\n", &datoReal);           // dirección de datoReal
    printf("*punteroDatoReal: %f\n", *punteroDatoReal);    // valor apuntado
    printf("punteroDatoReal: %p\n", punteroDatoReal);
    printf("&punteroDatoReal: %p\n", &punteroDatoReal); // dirección del puntero
    printf("**punteroPunteroDatoReal: %f\n", **punteroPunteroDatoReal);
    printf("*punteroPunteroDatoReal: %p\n", *punteroPunteroDatoReal);
    printf("punteroPunteroDatoReal: %p\n", punteroPunteroDatoReal);
    printf("&punteroPunteroDatoReal: %p\n", &punteroPunteroDatoReal);
    
    
    return 0;
}