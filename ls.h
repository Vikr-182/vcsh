#include "global.h"

mode_t persons[3][3] = {{S_IRUSR, S_IWUSR, S_IXUSR}, {S_IRGRP, S_IWGRP, S_IXGRP}, {S_IROTH, S_IWOTH, S_IXOTH}};
char values[3] = {'r', 'w', 'x'};

void ls_vcsh(int argc, char *argv[])
{
	int listing = 0;
	int hidden = 0;
	struct dirent *de;

	// Parse argv[] to get the correct path
	/*

	DIR *p = opendir(path);
	de = readdir(p);
	int cnt = 0;
	while (de || !cnt)
	{
		cnt++;
		de = readdir(p);
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
		}
	}*/
}
