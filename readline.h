#include "global.h"

char *readline(){               //              READS LINE UNITL \n ENCOUNTERED
	char *A = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	// char B[BUFFER_SIZE] = getchar();
	char B;
	char *C = (char *)malloc(sizeof(char)*BUFFER_SIZE);
	// fflush(stdout);
	// fflush(stdin);
	scanf("%[^\n]%*c",A);
	// ll a = getchar();
	// char a ;
	// scanf("%c",&a);
	// if(a)
	//	{	
	// 			printf("%lld\n",a);
	// 			A[ind++] = a;		
	// 			printf("%c\n",a);
	// 	}
	// 	// printf("%c\n",a);
	// 	scanf("%c",&a);
	// }
	// printf("%c\n",a);
	// A[ind] = '\0';
	// printf("line read is %d|\n",A[0]);
	// // A[strlen(A)] = '\n';
	// // A[strlen(A)+1] = '\0';
	return A;
}

char **parse(char *line){       //              PARSES COMMANDS BY " "
	char **commands = malloc(sizeof(char*)*(MAX_COMMANDS));
	if(!commands){
		perror("Error allocating the memory");
		exit(EXIT_FAILURE);
	}   
	char *indv ;
	indv = strtok(line,";");
	ll i = 0 ;
	while(indv){
		commands[i] = indv;
		i++;
		indv = strtok(NULL,";");
	}
	NUM_COMMANDS = i-1;
	return commands;
}
