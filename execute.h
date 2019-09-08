void redirect_simple(char *argv)
{
	char ** lt = parse_by_delim(argv,">>");	
	char ** commands = (char **)malloc(sizeof(char *)*MAX_TOKENS);
	int ind = 0;
	int mapping[10000];
	for(ll i=0;lt[i];i++)
	{
		char **ge = parse_by_delim(lt[i],"<");
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

	signal(SIGINT,ctrlccross);
	signal(SIGTSTP,ctrlzcross);		
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
	else if(!strcmp(tokens[n],"setenv"))
	{
		set_env(tokens);
	}
	else if(!strcmp(tokens[n],"unset"))
        {
                unset(tokens);
        }
	else if(!(strcmp(tokens[n],"fg")))
        {
                fg(tokens);
        }
        else if(!(strcmp(tokens[n],"bg")))
        {
                bg(tokens);
        }
        else if(!(strcmp(tokens[n],"kjob")))
        {
                kill_job(tokens);
        }
        else if(!(strcmp(tokens[n],"overkill")))
        {
                overkill();
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
				signal(SIGINT,SIG_DFL);
//				printf("Mai hu chld meri pid hai %ld\n",getpid());
				if (!strcmp(tokens[n], "ls"))
				{
					ls_vcsh(i - 1, tokens); // 				execute ls
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
				exit(1);
			}
			else if (pid < 0)
			{
				perror("Error forking");
			}
			else
			{
				parentid = pid;
//				printf("Parent ins %ld and child is %ld\n",parentid,pid);
				int status;
				do
				{
					waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status) && !nikal);
				//wait(NULL);
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
				signal(SIGINT,SIG_DFL);
				setpgid(0, 0);
				signal(SIGTSTP,ctrlzcross);		
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
					history_vcsh(i, F, 1);
				}
				else if(!strcmp(tokens[n],"cronjob"))
				{
					vcsh_cronjob(tokens);
				}
				else
				{
					signal(SIGINT,ctrlccross);
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
				parentid = pid;
				printf("Parent ins %ld and child is %ld\n",parentid,pid);
				procaarray[bgind] = pid;
				bgind++;
				strcpy(characterarray[pid], argv);
				int status;
				printf("Done %lld\n",bgind-1);
				return;
			}
		}
	}
	free(tokens); // 			Free up the memory
	free(line);
}

