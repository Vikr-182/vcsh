#include "global.h"
#include "ls.h"
#include "prompt.h"
#include "input.h"
#include "builtins.h"
#include "clear.h"
#include "history.h"
#include "pinfo.h"
#include "jobs.h"
#include "cronjob.h"
#include "execute.h"
#include "env.h"
#include "jobs_commands.h"

// ll pressedkey = 0;

ll NUM_COMMANDS;
ll reversemapping[3000000];
ll bgind;
ll procaarray[30000];


ll shell_loop()
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

	for (ll i = 0; i < lengthofhomedirectory; i++)
	{
		HISTORYY[i] = homedirectory[i];
	}
	ll o = lengthofhomedirectory + 1;
	char copystring[100] = "/.vcsh_history";
	for(ll y=0;y<15;y++){
		HISTORYY[y+o-1] = copystring[y];
	}
	printf("*******  Welcome to vcsh  \u263A  ***************\n");
	signal(SIGCHLD, signal_handler);
	signal(SIGINT,ctrlccross);
	signal(SIGTSTP,ctrlzcross);		
	signal(SIGQUIT,sigquit);

	shell_gid = getpgid(getpid());
	ll cl = 0;
	ll beforepressed = 0;
	save_out = dup(1);			// SAVE OUTPUT
	save_in = dup(0);			// SAVE INPUT
	while (1)
	{
		//			SET SHELL ID
		shellid = getpid();
		parentid = shellid;

		// 			SET PWD
		setpwd();

		// 			DISPLAY PROMPT
		prompt_display();

		// 			TAKE INPUT
		buffer = input();
		if(pressedkey)
		{
			// 					Display num command from last at prompt and expect user to hit enter
			char BUF[512][512];
			int ind = 0;
			FILE *fd = fopen(HISTORYY, "r");
			if (!fd)
			{
				perror("Can't open .vcsh_history");
			}
			char *u = NULL;
			size_t len;
			ll a = getline(&u, &len, fd);
			int cnt = 0;
			strcpy(BUF[cnt],u);
			while(a != -1)
			{
				strcpy(BUF[cnt], u);
				cnt++;
				a = getline(&u, &len, fd);
			}
			strcpy(buffer,BUF[cnt-numpressed]);
			prompt_display();
			buffer[strlen(buffer)-1] = '\0';
		}
		else if(!pressedkey)
		{
			history_vcsh(2, buffer, 0);
		}
		// 			PARSE THE INPUT
		char **totalcommands = parse_by_colon(buffer);
		for (ll n = 0; n <= NUM_COMMANDS; n++)
		{
			strcpy(curr_command,totalcommands[n]);
			char **totalpipes = parse_by_pipe(totalcommands[n]);
			// 			PIPE THE OUTPUT OF LAST TO FIRST ONE
			int fdarray[2];
			fdarray[0] = -1;
			fdarray[1] = -1;

			if(NUM_PIPES>=1)
			{
				for(int j=0;j <= NUM_PIPES; j++)
				{
					pipe(fdarray);
					int pid = fork();
					if(pid<0)
					{
						perror("Error forking");
					}
					else if(pid==0)
					{
						if(j!=NUM_PIPES)
						{
							close(1);		// Close stdout so that it is assigned to fdarray[1]
							dup(fdarray[1]);	// Now child will write to a copy of fdarray[1]
							close(fdarray[1]);	// Close the original fdarray[1]
						}
						close(0);		// Close stdin  so that it is assigned to fdarray[0]
						dup(glob_in);		// Now child will read from a copy of fdarray[0]
						close(fdarray[0]);	// Close the original fdarray[0]
						parse_it(totalpipes[j]);
						exit(1);
					}
					else
					{
						wait(NULL);
						close(fdarray[1]);		// Close fdarray[1] as only child will write to it
						glob_in = fdarray[0];  		// Make parent read from pipe
					}
				}
			}
			else
			{
				parse_it(totalcommands[n]);
			}
			signal(SIGCHLD, signal_handler);
			updatejobs();
			setpwd();
			resize();
		}
		pressedkey = 0;

		nikal = 0;
		iscronjob = 0;
	}
	return EXIT_FAILURE;
}

