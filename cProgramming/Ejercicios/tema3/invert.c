#include <stdio.h>
#include <string.h>

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("ERROR. Uso incorrecto de ivnert\n");
        return 1;
    }

    for (int i = (argc - 1); i > 0; i--)
    {
        for (int j = (strlen(argv[i]) - 1); j >= 0; j--)
        {
            printf("%c", argv[i][j]);
        }
        printf(" ");
    }

    printf("\n");  
    
    return 0;
}

/*
for (int i = argc; i > 1; i--)
    {
        for (int j = strlen(argv[i-1]); j--)
        {
            printf("%c", argv[i-1][j]);
        }
        printf(" ");
    }
*/