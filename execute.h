void removefromloop()
{
	printf("Lo mai aaya %ld\n",getpid());
	lekarle = 1;	
}


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
	// op = 43 implies '>>'
	if(!lt_exists && !op)
	{
		redirect(argv);
		return;
	}
	int status;

	// check read file for any spaces 
	ind = 0;
	start = 0;
	while(tot[1][ind]==' ')
	{
		ind++;
	}
	start = ind;
	while(ind < strlen(tot[1]) && tot[1][ind]!=' ')
	{
		ind++;
	}
	char temp[3000];
	for(ll k=start;k<ind;k++)
	{
		temp[k-start] = tot[1][k];
	}
	temp[ind-start] = '\0';
	ll chk = 0;
	for(ll p=ind;p<strlen(tot[1]);p++)
	{
		if(tot[1][p]!=' ')
		{
			chk = 1;	
		}
	}
	if(chk == 1)
	{
		printf("Please input the correct name of file\n");
		return ;
	}
	for(ll h=0;h<strlen(temp);h++)
	{
		tot[1][h] = temp[h];
	}
	tot[1][strlen(temp)] = '\0';

	// check write file for any spaces
	ind = 0;
        while(tot[2][ind]==' ')
        {
                ind++;
        }
        while(ind < strlen(tot[2]) && tot[2][ind] != ' ')
        {
                ind++;
        }
	char temp2[3000];
        for(ll k=start;k<ind;k++)
        {
                temp2[k-start] = tot[2][k];
        }
        temp2[ind-start] = '\0';
        chk = 0;
        for(ll p=ind;p<strlen(tot[2]);p++)
        {
                if(tot[2][p]!=' ')
                {
                        chk = 1;
                }
        }
	if(chk == 1)
	{
		printf("Please provide correct name of file\n");
		return ;
	}
	for(ll h=0;h<strlen(temp2);h++)
        {
                tot[2][h] = temp2[h];
        }
        tot[2][strlen(temp2)] = '\0';



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
					exit(0);
				}

				int fd3 = open(tot[2],O_WRONLY|O_APPEND|O_CREAT,0644); 		// Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
					exit(0);
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
					exit(0);
				}

				int fd3 = open(tot[2],O_WRONLY|O_TRUNC |O_CREAT,0644);                 // Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
					exit(0);
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
				exit(0);
			}

			// Now redirect stdin of first command such that it reads from fd2 and appends to fd3
			dup2(fd2,0);
			close(fd2);
			redirect(tot[0]);
		}
		else if(!lt_exists && op)
		{
			// Do only output
			if(op==43)
			{
				int fd3 = open(tot[1],O_WRONLY|O_APPEND|O_CREAT,0644);                 // Write to here
				if(fd3<0)
				{
					perror("Can't open\n");
					exit(0);
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
					exit(0);
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
	else if(!strcmp(tokens[n],"unsetenv"))
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
		if(tokens[3])
		{
			printf("Please provide the correct number of arguments\n");
		}
		else if(!tokens[1]||!tokens[2])
		{
			printf("Please provide all arguments\n");
			return;
		}
		ll jno = 0;
		ll p = 1;
		for(ll j=0;j<strlen(tokens[1]);j++)
		{
			jno += p*(tokens[1][strlen(tokens[1])-1-j]-'0');
			p *= 10;
		}
		printf("Got jon as %lld\n",jno);
		if(jno >= bgind){
			printf("Please provide correct job number\n");
			return ;
		}
		 ll sig = 0;
                p = 1;
                for(ll j=0;j<strlen(tokens[2]);j++)
                {
                        sig += p*(tokens[2][strlen(tokens[2])-1-j]-'0');
			p *= 10;
                }
                printf("Got sig as %lld\n",sig);
		kill_job(jno,sig);
	}
	else if(!(strcmp(tokens[n],"overkill")))
	{
		ll j = bgind;
		ll prev = bgind;
		while(j>0)
		{
			kill_job(0,9);
			while(prev==bgind);
			prev = bgind;
			j--;
		}
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

		if(!strcmp(tokens[n],"cronjob"))
		{
			iscronjob = 1;
			askedinbg = 1;								//			execute in background
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
				if(nikal)
				{
					printf("Caught signal %ld\n",pid);
					pid_t pid = getpid();
					if( (pid != shellid ) )
					{
						return;
					}
					else if(parentid>0)
					{
						pid = parentid;
						procaarray[bgind] = parentid;
						gidarray[bgind] = getpgid(parentid);
						strcpy(characterarray[pid],curr_command);
						printf("%lld+\tStopped\t%s\n",bgind,curr_command);
						bgind++;
						updatejobs();
					}
				}
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
				//setpgid(0, 0);
				signal(SIGTSTP,ctrlzcross);		
			//	printf("Mai hun child meri pid hai %ld\n",getpid());
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
				if(iscronjob)
				{
					pid_cronjob = pid;
				}
				parentid = pid;
				procaarray[bgind] = pid;
				gidarray[bgind] = getpgid(pid);
				bgind++;
				strcpy(characterarray[pid], argv);
				int status;
				kill(pid,SIGTTIN);				
				printf("Done %lld\n",bgind-1);
			}
		}
	}
	if(iscronjob)
	{
		kill(pid_cronjob,SIGCONT);
	}
	free(tokens); // 			Free up the memory
	free(line);
}

