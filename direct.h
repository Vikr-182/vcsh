#include "global.h"
#include "ls.h"
#include "prompt.h"
#include "readline.h"
#include "cd.h"
#include "clear.h"
#include "history.h"
#include "pinfo.h"

ll pressedkey = 0;
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
	/*for (ll n = 0; n < i; n++)
	{
		printf("|%s|", tokens[n]);
	}
	printf("\n");
	printf("->|%s|<-", tokens[0]);*/
	// int fd = open("history.txt",O_CREAT|O_RDWR);
	

	if (tokens[n][0] == 'c' && tokens[n][1] == 'd')
	{
		// 			Execute cd
		cd_vcsh(tokens);
		// history_vcsh(i-1,tokens,0);
	}
	else if (tokens[n][0] == 'e' && tokens[n][1] == 'c' && tokens[n][2] == 'h' && tokens[n][3] == 'o')
	{
		// 			Execute echo
		echo_vcsh(i - 1, tokens);
		// history_vcsh(i-1,tokens,0);

	}
	else if (tokens[n][0] == 'p' && tokens[n][1] == 'i' && tokens[n][2] == 'n' && tokens[n][3] == 'f' && tokens[n][4] == 'o')
	{
		// 			Execute pinfo
		pinfo_vcsh(tokens);
		// history_vcsh(i-1,tokens,0);

	}
	else if (tokens[n][0] == 'p' && tokens[n][1] == 'w' && tokens[n][2] == 'd')
	{
		// 			Execute pwd
		pwd_vcsh();
		// history_vcsh(i-1,tokens,0);

	}
	else if (tokens[n][0] == 'n' && tokens[n][1] == 'i' && tokens[n][2] == 'g' && tokens[n][3] == 'h' && tokens[n][4] == 't' && tokens[n][5] == 's' && tokens[n][6] == 'w' && tokens[n][7] == 'a' && tokens[n][8] == 't' && tokens[n][9] == 'c' && tokens[n][10] == 'h')
	{
		// 			Execute nightswatch
	}
	else if (tokens[n][0] == 'h' && tokens[n][1] == 'i' && tokens[n][2] == 's' && tokens[n][3] == 't' && tokens[n][4] == 'o' && tokens[n][5] == 'r' && tokens[n][6] == 'y')
	{
		// 			Execute history
		// history_vcsh(i,tokens,1);
		// history_vcsh(i-1,tokens,0);

	}
	else if (tokens[n][0] == 'c' && tokens[n][1] == 'l' && tokens[n][2] == 'e' && tokens[n][3] == 'a' && tokens[n][4] == 'r')
	{
		// 			Execute clear
		clear();
		// history_vcsh(i-1,tokens,0);

	}
	// 			Execute in child process only
	else
	{
		ll askedinbg = 0;
		ll findind = 0;
		for (ll n = 0; n < i && !askedinbg; n++)
		{
			if (tokens[n][0] == '&' && strlen(tokens[n]) == 1)
			{
				// 				Asked to execute in background
				askedinbg = 1;
				findind = n;
			}
		}
		if (!askedinbg)
		{
//			Execute in foreground
			printf("->start");
			int pid = fork();
			if (pid == 0)
			{
				if (tokens[n][0] == 'l' && tokens[n][1] == 's')
				{
					// 				Execute ls
					ls_vcsh(i - 1, tokens);
					// history_vcsh(i-1,tokens,0);

				}
				else if (tokens[n][0] == 'p' && tokens[n][1] == 'w' && tokens[n][2] == 'd')
				{
					// 				Execute pwd
					pwd_vcsh();
					// history_vcsh(i-1,tokens,0);

				}
				else
				{
					// 				Execute command
					if (execvp(tokens[0], tokens) == -1)
					{
						for (ll n = 0; n < i; n++)
						{
							printf("|%s|", tokens[i]);
						}
						// history_vcsh(i-1,tokens,0);
						perror("Erroor executing or wrong command\n");
					}
				}
				// printf("end %lld<-\n",getpid());
				exit(EXIT_FAILURE);
			}
			else if (pid < 0)
			{
				perror("Error forking");
			}
			else
			{
				printf("ho\n");
				int status;
				do
				{
					waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				// printf("-<end of proc %lld %lld %lld\n",WIFEXITED(status),WIFSIGNALED(status),getpid());
			}
		}
		else
		{
			//			Execute in background
			int pid = fork();
			if (pid == 0)
			{
				if (tokens[n][0] == 'l' && tokens[n][1] == 's')
				{
					// 				Execute ls
					ls_vcsh(i - 1, tokens);
				}
				else if (tokens[n][0] == 'p' && tokens[n][1] == 'w' && tokens[n][2] == 'd')
				{
					// 				Execute pwd
					pwd_vcsh();
				}
				else
				{
					// 				Execute command
					if (execvp(tokens[0], tokens) == -1)
					{
						for (ll n = 0; n < i; n++)
						{
							printf("|%s|", tokens[i]);
						}

						perror("Erroor executing or wrong command\n");
					}
				}
				exit(EXIT_FAILURE);
			}
			else if (pid < 0)
			{
				perror("Error forking");
			}
			else
			{
				printf("hi\n");
				int status;
				do
				{
					waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
				// exit(0);
			}
		}
	}

	// 			Free up the memory
	free(tokens);
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

	while (1)
	{
		// 			SET PWD
		char *c = getcwd(present_directory, sizeof(present_directory));
		// printf("Got %s\n",present_directory);
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
		history_vcsh(2,buffer,0);

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
			// printf("Got %s\n",present_directory);
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
