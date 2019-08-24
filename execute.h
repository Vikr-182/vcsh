#include "global.h"

// ll pressedkey = 0;
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
	if(!strcmp(tokens[n],"vcsh")){
		// execvp("./a.out","./a.out");
		tokens[n] = (char *)malloc(sizeof(char)*10);
		strcpy(tokens[n],"./a.out");
	}
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
		nightswatch(tokens);
	}
	else if (tokens[n][0] == 'h' && tokens[n][1] == 'i' && tokens[n][2] == 's' && tokens[n][3] == 't' && tokens[n][4] == 'o' && tokens[n][5] == 'r' && tokens[n][6] == 'y')
	{
		// 			Execute history
		printf("hey\n");
		char F[200];
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
		printf("|%s|\n",F);
		history_vcsh(i,F,1);
		// history_vcsh(i-1,tokens,0);
	}
	else if (tokens[n][0] == 'c' && tokens[n][1] == 'l' && tokens[n][2] == 'e' && tokens[n][3] == 'a' && tokens[n][4] == 'r')
	{
		// 			Execute clear
		clear();
		// history_vcsh(i-1,tokens,0);
	}
	else if(tokens[n][0]=='q' && tokens[n][1]=='u' && tokens[n][2]=='i' && tokens[n][3]=='t'){
		quit();
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
		if(askedinbg)
		{
			for (ll j = findind; j < i; j++)
			{
				tokens[j] = NULL;
			}
			i = findind;
			for (ll p = 0; p < findind; p++)
			{
				printf("|%s|",tokens[p]);
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
				// exit(EXIT_FAILURE);
				exit(0);
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
				printf("-<end of proc %lld %lld %lld\n",WIFEXITED(status),WIFSIGNALED(status),getpid());
			}
		}
		else
		{
//										EXECUTE IN BACKGROUND
			suspendedjobnumber++;
			int pid = fork();
			if (pid == 0)
			{
				// pid_t pgrp = getpgrp();
				// setpgid(0,0);
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
				printf("Done %lld\n",suspendedjobnumber);

			}
		}
	}

	// 			Free up the memory
	free(tokens);
	free(line);
}
