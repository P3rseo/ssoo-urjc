#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("prueba.txt", O_RDONLY);
    if (fd == -1)
    {
        // Handle error
        return 1;
    }
    char buffer[100];

    read(fd, buffer, 100);
    close(fd);

    printf("Contents of the file: %s\n", buffer);
    return 0;
} 