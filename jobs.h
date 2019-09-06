void ctrlccross()
{
	/*
	   pid_t pid = getpid();
	   printf("Hello %ld %ld %ld\n",pid,shellid,parentid);
	   if( (pid == shellid && shellid == parentid ))
	   {
	   printf("Namaskar\n");
	   }	
	   else
	   {
	   printf("Dhad se khad se alag kar diya %ld %ld %ld\n",pid,shellid,parentid);
	   kill(pid,SIGINT);		
	   }
	   */
	signal(SIGINT,ctrlccross);
}

void ctrlzcross()
{
	printf("Caught signal %ld\n",getpid());
	pid_t pid = getpid();
	if( (pid != shellid ) )
	{
		printf("Rara\n");
		return;
	}		
	else if(parentid>0)
	{
		pid = parentid;
		procaarray[bgind] = parentid;	
		strcpy(characterarray[pid],curr_command);
		printf("%lld+\tStopped\t%s\n",bgind+1,curr_command);
		bgind++;
		updatejobs();
	}	
	nikal = 1;
	signal(SIGTSTP,ctrlzcross);
}

void signal_handler()
{
	int c;
	ll mil = 0;
	ll io = 0;
	for (ll i = 0; !mil && i < bgind; i++)
	{
		updatejobs();
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
	updatejobs();
	signal(SIGCHLD, signal_handler);
}

void updatejobs()
{
	for(int r=0;r<bgind;r++)
	{
		char path[3000] = "/proc/";
		char num[2000];
		ll p = procaarray[r],ind = 0;
		while(p)
		{
			num[ind++] = (p%10)+'0';
			p = p/10;
		}
		for(ll i=0;i<ind;i++)
		{
			path[i+6] = num[ind-1-i];
		}
		path[ind+6] = '/';
		path[ind+7] = 's';
		path[ind+8] = 't';
		path[ind+9] = 'a';
		path[ind+10] = 't';
		path[ind+11] = 'u';
		path[ind+12] = 's';
		path[ind+13] = '\0';
		FILE *fd = fopen(path,"r");
		if (!fd)
		{
			for(int g=r+1;g<bgind;g++)
			{
				procaarray[g-1] = procaarray[g];
			}
			bgind--;
		}
	}
}

void showjobs()
{
	// 			Shows jobs	
	if(bgind==0)
	{
		printf("0\n");
		return ;
	}
	printf("%lld \n",bgind);

	for(int r=0;r<bgind;r++)
	{
		char path[3000] = "/proc/";
		char num[2000];
		ll p = procaarray[r],ind = 0;
		while(p)
		{
			num[ind++] = (p%10)+'0';
			p = p/10;
		}
		for(ll i=0;i<ind;i++)
		{
			path[i+6] = num[ind-1-i];
		}
		path[ind+6] = '/';
		path[ind+7] = 's';
		path[ind+8] = 't';
		path[ind+9] = 'a';
		path[ind+10] = 't';
		path[ind+11] = 'u';
		path[ind+12] = 's';
		path[ind+13] = '\0';
		FILE *fd = fopen(path, "r");
		printf("|%s|\n",path);
		if (!fd)
		{
			perror("Process terminated\n");
			return;
		}
		char B[BUFFER_SIZE];
		ll cnt = 0;
		fgets(B, BUFFER_SIZE, (FILE *)fd);
		ll fg = 0;
		if (B[0] == 'S' && B[1] == 't' && B[2] == 'a' && B[3] == 't' && B[4] == 'e')
		{
			fg = 1;
		}
		while (!fg)
		{
			fgets(B, BUFFER_SIZE, (FILE *)fd);
			if (B[0] == 'S' && B[1] == 't' && B[2] == 'a' && B[3] == 't' && B[4] == 'e')
			{
				fg = 1;
			}
			cnt++;
		}
		int mil = 0;
		for(int i=0;!mil;i++)
		{
			if(B[i+5]=='S')
			{
				mil = 1;	
			}
			else if(B[i+5]=='T')
			{
				mil = 2;	
			}
			else if(B[i+5]=='R')
			{
				mil = 3;
			}
		}
		if(mil==1)
			printf("[%lld] sleeping %lld %s\n",r,procaarray[r],characterarray[procaarray[r]]);
		else if(mil==2)
			printf("[%lld] stopped %lld %s\n",r,procaarray[r],characterarray[procaarray[r]]);
		else if(mil==3)
			printf("[%lld] running %lld %s\n",r,procaarray[r],characterarray[procaarray[r]]);
		else
			printf("[%lld] %lld %s\n",r,procaarray[r],characterarray[procaarray[r]]);
	}
}
