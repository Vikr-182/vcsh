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
		return;
	}
	for(ll t=jno;t<bgind-1;t++)
	{
		procaarray[t] = procaarray[t+1];
	}
	bgind--;
	pid_t pid = procaarray[jno];
	kill(pid,SIGCONT);
	int status;
	do
	{
		waitpid(pid,&status,WUNTRACED);
	}while(!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
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
