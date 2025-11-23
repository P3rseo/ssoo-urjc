#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    
    int fd1 = open("prueba.txt", O_RDONLY);  // Creamos el descriptor de ficheros y lo abrimos con solo lectura.

    if (fd1 == -1)  // Si hay algun error con open, el descriptor (fd) vale -1.
    {
        printf("Error with open\n");
        return 1;
    }
    else  // Si vale distinto de -1 es por que se le ha asignado el fd.
    {
        printf("El valor del fd es %d\n", fd1);
    }

    close(fd1);  // Cerramos el fd despues de utilizarlo.
    // *****************************************************


    /*
    O_CREAT - Crear si no existe.
    O_TRUNC - Si existe, vaciarlo.
    */
    int fd2 = open("otraPrueba.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);   // Creamos otro fd de manera distinta.

    if (fd2 == -1)  // Si fd2 vale -1 es por que algo ha salido mal con open.
    {
        printf("Error con el segundo open\n");
        return 2;
    }
    else  // Si devuelve un entero es porque open ha salido bien.
    {
        printf("El valor del fd es: %d\n", fd2);
    }
    
    char *msg = "Hola Carmen\nratilla";  // Creamos un mensaje para insertar en "otraPrueba.txt".
    if (write(fd2, msg, 19) == -1)  // Escribimos en el fd2, el mensaje msg y el tamanio de msg.
    {
        printf("Error con el write de fd2\n");
        return 2;
    }

    close(fd2);
    // ***********************************************************


    int fd3 = creat("otraPrueba.txt", 0600);

    if (fd3 == -1)
    {
        printf("Error con creat\n");
        return 3;
    }
    else
    {
        printf("El valor del fd es: %d\n", fd3);
    }

    if (write(fd3, "creado\n", 7) == -1)
    {
        printf("Error con write de fd3\n");
        return 3;
    }

    close(fd3);

    write(STDOUT_FILENO, "HELLO WORLD!\n", 14);

    return 0;
}