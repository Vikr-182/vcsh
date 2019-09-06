void set_env(char **commands)
{
	if(!commands[1] || !commands[2])
	{
		printf("Please provide complete arguments\n");
		return ;
	}
	if(setenv(commands[1],commands[2],1)<0)
	{
		perror("Error setting the environment variable");
	}
	else	
	{
		printf("Successfully done\n");
	}
}

void unset(char **commands)
{
        if(!commands[1])
        {
                printf("Please provide complete arguments\n");
                return ;
        }
        if(unsetenv(commands[1])<0)
        {
                perror("Error unsetting the environment variable");
        }
}
