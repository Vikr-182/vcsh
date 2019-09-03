
void prompt_display()
{
    
    int a = gethostname(hostname, sizeof(hostname));
    if (a)
    {
        perror("Error displaying the host name");
        exit(EXIT_FAILURE);
    }
    uid_t uid = getuid();
    struct passwd *b = getpwuid(uid);
    if(!b){
        perror("Error printing the username");
        exit(EXIT_FAILURE);
    }
    printf(ANSI_BLUE_BG);
    printf(ANSI_WHITE);
    printf("%s",b->pw_name);
    printf(ANSI_RESET);
    printf("@");
    printf(ANSI_DARKBLUE_BG);
    printf(ANSI_WHITE);
    printf("%s",hostname);
    printf(ANSI_RESET);
    printf(ANSI_GREY_BG);
    printf(ANSI_BLACK);
    printf("%s",present_directory);
    printf(ANSI_RESET);
    printf(ANSI_BLUE_BG);
    printf(ANSI_WHITE);
    printf("$");
    printf(ANSI_RESET);
    printf(" ");
    // printf("%s@%s:%s$ ",b->pw_name,hostname,present_directory); 
}
