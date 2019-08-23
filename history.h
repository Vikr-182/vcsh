#include "global.h"

void history_vcsh(int argc, char *argv, ll flag)
{
    if (flag == 0)
    {
        char HISTORY[3000];
        for (ll i = 0; i < lengthofhomedirectory; i++)
        {
            HISTORY[i] = homedirectory[i];
        }
        ll o = lengthofhomedirectory + 1;
        HISTORY[o - 1] = '/';
        HISTORY[o] = '.';
        HISTORY[o + 1] = 'v';
        HISTORY[o + 2] = 'c';
        HISTORY[o + 3] = 's';
        HISTORY[o + 4] = 'h';
        HISTORY[o + 5] = '_';
        HISTORY[o + 6] = 'h';
        HISTORY[o + 7] = 'i';
        HISTORY[o + 8] = 's';
        HISTORY[o + 9] = 't';
        HISTORY[o + 10] = 'o';
        HISTORY[o + 11] = 'r';
        HISTORY[o + 12] = 'y';
        HISTORY[o + 13] = '\0';
        printf("|%s|path\n", HISTORY);
        FILE *fd = fopen(HISTORY, "a+");
        if (fd == 0)
        {
            printf("Can't open file\n");
        }

//          Put command into history file
        fputs(buffer,fd);
        fputs("\n", fd);

    }
}