#include "global.h"
#include <curses.h>

void nightswatch(char *argv[])
{
	long int waiting;
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
			long int p = 1;
			long int num = 0;
			for (long int  i = 0; i < strlen(argv[2]); i++)
			{
				num += (argv[2][strlen(argv[2]) - 1 - i] - '0') * p;
				p *= 10;
			}
			waiting = num;
		}
	}

	ll run = 0;
	ll mera = -1;
	if (!strcmp(argv[3], "interrupts"))
	{
		mera = 1;
	}
	else if (!strcmp(argv[3], "dirty"))
	{
		mera = 2;
	}

	if(mera==-1){
		printf("Invalid\n");
		return 0;
	}
	int pid = fork();
	if (pid == 0)
	{
		int x = 0;
		int y = 0;
		WINDOW *curr = initscr();
		WINDOW* newwindow = newwin(800,600,1,1);
		keypad(newwindow,true);
		noecho();
		nodelay(newwindow,true);
		curs_set(0);
		int pressed = 0;
		int it = 0;
		long int curent = time(0);
		long int baar = curent;
		while (1 && !pressed)
		{
			baar = time(0);
			if(y>80){
				y = 0;
			}
			if(wgetch(newwindow)=='q'){
				wclear(newwindow);
				break;
			}
			if((time(0)-curent)==waiting && !pressed && baar!=curent)
			{
				if(mera == 2)
				{
					char meminfopath[5000] = "/proc/meminfo";
					y++;
					curent = time(0);
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
					mvwaddstr(newwindow,y,0,u);
					fclose(fd);
				}
				else if(mera==1)
				{
					char interruptpath[5000] = "/proc/interrupts";
					int fd = open(interruptpath, O_RDONLY);
					int wfd = open("./A.txt",O_CREAT|O_WRONLY,0777);
					int a = lseek(fd, 0, SEEK_END);
					char u[BUFFER_SIZE];
					fgets(u,BUFFER_SIZE,fd );
					int cnt = 0;
					while (cnt!=2)
					{
						cnt++;
						ll a = fgets(u, BUFFER_SIZE, fd);
					}

					write(wfd,u,strlen(u));
					mvwaddstr(newwindow,y,0,u);
					close(fd);
					y+=1;
				}
				wrefresh(newwindow);
			}
		}
		noecho();
		curs_set(1);
		endwin();
		exit(0);
	}
	else if (pid < 0)
	{
		perror("Error forking");
	}
	else
	{
		int status;
		int cc;
		;do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

