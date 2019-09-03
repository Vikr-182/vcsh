
void resize()
{
    ll cnt = 0;
    FILE *fd = fopen(HISTORYY, "a+");
    if (fd == 0)
    {
        perror("History file deleted");
        return;
    }
    char **tokens = (char **)malloc(sizeof(char *) * MAX_COMMANDS);
    for (ll o = 0; o < MAX_COMMANDS; o++)
    {
        tokens[o] = NULL;
    }
    ll i = 0;
    char *u = NULL;
    size_t len;
    int a = getline(&u, &len, fd);
    while (a != -1)
    {
        tokens[i] = (char *)malloc(sizeof(char)*BUFFER_SIZE);
        strcpy(tokens[i++], u);
        a = getline(&u, &len, fd);
    }

    fclose(fd);
    ll final;
    if (i < 25)
    {
        final = i;
    }
    else
    {
        final = 25;
    }

    // Write back first 25 to same file
    FILE *fd2 = fopen(HISTORYY,"w");
    for (ll j = i - final; j < i; j++)
    {
         fputs(tokens[j],fd2);
    }
    fclose(fd2);
}

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

        char **tokens = (char **)malloc(sizeof(char *) * MAX_COMMANDS);
        for (ll o = 0; o < MAX_TOKENS; o++)
        {
            tokens[o] = NULL;
        }
        char *line = (char *)malloc(sizeof(char) * (MAX_COMMANDS));
        line = strtok(argv, " ");
        ll i = 0;
        while (line != NULL)
        {
            // tokens[i] = (char *)malloc(sizeof(char *)*BUFFER_SIZE);
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

        char BUF[512][512];
        while (a != -1)
        {
            strcpy(BUF[cnt], u);
            cnt++;
            a = getline(&u, &len, fd);
        }
        for (ll l = cnt - final; l < cnt; l++)
        {
            printf("%s", BUF[l]);
        }
        fclose(fd);
    }
}
