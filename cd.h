#include "global.h"

ll cd_vcsh(char *argv[])
{
    // Parse from space
    if (argv[2] != NULL)
    {
        printf("Please give only 2 arguments\n");
    }
    if (argv[1] == NULL)
    {
        int a = chdir(homedirectory);
        if (a == -1)
        {
            perror("Unable to change directory");
            return (EXIT_FAILURE);
        }
        return 1;
    }

    ll g = 0;
    for (ll n = 1; n < strlen(argv[1]); n++)
    {
        if (argv[1][n] == '~')
        {
            g = 1;
        }
    }
    if (g)
    {
        printf("Invalid address\n");
        return (EXIT_FAILURE);
    }
    if (argv[1][0] == '~')
    {
        char A[BUFFER_SIZE];
        for (ll n = 0; n < strlen(homedirectory); n++)
        {
            A[n] = homedirectory[n];
        }
        for (ll n = 0; n < strlen(argv[1]); n++)
        {
            A[n + strlen(homedirectory)] = argv[1][1 + n];
        }
        A[strlen(argv[1])+strlen(homedirectory)] = '/';
        A[strlen(argv[1])+strlen(homedirectory)+1] = '\0';
        
        printf("|%s|\n",A);
        // ll b = chdir(A);
        ll b = chdir(homedirectory);
        printf("Yay %d\n",b);
        if (b == -1)
        { 
            perror("Unaable to change directory");
            return (EXIT_FAILURE);
        }
        return 1;
    }
    
    int f = chdir(argv[1]);
    if (f == -1)
    {
        perror("Unable to change directory");
        return (EXIT_FAILURE);
    }
    return 1;
}