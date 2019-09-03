#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <termios.h>
#include <curses.h>
#include <readline/readline.h>
#include <readline/history.h>


typedef long long ll;

extern int errno; 

#define BUFFER_SIZE 512
#define MAX_COMMANDS 100
#define MAX_TOKENS 10

#define ANSI_RESET "\x1b[0m"
#define ANSI_BLACK "\x1b[30m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_WHITE "\x1b[38m"
#define ANSI_RED_BG "\x1b[41m"
#define ANSI_GREEN_BG "\x1b[42m"
#define ANSI_YELLOW_BG "\x1b[43m"
#define ANSI_BLUE_BG "\x1b[44m"
#define ANSI_PURPLE_BG "\x1b[45m"
#define ANSI_DARKBLUE_BG "\x1b[46m"
#define ANSI_GREY_BG "\x1b[47m"
#define ANSI_CLEAR_SCREEN "\e[1;1H\e[2J"
#define ANSI_RESET_SCREEN "\033\143"

char str[100];
char homedirectory[65536];
char hostname[65536];
char username[65536];
char present_directory[65536];
char past_present_directory[65536];
char *buffer;

ll lengthofhomedirectory;
ll lengthofpresentdictionary;
extern ll NUM_COMMANDS ;
extern ll reversemapping[3000000];
extern ll bgind;
extern ll procaarray[30000];
char characterarray[10000][100];

char HISTORY[20][500];
ll commandnumber ;
ll suspendedjobnumber ;
ll jobs[3000][2] ;
ll naam;
int pressedkey = 0;
int numpressed = 0;
char HISTORYY[3000];
int parentid ;
// ll backgroundflag = 0;
// ll foregroundflag = 0;
