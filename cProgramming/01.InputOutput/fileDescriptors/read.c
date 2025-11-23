#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fd1;
    if ((fd1 = open("prueba.txt", O_RDONLY)) == -1)
    {
        printf("Error al abrir el open de fd1\n");
        return 1;
    }
    else
    {
        printf("El valor de fd1 es %d\n", fd1);
    }

    char buf[250];
    ssize_t readComp;
    if ((readComp = read(fd1, buf, 250)) == -1)
    {
        printf("Fallo al ejecutar el read de fd1\n");
    }
    else
    {
        printf("Lo que devuelve el read de fd1 es: %ld\n", readComp);
    }



    int fd2;
    if ((fd2 = open("otraPrueba.txt", O_RDWR)) == -1)
    {
        printf("Fallo al abrir otraPrueba.txt\n");
        return 2;
    }
    else
    {
        printf("El valor de fd2 es: %d\n", fd2);
    }
    
    ssize_t nr;
    char buffer[256];
    while (1) {
        if ((nr = read(fd1, buffer, 256)) == -1)
        {
            printf("Se ha producido un error en el read de nr\n");
            return 3;
        }
        else
        {
            printf("El valor de nr es: %ld\n", nr);
        }

        if (readComp > 0) 
        {
            write(fd2, buf, readComp);
            printf("Se ha escrito dentro de fd2\n");
        } 
        else
        {
            break;  // nr == 0 (EOF) o nr == -1 (error)
        }
    }

    close(fd1);
    close(fd2);
    
    return 0;
}