# vcsh
A basic shell in C

## Commands implemented

- `cd` 
- `pwd`
- `echo`
- `pinfo`
- `ls`
- `history`
- Putting process into background
- `nightswatch`

## File stucture

- `cd`,`pwd` and `echo` are implemented in builtins.h 
- String parsing and execution happens in direct.h
- prompt display happens in prompt.h
- history.h controls history
- Readline takes the input
- nightswatch.h implements nightswatch command

Hope you enjoy :-)
