#include "global.h"

void nightswatch(char *argv[])
{
    ll waiting;
    if (argv[1][0] != '-')
    {
        waiting = 1;
    }
    else
    {
        // Convert string into number
        if (argv[1][1] != 'n')
        {
            printf("Please provide correct flag\n");
            waiting = 1;
        }
        else
        {
            ll p = 1;
            ll num = 0;
            for (ll i = 0; i < strlen(argv[1]); i++)
            {
                num += (argv[1][strlen(argv[1]) - 1 - i] - '0') * p;
                p *= 10;
            }
            waiting = num;
        }
    }

    ll run = 0;
    if (!strcmp(argv[3], "interrupt"))
    {
        char interruptpath[5000] = "/proc/interrupts";

        int pid = fork();
        if (pid == 0)
        {
            // Execute this
            FILE *fd = fopen(interruptpath, "r");
            fclose(fd);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Error forking");
        }
        else
        {
            int status;
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status) && !run);
        }
    }
    else if (!strcmp(argv[3], "dirty"))
    {
        char meminfopath[5000] = "/proc/meminfo";

        int pid = fork();
        if (pid == 0)
        {
            FILE *fd = fopen(meminfopath, "r");
            fclose(fd);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Error forking");
        }
        else
        {
            int status;
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status) && !run);
        }
    }
}