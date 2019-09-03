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

// ll pressedkey = 0;

ll NUM_COMMANDS;
ll reversemapping[3000000];
ll bgind;
ll procaarray[30000];

void signal_handler();

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

	if (!strcmp(tokens[n], "vcsh"))							//			invoking shell
	{
		tokens[n] = (char *)malloc(sizeof(char) * 10);
		strcpy(tokens[n], "./a.out"); 
	}
	if (!strcmp(tokens[n], "cd"))							// 			execute cd
	{
		cd_vcsh(tokens); 				
	}
	else if (!strcmp(tokens[n], "quit"))						// 			execute exit
	{
		quit(); 	
	}

	else if (!strcmp(tokens[n], "pwd"))
	{
		pwd_vcsh(); // 				execute pwd
	}
	else if (!strcmp(tokens[n], "echo"))
	{
		echo_vcsh(i - 1, tokens); //                    execute echo
	}
	else if (!strcmp(tokens[n], "pinfo"))
	{
		pinfo_vcsh(tokens,parentid); //                  execute cd
	}
	else if (!strcmp(tokens[n], "pwd"))
	{
		pwd_vcsh(); //                  execute pwd
	}
	else
	{

		ll askedinbg = 0;
		ll findind = 0;
		for (ll n = 0; n < i && !askedinbg; n++)
		{
			if (tokens[n][0] == '&' && strlen(tokens[n]) == 1)
			{
				askedinbg = 1; 
				findind = n;
			}
		}
		ll gh = 0;
		ll find;
		for (ll o = 0; !gh && o < strlen(argv); o++)
		{
			if (argv[o] == '&')
			{
				gh = 1;
				find = o;
			}
		}
		if (gh)
		{
			argv[find] = '\0';
		}
		if (askedinbg)
		{
			for (ll j = findind; j < i; j++)
			{
				tokens[j] = NULL;
			}
			i = findind;
		}
		if (!askedinbg) 
		{
			//			asked to execute in foreground
			signal(SIGCHLD, signal_handler);
			int pid = fork();
			if (pid == 0)
			{
				if (!strcmp(tokens[n], "ls"))
				{
					ls_vcsh(i - 1, tokens); // 				execute ls
				}
				else if (!strcmp(tokens[n], "nightswatch"))
				{
					nightswatch(tokens); //                         execute nightswatch
				}
				else if(!strcmp(tokens[n],"jobs"))
				{
					showjobs();
				}
				else if (!strcmp(tokens[n], "history"))
				{
					char F[200]; //                         execute history
					ll ind = 0;
					for (ll q = 0; q < i; q++)
					{
						for (ll p = 0; p < strlen(tokens[q]); p++)
						{
							F[ind++] = tokens[q][p];
						}
						F[ind++] = ' ';
					}
					F[ind] = '\0';
					// printf("%s\n",F);ls

					history_vcsh(i, F, 1);
				}
				else if(!strcmp(tokens[n],"cronjob"))
				{
					vcsh_cronjob(tokens);
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
			//			execute in background
			signal(SIGCHLD, signal_handler);
			printf("%s\n", argv);
			for (ll i = 0; i < strlen(argv); i++)
			{
				characterarray[bgind][i] = argv[i];
			}
			int pid = fork();
			suspendedjobnumber++;
			if (pid == 0)
			{
				setpgid(0, 0);
				if (!strcmp(tokens[n], "ls"))
				{
					ls_vcsh(i - 1, tokens); // 				execute ls
				}
				else if (!strcmp(tokens[n], "pwd"))
				{
					pwd_vcsh(); // 				execute pwd
				}
				else if (!strcmp(tokens[n], "echo"))
				{
					echo_vcsh(i - 1, tokens); //                    execute echo
				}
				else if (!strcmp(tokens[n], "pinfo"))
				{
					pinfo_vcsh(tokens,parentid); //                  execute cd
				}
				else if (!strcmp(tokens[n], "pwd"))
				{
					pwd_vcsh(); //                  execute pwd
				}
				else if (!strcmp(tokens[n], "nightswatch"))
				{
					nightswatch(tokens); //                         execute nightswatch
				}
				else if(!strcmp(tokens[n],"jobs"))
				{
					showjobs();
				}
				else if (!strcmp(tokens[n], "history"))
				{
					char F[200]; //                         execute history
					ll ind = 0;
					for (ll q = 0; q < i; q++)
					{
						for (ll p = 0; p < strlen(tokens[q]); p++)
						{
							F[ind++] = tokens[q][p];
						}
						F[ind++] = ' ';
					}
					F[ind] = '\0';
					// printf("%s\n",F);ls

					history_vcsh(i, F, 1);
				}
				else if(!strcmp(tokens[n],"cronjob"))
				{
					vcsh_cronjob(tokens);
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
				procaarray[bgind] = pid;
				bgind++;
				strcpy(characterarray[pid], argv);
				int status;
				printf("Done %lld\n", suspendedjobnumber);
				return;
			}
		}
	}
	free(tokens); // 			Free up the memory
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
	char copystring[100] = "/.vcsh_history";
	for(ll y=0;y<15;y++){
		HISTORYY[y+o-1] = copystring[y];
	}
	printf("*******  Welcome to vcsh  \u263A  ***************\n");
	signal(SIGCHLD, signal_handler);
	rl_bind_key('\t',rl_complete);
	parentid = getpid();
	ll cl = 0;
	ll beforepressed = 0;
	while (1)
	{
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
			//printf("\r%s",buffer);
		}
		else if(!pressedkey)
		{
			history_vcsh(2, buffer, 0);
		}
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
			signal(SIGCHLD, signal_handler);
			setpwd();
			resize();
		}
		pressedkey = 0;

	}
	return EXIT_FAILURE;
}

void signal_handler()
{
	int c;
	ll mil = 0;
	ll io = 0;
	for (ll i = 0; !mil && i < bgind; i++)
	{
		int r = waitpid(procaarray[i], &c, WNOHANG | WUNTRACED);
		resize();
		if (procaarray[i] == r)
		{
			if (WIFEXITED(c))
			{
				resize();
				printf("|%s|%lld exited normally with status %lld\n", characterarray[r], procaarray[i], c);
			}
			else if (WIFSIGNALED(c))
			{
				resize();
				printf("|%s|%lld exited with status %lld\n", characterarray[r], procaarray[i], c);
			}
			mil = 1;
			io = i;
		}
	}
	//signal(SIGCHLD, signal_handler);
}
