#include <stdio.h>

#define PI 3.141519

float areaTriangulo(float base, float altura);
float areaRectangulo(float base, float altura);
float areaCuadrado(float lado);
float areaCirculo(float radio);

int main(int argc, char* argv[]) {
    // Comprobacion de argumentos
    if (argc != 1) {
        fprintf(stderr, "Uso: %s\n", argv[0]);
        return 1;
    }

    int option = 0;
    do {
        fprintf(stdout, "1. Area del triangulo\n2. Area del rectangulo\n3. Area del cuadrado\n4. Area del circulo\n5. Salir\nElija una opcion: ");
        
        scanf("%d", &option);

        switch (option) {
            case 1:
            {
                float base, altura;
                fprintf(stdout, "Introduce la base y la altura del triangulo: ");

                if (scanf("%f %f", &base, &altura) != 2) {
                    fprintf(stderr, "\nEntrada no valida\n\n");
                    break;
                }

                fprintf(stdout, "\nEl area es: %.2f\n\n", areaTriangulo(base, altura));
                break;
            }
            
            case 2:
            {
                float base, altura;
                fprintf(stdout, "Introduce la base y la altura del rectangulo: ");

                if (scanf("%f %f", &base, &altura) != 2) {
                    fprintf(stderr, "\nEntrada no valida\n\n");
                    break;
                }

                fprintf(stdout, "\nEl area es: %.2f\n\n", areaRectangulo(base, altura));
                break;
            }
            
            case 3:
            {
                float lado;
                fprintf(stdout, "Introduce el lado del cuadrado: ");

                if (scanf("%f", &lado) != 1) {
                    fprintf(stderr, "\nEntrada no valida\n\n");
                    break;
                }

                fprintf(stdout, "\nEl area es: %.2f\n\n", areaCuadrado(lado));
                break;
            }
                
            case 4:
            {
                float radio;
                fprintf(stdout, "Introduce el radio del circulo: ");

                if(scanf("%f", &radio) != 1) {
                    fprintf(stderr, "\nEntrada no valida\n\n");
                    break;
                }

                fprintf(stdout, "\nEl area es: %.2f\n\n", areaCirculo(radio));
                break;
            }

            case 5:
            {
                fprintf(stdout, "\nSaliendo...\n\n");
                break;
            }
                
            default:
                fprintf(stderr, "\nIntroduce una opcion correcta!!!\n\n");
        }
    }  while (option != 5);

    return 0;
}

// Funciones de areas:
float areaTriangulo(float base, float altura) {
    return ((base * altura) / 2);
}

float areaRectangulo(float base, float altura) {
    return (base * altura);
}

float areaCuadrado(float lado) {
    return (lado * lado);
}

float areaCirculo(float radio) {
    return (PI * radio * radio);
}