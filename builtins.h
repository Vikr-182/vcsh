#include "global.h"

ll cd_vcsh(char *argv[])
{
	// Parse from space
	if (argv[2] != NULL)
	{
		printf("Please giive only 2 arguments\n");
	}
	if (argv[1] == NULL)
	{
		// printf("Yay %s\n",homedirectory);
		int a = chdir(homedirectory);
		if (a == -1)
		{
			perror("Unable to change directory");
			return (EXIT_FAILURE);
		}

		// If successfully changed then update past directory
		for (ll n = 0; n < lengthofhomedirectory; n++)
		{
			past_present_directory[n] = present_directory[n];
		}
		return 1;
	}

	ll g = 0;
	for (ll n = 1; n < strlen(argv[1]); n++)
	{
		if (argv[1][n] == '~')
		{
			g = 1;
		}
	}
	if (g)
	{
		printf("Invalid address\n");
		return (EXIT_FAILURE);
	}
	if (argv[1][0] == '~')
	{
		char A[BUFFER_SIZE];
		for (ll n = 0; n < strlen(homedirectory); n++)
		{
			A[n] = homedirectory[n];
		}
		for (ll n = 0; n < strlen(argv[1]); n++)
		{
			A[n + strlen(homedirectory)] = argv[1][1 + n];
		}
		A[strlen(argv[1]) + strlen(homedirectory)] = '/';
		A[strlen(argv[1]) + strlen(homedirectory) + 1] = '\0';

		printf("|%s|\n", A);
		// ll b = chdir(A);
		ll b = chdir(A);
		// printf("Yay %d\n", b);
		if (b == -1)
		{
			perror("Unaable to change directory");
			return (EXIT_FAILURE);
		}

		// If was able to successfully cd
		for (ll n = 0; n < lengthofpresentdictionary; n++)
		{
			past_present_directory[n] = present_directory[n];
		}
		return 1;
	}

	if (argv[1][0] == '-' && strlen(argv[1]) == 1)
	{
		// go back to previous directory
		printf("%s\n", past_present_directory);
		ll g = chdir(past_present_directory);
		if (g == -1)
		{
			perror("Error going back to past directory");
			return EXIT_FAILURE;
		}
		for (ll n = 0; n < lengthofpresentdictionary; n++)
		{
			past_present_directory[n] = present_directory[n];
		}
	}
	int f = chdir(argv[1]);
	if (f == -1)
	{
		perror("Unable to change directory");
		return (EXIT_FAILURE);
	}

	// If was able to successfully cd
	for (ll n = 0; n < lengthofpresentdictionary; n++)
	{
		past_present_directory[n] = present_directory[n];
	}

	return 1;
}

void pwd_vcsh()
{
	char A[65536];
	char B[65536];

	getcwd(A, sizeof(A));
	if (!A)
	{
		perror("Cant see the present directory");
		return;
	}
	printf("|%s|\n", A);
}

void clear()
{
	printf(ANSI_CLEAR_SCREEN);
}

void reset()
{
	printf(ANSI_RESET_SCREEN);
}

void echo_vcsh(ll argc, char *argv[])
{
	ll nflag = 0;
	for (ll i = 1; i < argc + 1; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == 'n')
		{
			nflag = 0;
		}
	}
	ll start = 0;
	ll fin = 0;
	ll startind = 0;
	ll startarr = 0;
	ll findind = 0;
	ll findarr = 0;
	char majorstring[BUFFER_SIZE];
	ll ind = 0;
	;
	for (ll i = 1; i < argc + 1; i++)
	{
		for (ll j = 0; j < strlen(argv[i]); j++)
		{
			majorstring[ind++] = argv[i][j];
		}
		majorstring[ind++] = ' ';
	}

	ll dollaraaya = 0;
	for (ll i = 0; i < ind;)
	{
		if (!start && majorstring[i] == '$')
		{
			dollaraaya = 1;
		}
		// printf("%lld %lld %lld %c\n",i,start,dollaraaya,majorstring[i]);
		if (!start && majorstring[i] == '\"')
		{
			start = 1;
			startind = i + 1;
			i++;
			continue;
		}
		else if (start && majorstring[i] == '\"')
		{
			start = 0;
			findind = i - 1;

			//				Print from i->j whatever it is
			for (ll h = startind; h <= findind;)
			{
				ll dollar = 0;
				ll p;
				ll markedarray[3000];
				char H[2000];
				ll here = 0;
				if (majorstring[h] == '$')
				{
					here = h;
					ll ij;
					for (ij = h + 1; ij <= findind && majorstring[ij] != ' '; ij++)
					{
						H[ij - h - 1] = majorstring[ij];
					}
					H[ij - h - 1] = '\0';
					dollar = 1;
					char *b = getenv(H);
					if (b && dollar)

						printf("%s ", b);
					else if (dollar)
					{
						printf("\t");
					}
					h = ij;
				}
				else
				{
					printf("%c", majorstring[h]);
					h++;
				}
			}
			i = findind + 2;
			continue;
		}
		else if (!start && majorstring[i] != '\"' && !dollaraaya)
		{
			printf("%c", majorstring[i]);
			i++;
			continue;
		}
		else if (!start && majorstring[i] != '\"' && dollaraaya)
		{
			dollaraaya = 0;
			ll ij;
			char T[300];
			for (ij = i + 1; ij < ind && majorstring[ij] != ' '; ij++)
			{
				T[ij - i - 1] = majorstring[ij];
			}

			T[ij - i - 1] = '\0';

			i = ij;
			char *b = getenv(T);
			if (b)

				printf("%s ", b);
			else
			{
				printf("\t");
			}
			continue;
		}
		else if (!start && majorstring[i] != '$' && !dollaraaya)
		{
			i++;
			continue;
		}
		else
		{
			i++;
		}
	}

	if (!nflag)
	{
		printf("\n");
	}
}

void quit()
{
	exit(0);
}