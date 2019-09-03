
void vcsh_cronjob(char *argv[]){
	// Following -c -t -p format
	// -c for command
	// -t for repeatation time
	// -p for max time
	ll cind = 0;
	ll pind = 0;
	ll tind = 0;
	ll prod = 1;
	if(strcmp(argv[1],"-c")){
		printf("Please provide the correct format for flags\n");
		return ;
	}
	if(strcmp(argv[3],"-t"))
	{
		printf("Please provide the correct format for flags\n");
		return ;
	}
	if(strcmp(argv[5],"-p"))
	{
		printf("Please provide the correct format for flag\n");
		return ;	
	}
	for(ll i=0;i<strlen(argv[4]);i++){
		tind += (argv[4][strlen(argv[4]-1)]-'0')*prod;
		prod *= 10;
	}	
	prod = 1;
        for(ll i=0;i<strlen(argv[6]);i++){
                pind += (argv[6][strlen(argv[6]-1)]-'0')*prod;
                prod *= 10;
        }
	printf("Hi %ld %ld\n",pind,tind);
	long int start = time(0);
	long int cur = time(0);
	long int prev = -1;
	char tokens[3][10];
	strcpy(tokens[0],argv[2]);
	printf("%ld %ld\n",pind,tind);
	while(time(0)-cur<=pind)
	{
		if(((time(0)-start)-tind)==0 && prev!=start)
		{
			prev = start;
			redirect(tokens);
			start = time(0);	
		}
	}
}
