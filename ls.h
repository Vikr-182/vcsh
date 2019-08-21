#include "global.h"

mode_t persons[3][3] = {{S_IRUSR, S_IWUSR, S_IXUSR}, {S_IRGRP, S_IWGRP, S_IXGRP}, {S_IROTH, S_IWOTH, S_IXOTH}};
char values[3] = {'r', 'w', 'x'};

ll isdir(mode_t a)
{
	return S_ISDIR(a);
}

ll isfile(mode_t a)
{
	return S_ISREG(a);
}

ll islink(mode_t a)
{
	return S_ISLNK(a);
}

ll isblk(mode_t a)
{
	return S_ISBLK(a);
}

void color(ll a)
{
	if (a == 0)
	{
		printf(ANSI_BLUE);
	}
	else if (a == 1)
	{
		printf(ANSI_GREEN);
	}
	else if (a == 2)
	{
		printf(ANSI_RED);
	}
	else if (a == 3)
	{
		printf(ANSI_YELLOW);
	}
}

void allocate(mode_t a, ll fg)
{
	if (isdir(a))
	{
		color(0);
		if (fg)
		{
			printf("d");
		}
	}
	else if (isfile(a))
	{
		color(1);
		if (fg)
		{
			printf("-");
		}
	}
	else if (islink(a))
	{
		color(2);
		if (fg)
		{
			printf("l");
		}
	}
	else if (isblk(a))
	{
		color(3);
		if (fg)
		{
			printf("b");
		}
	}
}

