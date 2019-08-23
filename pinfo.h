#include "global.h"

void pinfo_vcsh(char *argv[])
{
    if (argv[2] != NULL)
    {
        printf("Please provide 2 arguments\n");
    }
    if (argv[1] == NULL)
    {
        //         Current process in the info need to print about
        pid_t pid = getpid();
        if (pid < 0)
        {
            perror("Getting pid");
            return;
        }
        printf("Process id --- %d\n", pid);
        char f[30000];
        char path[300] = "/proc/";
        ll num = pid;
        ll find = 0;
        while (num)
        {
            f[find++] = num % 10 + '0';
            num = num / 10;
        }
        for (ll i = 0; i < find / 2; i++)
        {
            char temp;
            temp = f[i];
            f[i] = f[find - 1 - i];
            f[find - 1 - i] = temp;
        }
        f[find] = '\0';
        ll len = strlen(f);
        printf("|%s|\n", f);
        for (ll i = 6; i < len + 6; i++)
        {
            path[i] = f[i - 6];
        }
        path[len + 6] = '\0';
        char status[3000], execpath[3000];
        for (ll i = 0; i < strlen(path); i++)
        {
            status[i] = path[i];
            execpath[i] = path[i];
        }
        status[len + 6] = '/';
        status[len + 7] = 's';
        status[len + 8] = 't';
        status[len + 9] = 'a';
        status[len + 10] = 't';
        status[len + 11] = 'u';
        status[len + 12] = 's';
        status[len + 13] = '\0';
        execpath[len + 6] = '/';
        execpath[len + 7] = 'e';
        execpath[len + 8] = 'x';
        execpath[len + 9] = 'e';
        execpath[len + 10] = '\0';

        // printf("|%s|%s|\n", status, execpath);
        FILE *fd = fopen(status, "r");
        if (!fd)
        {
            perror("No such process");
            return;
        }

        char B[BUFFER_SIZE];
        ll cnt = 0;
        while (cnt < 2)
        {
            fgets(B, BUFFER_SIZE, (FILE *)fd);
            cnt++;
        }
        printf("Process %s", B);

        while (cnt < 12)
        {
            fgets(B, BUFFER_SIZE, (FILE *)fd);
            cnt++;
        }
        ll ind;
        for (ll i = 0; i < strlen(B); i++)
        {
            if (B[i] == ':')
            {
                ind = i;
                break;
            }
        }
        // printf("%s\n",B);
        printf("Memory: ");
        for (ll i = ind; B[i] != 'B' && i < strlen(B); i++)
        {
            printf("%c", B[i]);
        }
        printf("B\n");

        char BUF[BUFFER_SIZE];
        int fgi = readlink(execpath, BUF, BUFFER_SIZE);
        BUF[fgi] = '\0';
        if (fgi == -1)
        {
            perror("Can't print ");
            return;
        }
        printf("Execution path: \t|%s|\n", BUF);
    }
    else
    {
        ll len = strlen(argv[1]);
        char path[300] = "/proc/";
        for (ll i = 6; i < len + 6; i++)
        {
            path[i] = argv[1][i - 6];
        }
        path[len + 6] = '\0';
        char status[3000], execpath[3000];
        for (ll i = 0; i < strlen(path); i++)
        {
            status[i] = path[i];
            execpath[i] = path[i];
        }
        status[len + 6] = '/';
        status[len + 7] = 's';
        status[len + 8] = 't';
        status[len + 9] = 'a';
        status[len + 10] = 't';
        status[len + 11] = 'u';
        status[len + 12] = 's';
        status[len + 13] = '\0';
        execpath[len + 6] = '/';
        execpath[len + 7] = 'e';
        execpath[len + 8] = 'x';
        execpath[len + 9] = 'e';
        execpath[len + 10] = '\0';
        FILE *fd = fopen(status, "r");
        if (!fd)
        {
            perror("No such process");
            return;
        }
        // printf("Hi %d\n", fd);
        if (fd < 0)
        {
            perror("Can't open process");
            return;
        }
        printf("Process id --- %s\n", argv[1]);

        char B[BUFFER_SIZE];
        ll cnt = 0;
        while (cnt < 2)
        {
            fgets(B, BUFFER_SIZE, (FILE *)fd);
            cnt++;
        }
        printf("Process %s", B);

        while (cnt < 12)
        {
            fgets(B, BUFFER_SIZE, (FILE *)fd);
            cnt++;
        }
        ll ind;
        for (ll i = 0; i < strlen(B); i++)
        {
            if (B[i] == ':')
            {
                ind = i;
                break;
            }
        }

        printf("Memory: ");
        for (ll i = ind; B[i] != 'B' && i < strlen(B); i++)
        {
            printf("%c", B[i]);
        }
        printf("B\n");
        char BUF[BUFFER_SIZE];
        int fd2 = open(execpath, O_RDONLY);
        int o = lseek(fd2, 0, SEEK_END);
        int fgi = readlink(execpath, BUF, BUFFER_SIZE);
        BUF[fgi] = '\0';
        if (fgi == -1)
        {
            perror("Can't print ");
            return;
        }
        printf("Execution path:  |%s|\n", BUF);
    }
}