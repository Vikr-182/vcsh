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
            while (1)
            {
                int fd = open(interruptpath, O_RDONLY);
                int a = lseek(fd, 0, SEEK_END);
                char B[4000];
                read(fd, B, a);
                // printf("%lld\n", a);
                printf("%s\n", B);
                int milli_seconds = 1000 * waiting;
                // Stroing start time
                clock_t start_time = clock();

                // looping till required time is not acheived
                while (clock() < start_time + milli_seconds)
                    ;
                close(fd);
            }
            exit(0);
        }
        else if (pid < 0)
        {
            perror("Error forking");
        }
        else
        {
            int status;
        }
    }
    else if (!strcmp(argv[3], "dirty"))
    {
        char meminfopath[5000] = "/proc/meminfo";

        int pid = fork();
        if (pid == 0)
        {
            while (1)
            {
                FILE *fd = fopen(meminfopath, "r");
                char u[BUFFER_SIZE];
                size_t len;
                ll a = fgets(u, BUFFER_SIZE, fd);
                ll r = 0;
                if (u[0] == 'D' && u[1] == 'i' && u[2] == 'r' && u[3] == 't' && u[4] == 'y')
                {
                    r = 1;
                }
                while (!r)
                {
                    ll a = fgets(u, BUFFER_SIZE, fd);
                    if (u[0] == 'D' && u[1] == 'i' && u[2] == 'r' && u[3] == 't' && u[4] == 'y')
                    {
                        r = 1;
                    }
                }
                printf("%s\n", u);
                int milli_seconds = 1000 * waiting;
                // Stroing start time
                clock_t start_time = clock();

                // looping till required time is not acheived
                while (clock() < start_time + milli_seconds)
                    ;
                fclose(fd);
            }
            exit(0);
            char A = getchar();
            int cc;
            while (A != 'q')
            {
                waitpid(pid, &cc, WNOHANG | WUNTRACED);
                A = getchar();
            }
            kill(pid, SIGKILL);
        }
        else if (pid < 0)
        {
            perror("Error forking");
        }
        else
        {
            int status;
            char A = getchar();
            int cc;
            do
            {
                waitpid(pid, &status, WUNTRACED);
                A = getchar();
            } while (!WIFEXITED(status) && !WIFSIGNALED(status) && A != 'q');
            kill(pid, SIGKILL);
        }
    }
}