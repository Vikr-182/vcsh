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

        // If successfully changed then update past directory
        for (ll n = 0; n < lengthofhomedirectory; n++)
        {
            past_present_directory[n] = present_directory[n];
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
        A[strlen(argv[1]) + strlen(homedirectory)] = '/';
        A[strlen(argv[1]) + strlen(homedirectory) + 1] = '\0';

        printf("|%s|\n", A);
        // ll b = chdir(A);
        ll b = chdir(homedirectory);
        // printf("Yay %d\n", b);
        if (b == -1)
        {
            perror("Unaable to change directory");
            return (EXIT_FAILURE);
        }

        // If was able to successfully cd
        for (ll n = 0; n < lengthofpresentdictionary; n++)
        {
            past_present_directory[n] = present_directory[n];
        }
        return 1;
    }

    if (argv[1][0] == '-' && strlen(argv[1]) == 1)
    {
        // go back to previous directory
        printf("%s\n",past_present_directory);
        ll g = chdir(past_present_directory);
        if(g==-1){
            perror("Error going back to past directory");
            return EXIT_FAILURE;
        }
        for (ll n = 0; n < lengthofpresentdictionary; n++)
        {
            past_present_directory[n] = present_directory[n];
        }
    }
    int f = chdir(argv[1]);
    if (f == -1)
    {
        perror("Unable to change directory");
        return (EXIT_FAILURE);
    }

    // If was able to successfully cd
    for (ll n = 0; n < lengthofpresentdictionary; n++)
    {
        past_present_directory[n] = present_directory[n];
    }

    return 1;
}

void pwd_vcsh(){
    char A[65536];
    char B[65536];
    
    getcwd(A,sizeof(A));
    ll fg = 0;
    // printf("|%s|\n",A);
    if(strlen(A)>=strlen(homedirectory)){
        for (ll n = 0; n < lengthofhomedirectory; n++)
        {
            if(A[n]!=homedirectory[n]){
                fg = 1;
            }
        }
    }
    else{
        fg = 1;
    }
    if(!fg){
        B[0] = '~';
        B[1] = '/';
        // printf("Hi\n");
        for (ll n = 2; n < strlen(A)-strlen(homedirectory)+1; n++)
        {
            B[n] = A[n+lengthofhomedirectory-1];
        }
        B[strlen(A)-strlen(homedirectory)+1] = '\0';
        printf("%s\n",B);
    }
    else{
        printf("%s\n",A);
    }
}