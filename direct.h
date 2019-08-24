#include "global.h"
#include "ls.h"
#include "prompt.h"
#include "readline.h"
#include "builtins.h"
#include "clear.h"
#include "history.h"
#include "pinfo.h"
#include "nightswatch.h"


// ll pressedkey = 0;

void signal_handler(){
	int pid = waitpid(-1,NULL,WNOHANG);
	if(pid>0){
		printf("Child successfully exited %d\n",pid);
	}
}

void redirect(char *argv)
{
	if (!argv)
	{
		return;
	}

	char **tokens = (char **)malloc(sizeof(char *) * MAX_TOKENS);
	for (ll o = 0; o < MAX_TOKENS; o++)
	{
		tokens[o] = NULL;
	}
	char *line = (char *)malloc(sizeof(char) * (MAX_TOKENS));
	line = strtok(argv, " ");
	ll i = 0;
	while (line != NULL)
	{
		tokens[i++] = line;
		line = strtok(NULL, " ");
	}
	ll n = 0;


	if(!strcmp(tokens[n],"vcsh"))
	{
		tokens[n] = (char *)malloc(sizeof(char)*10);
		strcpy(tokens[n],"./a.out");		//			invoking shell	
	}
	if (!strcmp(tokens[n],"cd"))
	{
		cd_vcsh(tokens);					// 			execute cd
	}
	else if (!strcmp(tokens[n],"echo"))
	{
		echo_vcsh(i - 1, tokens);			// 			execute echo
	}
	else if (!strcmp(tokens[n],"pinfo"))
	{
		pinfo_vcsh(tokens);					// 			execute cd
	}
	else if (!strcmp(tokens[n],"pwd"))
	{
		pwd_vcsh();							// 			execute pwd
	}
	else if (!strcmp(tokens[n],"nightswatch"))
	{
		nightswatch(tokens);				// 			execute nightswatch
	}
	else if (!strcmp(tokens[n],"history"))
	{
		char F[200];						// 			execute history
		ll ind  = 0;
		for (ll q = 0; q <i; q++)
		{
			for (ll p = 0; p < strlen(tokens[q]); p++)
			{
				F[ind++] = tokens[q][p];
			}
			F[ind++] = ' ';
		}
		F[ind] = '\0';
		// printf("%s\n",F);
		history_vcsh(i,F,1);
	}
	else if (!strcmp(tokens[n],"clear"))
	{
		clear();							// 			execute clear
	}
	else if(!strcmp(tokens[n],"quit"))
	{
		quit();								// 			execute exit
	}
	else
	{
		ll askedinbg = 0;
		ll findind = 0;
		for (ll n = 0; n < i && !askedinbg; n++)
		{
			if (tokens[n][0] == '&' && strlen(tokens[n]) == 1)
			{
				askedinbg = 1;				//			asked to execute in background
				findind = n;
			}
		}
		if(askedinbg)
		{
			for (ll j = findind; j < i; j++)
			{
				tokens[j] = NULL;
			}
			i = findind;
		}
		if (!askedinbg)						//			execute in foreground
		{
			int pid = fork();
			if (pid == 0)
			{
				if (!strcmp(tokens[n],"ls"))
				{
					ls_vcsh(i - 1, tokens); // 				execute ls	
				}
				else if (!strcmp(tokens[n],"pwd"))
				{
					pwd_vcsh();				// 				execute pwd				
				}
				else
				{
					if (execvp(tokens[0], tokens) == -1)	
					{
						perror("Erroor executing or wrong command\n");
					}
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
				do
				{
					waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		}
		else
		{	
			suspendedjobnumber++;			//				execute in background
			int pid = fork();
			if (pid == 0)
			{
				// pid_t pgrp = getpgrp();
				setpgid(0,0);
				if (!strcmp(tokens[n],"ls"))
				{
					ls_vcsh(i - 1, tokens); // 				execute ls	
				}
				else if (!strcmp(tokens[n],"pwd"))
				{
					pwd_vcsh();				// 				execute pwd				
				}
				else
				{
					if (execvp(tokens[0], tokens) == -1)
					{
						perror("Erroor executing or wrong command\n");
					}
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
				printf("Done %lld\n",suspendedjobnumber);
				signal(SIGCHLD,signal_handler);
			}
		}
	}
	free(tokens);									// 			Free up the memory
	free(line);
}


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
	HISTORYY[o - 1] = '/';
	HISTORYY[o] = '.';
	HISTORYY[o + 1] = 'v';
	HISTORYY[o + 2] = 'c';
	HISTORYY[o + 3] = 's';
	HISTORYY[o + 4] = 'h';
	HISTORYY[o + 5] = '_';
	HISTORYY[o + 6] = 'h';
	HISTORYY[o + 7] = 'i';
	HISTORYY[o + 8] = 's';
	HISTORYY[o + 9] = 't';
	HISTORYY[o + 10] = 'o';
	HISTORYY[o + 11] = 'r';
	HISTORYY[o + 12] = 'y';
	HISTORYY[o + 13] = '\0';
	// printf("%s\n",HISTORYY);
	while (1)
	{
		// 			SET PWD
		char *c = getcwd(present_directory, sizeof(present_directory));
		if (strlen(present_directory) >= lengthofhomedirectory)
		{
			char PP[65536];
			ll fg = 0;
			for (ll n = 0; !fg && n < lengthofhomedirectory; n++)
			{
				if (present_directory[n] != homedirectory[n])
				{
					fg = 1;
				}
			}
			if (!fg)
			{
				PP[0] = '~';
				for (ll n = lengthofhomedirectory; n < strlen(present_directory); n++)
				{
					PP[n - lengthofhomedirectory + 1] = present_directory[n];
				}
				PP[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
				for (ll n = 0; n < strlen(present_directory) - lengthofhomedirectory + 1; n++)
				{
					present_directory[n] = PP[n];
				}
				present_directory[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
				lengthofpresentdictionary = strlen(present_directory);
			}
			else
			{
				lengthofpresentdictionary = strlen(present_directory);
			}
		}
		lengthofpresentdictionary = strlen(present_directory);

		// 			DISPLAY PROMPT
		prompt_display();

		// 			TAKE INPUT
		buffer = readline();
		history_vcsh(2, buffer, 0);

		// 			PARSE THE INPUT
		char **totalcommands = parse(buffer);

		for (ll n = 0; n <= NUM_COMMANDS; n++)
		{
			for (ll j = 0; j < strlen(totalcommands[n]); j++)
			{
				HISTORY[commandnumber][j] = totalcommands[n][j];
			}
			HISTORY[commandnumber][strlen(totalcommands[n])] = '\0';
			commandnumber++;
			commandnumber %= 20;
			redirect(totalcommands[n]);
			char *c = getcwd(present_directory, sizeof(present_directory));
			if (strlen(present_directory) >= lengthofhomedirectory)
			{
				char PP[65536];
				ll fg = 0;
				for (ll n = 0; !fg && n < lengthofhomedirectory; n++)
				{
					if (present_directory[n] != homedirectory[n])
					{
						fg = 1;
					}
				}
				if (!fg)
				{
					PP[0] = '~';
					for (ll n = lengthofhomedirectory; n < strlen(present_directory); n++)
					{
						PP[n - lengthofhomedirectory + 1] = present_directory[n];
					}
					PP[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
					for (ll n = 0; n < strlen(present_directory) - lengthofhomedirectory + 1; n++)
					{
						present_directory[n] = PP[n];
					}
					present_directory[strlen(present_directory) - lengthofhomedirectory + 1] = '\0';
					lengthofpresentdictionary = strlen(present_directory);
				}
				else
				{
					lengthofpresentdictionary = strlen(present_directory);
				}
			}
			lengthofpresentdictionary = strlen(present_directory);
		}
	}
	return EXIT_FAILURE;
}
