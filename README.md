# vcsh
A basic shell in C

## Installation Instructions
- Download the file as a zip or use the following command <br/>
`$ git clone https://github.com/Vikr-182/vcsh `
- Inside the directory run  <br/>
`$ make   ` 

     `$ ./Shell`
- To clean the object files, use <br/> 
`$ make clean`

## File stucture 
    .
    ├── builtins.h          # implements builtin commands like cd,pwd,quit and echo. 
    ├── cronjob.h           # implements cronjob command
    ├── direct.h            # contains the shell loop and initalisation as well as piping.
    ├── execute.h           # parses the string to check redirection and executes the command.
    ├── env.h               # implements setenv and unsetenv
    ├── history.h           # has history-related commands like history and for resizing.
    ├── input.h_            # takes the user input . It also checks if up arrow key was pressed or not.
    ├── jobs_commands.h     # implements functions like fg,bg and kjob.
    ├── jobs.h              # contains signal handlers for Ctrl+Z and Ctrl+C as well as SIGCHLD handler. 
    ├                       # In addition, it contains updatejobs and showjobs which update and display the list respectively.
    ├── ls.h                # implements the ls command.
    ├── main.c              # contains the welcome message and call to the loop
    ├── nightswatch.h       # contains nightswatch implementation.
    ├── pinfo.h             # has `pinfo` implemented.
    ├── prompt.h            # displays the prompt.
    ├── global.h            # contains the variables shared across the files.

## Commands implemented
##### `cd`
- Changes directory using _chdir_
- Keeps a track of previous directory whenever changing to another directory to take care of `cd -`

##### `ls`
- Implements `ls -[la]` in all different ways.
- If a directory is specified, the contents of that directory are obtained using _readdir_ command which returns a struct which is passed as argument to _stat_ function to obtain other information
- Appropriate color scheme is used for files, directories, links etc.

##### `pwd`
- Uses the command _getcwd_ .
- Error handling is done if size too big.

##### `echo`
- First -n flag is looked whether to output the trailing newline.
- Across the traversal, _$_ is searched for to check presence of any environment variables.
- Also, string inside _" "_ will be printed as it is. It is checked via keeping a start and end pointer for the opening and closing positions. If not closed, error is shown up.

##### `pinfo`
- Only requires pid or null as argument. Error handling is done in case not done in such a way.
- All information regarding the process is obtained in various files located inside _/proc/pid_ .

##### `nightswatch`
- ncurses library is used to implement it/

##### `history <num>`
- A file ".vcsh\_history" keeps track of all commands typed. 
- _resize_ checks the number of commands entered till now. If it is greater than 25, the history file is resized by taking the most recent commands
- Prints the last num commands typed. If num not entered , prints the last 10 commands by default.

##### `setenv <variable> <value>`
- Only accepts name of environment variable and value as input. Error handling is done otherwise.
- The variable is set using _setenv_ command. If not able to set, error message shows up.

##### `unsetenv <variable>`
- Takes only name of variable to be unset, error shown otherwise. 
- Unset using _unset_ command, error shown if not able to.

##### `fg <jobnumber>`
- The process\-id is retrieved using the _procaarray_.If no job number is specified, most recent process is taken into consideration.
- a _SIGCONT_ signal is given to the process and the shell is made to wait for the child using _waitpid_.  Ctrl+Z in the process is also handled.
- tcsetpgrp is used to give control to the child process.

##### `bg <jobnumber>`
- The process\-id is retrieved using the _procaarray_.
- _SIGCONT_ is given to the the process using kill command.

##### `overkill`
- Takes no arguments. Error shown otherwise.
- Iterates over the process array, keeps giving _SIGINT_ signal until each one of them killed.

##### `kjob <jobnumber> <signalnumber>`
- Takes input the job number and signal number. If no such job number exists or signal is invalid, error is thrown up.
- Identifes the process from job number through the procaarray list.

##### `jobs`
- Prints the list of background jobs along with their status.
- Prints error if any argument given

##### `quit`
- Will make the shell quit.

##### `cronjob -c <command> -t <time-period> -p <total-time>`
- Takes input a single word command which will execute after _time-period_ interval until _total-time_.
- Treated as background process.

### Additional features implemented
- UP arrow key
- Use of '&' to put process in background
- Implementing piping and redirection .
- Ctrl+Z putting process in background
- Ctrl+C or any such signal ignored by parent process, only exits using `quit` command