void ls_vcsh(int argc, char *argv[])
{
	int listing = 0;
	int hidden = 0;
	struct dirent *de;
	for (ll n = 0; n <= argc; n++)
	{
		printf("|%s|", argv[n]);
	}
	printf("ho");
	ll *array = (ll *)calloc(sizeof(ll), MAX_TOKENS);
	for (ll i = 1; i <= argc; i++)
	{
		if (argv[i] == NULL)
		{
			break;
		}
		if (argv[i][0] == '-')
		{
			// it is a flag
			array[i] = 1;
		}
	}
	ll f = 1;
	ll find = 0;
	ll count = 0;
	for (ll i = 1; i <= argc; i++)
	{
		if (array[i] != 1 && f)
		{
			// First not a flag this is an address.
			f = 0;
			find = i;
			count++;
		}
		else if (array[i] != 1 && !f)
		{
			count++;
		}
	}
	if (count > 1)
	{
		printf("\nPlease provide only 2 paths\n");
	}
	char *path = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	printf("||_<>|%s|\n",homedirectory);
	printf("%lld %lld\n", f, find);
	if (f)
	{
		//No path provided ls in current working directory
		printf("hi %s %d\n",present_directory,lengthofpresentdictionary);
		for (ll n = 0; n < lengthofpresentdictionary; n++)
		{
			path[n] = present_directory[n];
		}
		path[lengthofpresentdictionary] = '\0';
		printf("-|%s|-\n",path);
		if (path[0] == '~')
		{
			for (ll n = 0; n < lengthofhomedirectory; n++)
			{
				path[n] = homedirectory[n];
			}
			path[lengthofhomedirectory] = '/';
			 printf("middle |%s|\n",path);
			for (ll n = 2; n < lengthofpresentdictionary; n++)
			{
				path[n - 2 + lengthofhomedirectory + 1] = present_directory[n];
			}
			path[lengthofpresentdictionary-2+1+lengthofhomedirectory] = '\0';
		}
		// printf("%s|\n",present_directory);
		printf("%s|\n", path);
	}
	else
	{
		// This is the same path as argv[find]
		if (argv[find][0] == '~')
		{
			for (ll n = 0; n < lengthofhomedirectory; n++)
			{
				path[n] = homedirectory[n];
			}
			path[lengthofhomedirectory] = '/';
			for (ll n = 2; n < strlen(argv[find]); n++)
			{
				path[n + lengthofhomedirectory + 2] = argv[find][n];
			}
			printf("%s|\n", path);
		}
		else
		{
			for (ll n = 0; n < strlen(argv[find]); n++)
			{
				path[n] = argv[find][n];
			}
			path[strlen(argv[find])] = '\0';
			printf("%s|\n", path);
		}
	}
	ll invalid = 0;
	for (ll n = 0; n < argc + 1; n++)
	{
		if (array[n])
		{
			printf("%s ok\n", argv[n]);
			if (argv[n][0] == '-' && argv[n][1] == 'l' && argv[n][2] != 'a')
			{
				listing = 1;
			}
			else if (argv[n][0] == '-' && argv[n][1] == 'l' && argv[n][2] == 'a')
			{
				listing = 1;
				hidden = 1;
			}
			else if (argv[n][0] == '-' && argv[n][1] == 'a' && argv[n][2] != 'l')
			{
				hidden = 1;
			}
			else if (argv[n][0] == '-' && argv[n][1] == 'a' && argv[n][2] == 'l')
			{
				listing = 1;
				hidden = 1;
			}
			else if (argv[n][0] == '-')
			{
				printf("invalid |%s|\n", argv[n]);
				invalid = 1;
			}
		}
	}
	if (invalid)
	{
		printf("Invalid option\n");
		return;
	}
	// Parse argv[] to get the correct path
	printf("yay %s\n",path);
	DIR *p = opendir(path);
	if (!p)
	{
		perror("can;t open\n");
		return;
	}
	de = readdir(p);
	int cnt = 0;
	// printf("%lld %lld\n", listing, hidden);
	ll a = chdir(path);
	printf("%s\n", present_directory);
	while (de || !cnt)
	{
		cnt++;

		if (!de)
		{
			perror("Error occured opening the directory");
			exit(EXIT_FAILURE);
		}
		// de->d_name contains the name of the file / directory
		else
		{
			char filepath[30000];
			for (ll i = 0; i < strlen(path); i++)
			{
				filepath[i] = path[i];
			}
			filepath[strlen(path)] = '/';
			for (ll i = strlen(path) + 1; i < strlen(path) + strlen(de->d_name); i++)
			{
				filepath[i] = de->d_name[i - strlen(path)];
			}
			// printf("->%s<-\n", de->d_name);
			struct stat h;
			int a = stat(de->d_name, &h);
			if (a == -1)
			{
				printf("Error displaying the file\n");
				de = readdir(p);
				continue;
			}
			if (listing && !hidden)
			{
				if (de->d_name[0] != '.')
				{
					allocate(h.st_mode, 1);

					// Printing permissions
					for (ll i = 0; i < 3; i++)
					{
						for (ll j = 0; j < 3; j++)
						{
							if (h.st_mode & persons[i][j])
							{
								printf("%c", values[j]);
							}
							else
							{
								printf("-");
							}
						}
					}

					printf("\t");

					// printing number of links
					printf("%ld", h.st_nlink);

					printf("\t");

					// printing User
					uid_t uid = h.st_uid;
					struct passwd *b = getpwuid(uid);
					printf("%s", b->pw_name);

					printf("\t");

					// printing Group
					gid_t gid = h.st_gid;
					struct passwd *ab = getgrgid(gid);
					printf("%s", ab->pw_name);

					printf("\t");

					// Printing block size
					printf("%ld", h.st_blocks);
					printf("\t");

					// Last atime
					// printf("%d",(h.st_atime));
					struct tm *tm = localtime(&h.st_mtime);
					char thetime[BUFFER_SIZE];
					strcpy(thetime, asctime(tm));
					thetime[strlen(thetime) - 1] = '\0';

					printf("%s", thetime);

					printf("\t");

					// Printing block name
					printf("%s", de->d_name);
				}
			}
			else if (listing && hidden)
			{
				allocate(h.st_mode, 1);

				// Printing permissions
				for (ll i = 0; i < 3; i++)
				{
					for (ll j = 0; j < 3; j++)
					{
						if (h.st_mode & persons[i][j])
						{
							printf("%c", values[j]);
						}
						else
						{
							printf("-");
						}
					}
				}

				printf("\t");

				// printing number of links
				printf("%ld", h.st_nlink);

				printf("\t");

				// printing User
				uid_t uid = h.st_uid;
				struct passwd *b = getpwuid(uid);
				printf("%s", b->pw_name);

				printf("\t");

				// printing Group
				gid_t gid = h.st_gid;
				struct passwd *ab = getgrgid(gid);
				printf("%s", ab->pw_name);

				printf("\t");

				// Printing block size
				printf("%ld", h.st_blocks);

				printf("\t");

				// Last atime
				// printf("%d",(h.st_atime));
				struct tm *tm = localtime(&h.st_mtime);
				char thetime[BUFFER_SIZE];
				strcpy(thetime, asctime(tm));
				thetime[strlen(thetime) - 1] = '\0';

				printf("%s", thetime);

				printf("\t");

				// Printing block name
				printf("%s", de->d_name);
			}
			else if (hidden && !listing)
			{
				allocate(h.st_mode, 0);
				printf("%s\t", de->d_name);
			}
			else if (!hidden && !listing)
			{
				if (de->d_name[0] != '.')
				{
					allocate(h.st_mode, 0);
					printf("%s\t", de->d_name);
				}
			}
			printf(ANSI_RESET);
		}
		printf("\n");
		de = readdir(p);
	}
	printf("\n");
	char comm[BUFFER_SIZE + 2];
	if(present_directory[0]=='~'){
		for (ll n = 0; n < lengthofhomedirectory; n++)
		{
			comm[n] = homedirectory[n];
		}
		comm[lengthofhomedirectory] = '/';
		for (ll n = 0; n < lengthofpresentdictionary; n++)
		{
			comm[lengthofhomedirectory+1+n] = present_directory[n+1];
		}
		ll b = chdir(comm);
	}
	else{
		ll b = chdir(present_directory);
	}
}
