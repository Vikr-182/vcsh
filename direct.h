#include "global.h"
#include "ls.h"
#include "prompt.h"
#include "input.h"
#include "builtins.h"
#include "clear.h"
#include "history.h"
#include "pinfo.h"
#include "nightswatch.h"
#include "jobs.h"
#include "cronjob.h"
#include "execute.h"
#include "env.h"

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

	rl_bind_key('\t',rl_complete);
	ll cl = 0;
	ll beforepressed = 0;
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
			printf("%s\n",buffer);
		}
		else if(!pressedkey)
		{
			history_vcsh(2, buffer, 0);
		}
		// 			PARSE THE INPUT
		char **totalcommands = parse_by_colon(buffer);
		for (ll n = 0; n <= NUM_COMMANDS; n++)
		{
			char **totalpipes = parse_by_pipe(totalcommands[n]);

		// 			PIPE THE OUTPUT OF LAST TO FIRST ONE
			int fdarray[2];
			fdarray[0] = -1;
			fdarray[1] = -1;
			pipe(fdarray);
			/*
			for(int j=0;j <= NUM_PIPES; j++)
			{
				int pid = fork();
				if(pid<0)
				{
					perror("Error forking");
				}
				else if(pid==0)
				{
					dup2(sin,0);	// Make child read from stdin
					dup2(fdarray[1],1); // Make child write to pipe
					close(fdarray[0]);
					redirect(totalpipes[j]);
					exit(2);
				}
				else
				{
					wait(NULL);
					close(fdarray[1]);
					sin = fdarray[0];  // Make parent read from pipe
				}
				printf("|%s|",totalpipes[j]);
			}
			// Restore stdout and stdin
			redirect(totalpipes[NUM_PIPES]);
			dup2(sout,1);
			dup2(sin,0);*/
			strcpy(curr_command,totalcommands[n]);
			redirect(totalcommands[n]);
			signal(SIGCHLD, signal_handler);
			updatejobs();
			setpwd();
			resize();
		}
		pressedkey = 0;

	}
	return EXIT_FAILURE;
}

