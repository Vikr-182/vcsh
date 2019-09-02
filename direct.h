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
			int pid = fork();
			if (pid == 0)
			{
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
					pinfo_vcsh(tokens); //                  execute cd
				}
				else if (!strcmp(tokens[n], "pwd"))
				{
					pwd_vcsh(); //                  execute pwd
				}
				else if (!strcmp(tokens[n], "nightswatch"))
				{
					nightswatch(tokens); //                         execute nightswatch
				}
				else if(!strcmp(tokens[n],"showjobs"))
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
					pinfo_vcsh(tokens); //                  execute cd
				}
				else if (!strcmp(tokens[n], "pwd"))
				{
					pwd_vcsh(); //                  execute pwd
				}
				else if (!strcmp(tokens[n], "nightswatch"))
				{
					nightswatch(tokens); //                         execute nightswatch
				}
				else if(!strcmp(tokens[n],"showjobs"))
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
	printf("*******  Welcome to vcsh  \u263A  ***************\n");
	// printf("%s\n",HISTORYY);
	signal(SIGCHLD, signal_handler);

	rl_bind_key('\t',rl_complete);
	while (1)
	{
		// 			SET PWD
		setpwd();

		// 			DISPLAY PROMPT
		prompt_display();

		// 			TAKE INPUT
		buffer = input();
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
			setpwd();
			resize();
		}
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
	signal(SIGCHLD, signal_handler);
}
