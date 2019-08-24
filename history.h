#include "global.h"

void history_vcsh(int argc, char *argv, ll flag)
{
    if (flag == 0)
    {
        FILE *fd = fopen(HISTORYY, "a+");
        if (fd == 0)
        {
            printf("Can't open .vcsh_history\n");
        }

        //          Put command into history file
        fputs(buffer, fd);
        fputs("\n", fd);
        fclose(fd);
    }
    else
    {

        //          Extract top num commands
        FILE *fd = fopen(HISTORYY, "r");
        if (!fd)
        {
            perror("Can't open .vcsh_history");
        }
        char **tokens = (char **)malloc(sizeof(char *) * MAX_TOKENS);
        for (ll o = 0; o < MAX_TOKENS; o++)
        {
            tokens[o] = NULL;
        }
        char *line = (char *)malloc(sizeof(char) * (MAX_TOKENS));
        line = strtok(argv, " ");
        ll i = 0;
        while (line != NULL)
        {
            tokens[i++] = line;
            line = strtok(NULL, " ");
        }
        if (tokens[2] != NULL)
        {
            printf("Please do not provide multiple arguments\n");
        }
        ll p = 1;
        ll num = 0;
        if (tokens[1])
        {
            for (ll i = 0; i < strlen(tokens[1]); i++)
            {
                num += (tokens[1][strlen(tokens[1]) - 1 - i] - '0') * p;
                p *= 10;
            }
        }
        ll cnt = 0;
        size_t len;
        char *u = NULL;
        ll a = getline(&u, &len, fd);
        ll final = num > 10 ? 10 : num;
        if (tokens[1] == NULL)
        {
            final = 10;
        }
        char BUF[BUFFER_SIZE][BUFFER_SIZE];
        while (a != -1)
        {
            strcpy(BUF[cnt], u);
            cnt++;
            a = getline(&u, &len, fd);
        }
        for (ll l = cnt - final; l < cnt; l++)
        {
            printf("%s\n", BUF[l]);
        }
        fclose(fd);
    }
}