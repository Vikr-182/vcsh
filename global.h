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

// PROMPT INFORMATION
char str[100];
char homedirectory[65536];
char hostname[65536];
char username[65536];
char present_directory[65536];
char past_present_directory[65536];
ll lengthofhomedirectory;
ll lengthofpresentdictionary;

// COMMAND INFORMATION
int pressedkey = 0;
int numpressed = 0;
ll NUM_PIPES;
extern ll NUM_COMMANDS ;
char *buffer;
char HISTORYY[3000];
char HISTORY[20][500];
char curr_command[512];

// PROCESS INFORMATION
extern ll reversemapping[3000000];
extern ll bgind;
extern ll procaarray[30000];
ll commandnumber ;
ll suspendedjobnumber ;
ll jobs[3000][2] ;
ll nikal;
ll naam;
pid_t pid_of_shell;
pid_t parentid ;
int shellid;
int glob_out;
int glob_in;
int save_out;
int save_in;
char characterarray[10000][100];
// ll backgroundflag = 0;
// ll foregroundflag = 0;
