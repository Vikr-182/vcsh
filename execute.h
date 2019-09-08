void parse_it(char *argv)
{
	char **tot = (char **)malloc(sizeof(char *)*512);
	for(ll k=0;k<200;k++)
	{
		tot[k] = (char *)malloc(sizeof(char)*100);
	}
	ll start = 0;
	ll end = 0;
	ll ind = 0;
	char ** er = (char **)malloc(sizeof(char *)*512);
	for(ll k=0;k<200;k++)
	{
		er[k] = (char *)malloc(sizeof(char )*100);
	}
	ll di = 0;
	ll op = 0;
	while(ind < strlen(argv))
	{
		if((ind < strlen(argv)-1) && argv[ind]=='>' && argv[ind+1]=='>')
		{
			op = 43;        // Random arbitary number
			end = ind-1;
			er[di] = (char *)malloc(sizeof(char )*100);
			for(ll i=start;i<=end;i++)
			{
				er[di][i-start] = argv[i];
			}
			er[di][end-start+1] = '\0';
			di++;
			start = ind+2;
			ind++;
		}
		else if((ind < strlen(argv)-1) && argv[ind]=='>' && argv[ind+1]!='>')
		{
			op = 45;        // Random arbitary number
			end = ind-1;
			er[di] = (char *)malloc(sizeof(char )*100);
			for(ll i=start;i<=end;i++)
			{
				er[di][i-start] = argv[i];
			}
			er[di][end-start+1] = '\0';
			di++;
			start = ind+1;
			ind++;
		}
		else
		{
			// do nothing
		}
		ind++;
	}
	end = ind-1;
	for(ll i=start;i<=end;i++)
	{
		er[di][i-start] = argv[i];
	}
	er[di][end-start+1] = '\0';
	di++;
	ll tin = 0;
	ll lt_exists = 0;
	for(ll f=0;f<di;f++)
	{
		// Parse each string by "<"
		start = 0;
		end = 0;
		for(ll o=0;o<strlen(er[f]);o++)
		{
			if(er[f][o]=='<')
			{
				lt_exists = 1;
				end = o-1;
				tot[tin] = (char *)malloc(sizeof(char )*100);
				for(ll b=start;b<=end;b++)
				{
					tot[tin][b-start] = er[f][b];
				}
				tot[tin][end-start+1] = '\0';
				tin++;
				start = o+1;
			}
			else
			{
				//		printf("Na\n");
			}
		}
		end = strlen(er[f])-1;
		tot[tin] = (char *)malloc(sizeof(char )*100);
		for(ll b=start;b<=end;b++)
		{
			tot[tin][b-start] = er[f][b];
		}
		tot[tin][end-start+1] = '\0';
		tin++;
	}
	for(ll u=0;u<tin;u++){printf(";%s;",tot[u]);}
	// op = 43 implies '>>'
	if(!lt_exists && !op)
	{
		redirect(argv);
		return;
	}
	int status;
	pid_t pid = fork();
	if(pid<0){perror("Forking error\n");}
	if(pid==0)
	{
		if(lt_exists && op)
		{
			// Do both input and output
			if(op==43)
			{
				int fd2 = open(tot[1],O_RDONLY);			// Use this file for reading only
				if(fd2<0)
				{
					perror("Can't open\n");
				}

				int fd3 = open(tot[2],O_WRONLY|O_APPEND|O_CREAT,0644); 		// Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
				}

				// Now redirect stdin of first command such that it reads from fd2 and appends to fd3
				dup2(fd2,0);					// Make it read from fd2
				dup2(fd3,1);					// Make it write to fd3
				redirect(tot[0]);
				close(fd2);
				close(fd3);
			}
			else
			{
				int fd2 = open(tot[1],O_RDONLY);                  // Use this file for reading only
				if(fd2<0)
				{
					perror("Can't open\n");
				}

				int fd3 = open(tot[2],O_WRONLY|O_TRUNC |O_CREAT,0644);                 // Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
				}


				// Now redirect stdin of first command such that it reads from fd2 and appends to fd3
				dup2(fd2,0);					// Make it read from fd2
				close(fd2);
				dup2(fd3,1);					// Make it write to fd3
				close(fd3);
				redirect(tot[0]);
			}
		}
		else if(lt_exists && !op)
		{
			// Do only input 
			int fd2 = open(tot[1],O_RDONLY);                  // Use this file for reading only
			if(fd2<0)
			{
				perror("Cant open\n");
			}

			// Now redirect stdin of first command such that it reads from fd2 and appends to fd3
			dup2(fd2,0);
			close(fd2);
			redirect(tot[0]);
		}
		else if(!lt_exists && op)
		{
			// Do only output
			printf("JHi ra %s\n",tot[1]);
			if(op==43)
			{
				int fd3 = open(tot[1],O_WRONLY|O_APPEND|O_CREAT,0644);                 // Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
				}
				dup2(fd3,1);
				close(fd3);
				redirect(tot[0]);
			}
			else
			{
				int fd3 = open(tot[1],O_WRONLY|O_TRUNC|O_CREAT,0644);                 // Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
				}
				dup2(fd3,1);
				close(fd3);
				redirect(tot[0]);
			}
		}
		exit(0);
	}
	else
	{
		while (!(wait(&status) == pid)) ;
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

