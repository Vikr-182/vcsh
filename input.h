
char *input()
{ //              READS LINE UNITL \n ENCOUNTERED
	char *A = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	char B;
	char *C = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	fgets(A, BUFFER_SIZE, stdin);
	//A = readline(">prompt");
	A[strlen(A) - 1] = '\0';
	if (A[0] == '\33')
	{
		if(A[1]=='[' && A[2]=='A')
		{
			int cnt = 0;
			while(cnt<strlen(A))
			{
				if(A[cnt+2]=='A')
				{
					numpressed++;
				}
				else if(A[cnt+2]=='B')
				{
					numpressed--;
				}
				cnt+=3;
			}
			numpressed = strlen(A)/3;
			pressedkey = 1;
		}
	}

	if(A[0]==12)
	{
		strcpy(A,"clear");
		A[6] = '\0';
	}
	return A;
}

char **parse_by_colon(char *line)
{ //              PARSES COMMANDS BY "; "
	char **commands = malloc(sizeof(char *) * (MAX_COMMANDS));
	if (!commands)
	{
		perror("Error allocating the memory");
		exit(EXIT_FAILURE);
	
	char *indv;
	indv = strtok(line, ";");
	ll i = 0;
	while (indv)
	{
		commands[i] = indv;
		i++;
		indv = strtok(NULL, ";");
	}
	NUM_COMMANDS = i - 1;
	return commands;
}

char **parse_by_pipe(char *line)
{ //              PARSES COMMANDS BY "| "
	char **commands = malloc(sizeof(char *) * (MAX_COMMANDS));
	if (!commands)
	{
		perror("Error allocating the memory");
		exit(EXIT_FAILURE);
	}
	char *indv;
	indv = strtok(line, "|");
	ll i = 0;
	while (indv)
	{
		commands[i] = indv;
		i++;
		indv = strtok(NULL, "|");
	}
	NUM_PIPES = i - 1;
	return commands;
}

char **parse_by_space(char *line)
{ //              PARSES COMMANDS BY "| "
	char **commands = malloc(sizeof(char *) * (MAX_COMMANDS));
	if (!commands)
	{
		perror("Error allocating the memory");
		exit(EXIT_FAILURE);
	}
	char *indv;
	indv = strtok(line, " ");
	ll i = 0;
	while (indv)
	{
		commands[i] = indv;
		i++;
		indv = strtok(NULL, " ");
	}
	NUM_PIPES = i - 1;
	return commands;
}
