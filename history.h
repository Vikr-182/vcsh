#include "global.h"

void history_vcsh(char *argv[])
{
    // printf("HI");
    if(argv[2]!=NULL){
        printf("Please provide only 2 arguments\n");
    }
    
    ll power = 1;
    ll total = 0;
    for (ll i = 0; i < strlen(argv[1]); i++)
    {
        printf("|%c||",argv[1][i]);
    }
    ll num = 2;
    
    if(argv[1]==NULL){
        num = 20;
        printf("laal\n");
    }
    else
    {
        for (ll i = 0; i < strlen(argv[1]); i++)
        {
            total += power*(argv[1][strlen(argv[1])-1-i]-'0');
            power*= 10;
        }
        
    }
    num = total;
    // printf("%lld ok \n",total);
    ll curr = commandnumber;
    ll diff = 0;
    char lol[20][500];
    while (HISTORY[curr] && diff <= num)
    {
        printf("%s\n",HISTORY[curr]);
        strcpy(lol[diff],HISTORY[curr]);
        curr = curr - 1;
        curr = curr + 20;
        curr = curr % 20;
        diff++;
    }
    for (ll n = 0; n < diff; n++)
    {
        printf("%s\n",lol[diff-n-1]);
    }
    
}