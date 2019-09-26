ll convert(char *tokens)
{
	ll num = 0;
	ll p = 1;
	ll jno,sig;
	for(ll i=0;i<strlen(tokens);i++)
	{
		num += (tokens[strlen(tokens)-1-i]-'0')*p;
		p *= 10;
	}
	jno = num;
	return jno;
}

void fg(char **tokens)
{
	int og = 0;
	if(tokens[2])
	{
		printf("Please provide the correct number of arguments\n");
	}
	else if(!tokens[1])
	{
		printf("Please provide all arguments\n");
		og = 1;
	}
	printf("eji oji lo ji suno ji %lld\n",og);
	ll jno ;
	if(!og)
	{
		jno = convert(tokens[1]);
	}
	else
	{
		jno = bgind - 1;	
	}
	printf("%lld %s is the job number put into forefround\n",jno,characterarray[procaarray[jno]]);
	if(jno >= bgind)
	{
		printf("Please provide correct job number\n");
		return;
	}
	for(ll t=jno;t<bgind-1;t++)
	{
		procaarray[t] = procaarray[t+1];
	}
	bgind--;
	pid_t pid = procaarray[jno];

	/* Capture the terminal attributes in a struct termios file */
	
	struct termios term1,term2;
	if(tcgetattr(0,&term1)!=0)
	{
		printf("Error transfering control\n");
		return ;
	}
	/*
	if(tcgetattr(1,&term2)!=0)
	{
		printf("Error transfering control\n");
		return ;
	}
	*/

	/* Give the controlling terminal to this process */
	kill(pid,SIGCONT);
	/*
	if(tcsetpgrp(0,gidarray[jno])==-1)
	{
		perror("Error giving stdin\n");
		return ;
	}

	if(tcsetpgrp(1,gidarray[jno])==-1)
	{
		perror("Error giving stdout\n");
		return ;
	}
	*/
	/* Give attributes to the process */
	/*
	if(tcsetattr(0,TCSNOW   ,&term1)!=0)
	{
		printf("Error transfering stdin\n");
		return ;
	}
	if(tcsetattr(1,TCSNOW   ,&term2)!=0)
	{
		printf("Error transfering stdout\n");
		return ;
	}


	printf("Gave stdout to process\n");
	*/
	printf("%ld ko bulaya\n",pid);
	int status;
	do
	{
		waitpid(pid,&status,WUNTRACED);
	}
	while(!WIFEXITED(status) && !WIFSIGNALED(status) );
	if(WIFEXITED(status))
	{
		printf("Lo\n");
	}
	else if(WIFSIGNALED(status))
	{
		printf("Fo\n");
	}
	else if(WIFSTOPPED(status))
	{
		printf("Go\n");
	}

	/* Return back controlling terminal from process to shell */
	/*
	tcsetpgrp(0,shell_gid);
	tcsetpgrp(1,shell_gid);
	if(tcsetattr(0,TCSNOW   ,&term1)!=0)
	{
		printf("Error transfering stdin\n");
		return ;
	}
	if(tcsetattr(1,TCSNOW   ,&term2)!=0)
	{
		printf("Error transfering stdout\n");
		return ;
	}
	*/
	updatejobs();
}

void bg(char **tokens)
{
	if(tokens[2])
	{
		printf("Please provide the correct number of arguments\n");
	}
	else if(!tokens[1])
	{
		printf("Please provide all arguments\n");
		return;
	}
	ll jno = convert(tokens[1]);
	if(jno >= bgind){
		printf("Please provide correct job number\n");
	}
	pid_t pid = procaarray[jno];
	kill(pid,SIGCONT);
	updatejobs();
}

void kill_job(char **tokens)
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
	ll jno = convert(tokens[1]);
	if(jno >= bgind){
		printf("Please provide correct job number\n");
	}
	ll sig = convert(tokens[2]);
	pid_t pid = procaarray[jno];
	kill(pid,sig);
}

void overkill()
{
	for(ll j=0;j<bgind;j++)
	{
		kill(procaarray[j],SIGINT);
	}
	bgind = 0;
}
