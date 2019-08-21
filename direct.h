#include "global.h"
#include "ls.h"
#include "prompt.h"
#include "readline.h"
#include "cd.h"
#include "clear.h"

void redirect(char *argv)
{
    if (!argv)
    {
        return;
    }
    // printf("Yay\n");
    char **tokens = (char **)malloc(sizeof(char *) * MAX_TOKENS);
    char *line = (char *)malloc(sizeof(char) * (MAX_TOKENS));
    line = strtok(argv, " ");
    ll i = 0;
    while (line != NULL)
    {
        tokens[i++] = line;
        line = strtok(NULL, " ");
    }
    ll n = 0;
    for (ll n = 0; n < i; n++)
    {
        printf("|%s|",tokens[n]);
    }
    printf("\n");
    // printf("|%s|", tokens[n]);
    if (tokens[n][0] == 'l' && tokens[n][1] == 's')
    {
        // Execute ls
    }
    else if (tokens[n][0] == 'c' && tokens[n][1] == 'd')
    {
        // Execute cd
        cd_vcsh(tokens);
    }
    else if (tokens[n][0] == 'e' && tokens[n][1] == 'c' && tokens[n][2] == 'h' && tokens[n][3] == 'o')
    {
        // Execute echo
    }
    else if (tokens[n][0] == 'p' && tokens[n][1] == 'i' && tokens[n][2] == 'n' && tokens[n][3] == 'f' && tokens[n][4] == 'o')
    {
        // Execute pinfo
    }
    else if (tokens[n][0] == 'p' && tokens[n][1] == 'w' && tokens[n][2] == 'd')
    {
        // Execute pwd
    }
    else if (tokens[n][0] == 'n' && tokens[n][1] == 'i' && tokens[n][2] == 'g' && tokens[n][3] == 'h' && tokens[n][4] == 't' && tokens[n][5] == 's' && tokens[n][6] == 'w' && tokens[n][7] == 'a' && tokens[n][8] == 't' && tokens[n][9] == 'c' && tokens[n][10] == 'h')
    {
        // Execute nightswatch
    }
    else if (tokens[n][0] == 'h' && tokens[n][1] == 'i' && tokens[n][2] == 's' && tokens[n][3] == 't' && tokens[n][4] == 'o' && tokens[n][5] == 'r' && tokens[n][6] == 'y')
    {
        // Execute history
    }
    else if (tokens[n][0] == 'c' && tokens[n][1] == 'l' && tokens[n][2] == 'e' && tokens[n][3] == 'a' && tokens[n][4] == 'r')
    {
        // Execute clear
        clear();
    }
     else if (tokens[n][0] == 'r' && tokens[n][1] == 'e' && tokens[n][2] == 's' && tokens[n][3] == 'e' && tokens[n][4] == 't')
    {
        // Execute reset
        reset();
    }
}

void shell_loop()
{
    char *c = getcwd(homedirectory, sizeof(homedirectory));
    if (!c)
    {
        perror("Error getting the current directory");
        exit(EXIT_FAILURE);
    }
    printf(ANSI_CLEAR_SCREEN);
    lengthofhomedirectory = strlen(homedirectory);
    for (ll n = 0; n < lengthofhomedirectory; n++)
    {
        past_present_directory[n] = homedirectory[n];
    }
    while (1)
    {
        // SET PWD
        char *c = getcwd(present_directory, sizeof(present_directory));
        if (strlen(present_directory) >= lengthofhomedirectory)
        {
            char PP[65536];
            ll fg = 0;
            for (ll n = 0; !fg && n < lengthofhomedirectory; n++)
            {
                if (present_directory[n] != homedirectory[n])
                {
                    fg = 1;
                }
            }
            if (!fg)
            {
                PP[0] = '~';
                for (ll n = lengthofhomedirectory; n < strlen(present_directory); n++)
                {
                    PP[n - lengthofhomedirectory + 1] = present_directory[n];
                }
                PP[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
                for (ll n = 0; n < strlen(present_directory) - lengthofhomedirectory + 1; n++)
                {
                    present_directory[n] = PP[n];
                }
                present_directory[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
            }
        }

        // DISPLAY PROMPT
        prompt_display();

        // TAKE INPUT
        buffer = readline();

        // PARSE THE INPUT
        char **totalcommands = parse(buffer);

        for (ll n = 0; n <= NUM_COMMANDS; n++)
        {
            // printf("%s \n",totalcommands[n]);
            redirect(totalcommands[n]);
            printf("\n\n");
            // free(totalcommands[n]);
        }
    }
}
