#include "global.h"

char *readline()
{ //              READS LINE UNITL \n ENCOUNTERED
	char *A = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	char B;
	char *C = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	fgets(A, BUFFER_SIZE, stdin);
	A[strlen(A) - 1] = '\0';
	if (A[0] == (0))
	{
		A[0] = '\n';
		A[1] = '\0';
	}
	return A;
}

char **parse(char *line)
{ //              PARSES COMMANDS BY " "
	char **commands = malloc(sizeof(char *) * (MAX_COMMANDS));
	if (!commands)
	{
		perror("Error allocating the memory");
		exit(EXIT_FAILURE);
	}
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
