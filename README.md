# Multi-Process-MCP
This project was completed during my Operating Systems course. 

The concept of the project is take an input file that lists various commands that should be performed by the CLI and would execute if they are valid commands with valid arguments. 

The project adds a multi process feature by having multiple processes working on the input file and thus increasing the efficiency of the application. There is also involvment of a parent process and several child processes that would work on the execution of the commands.

There was an added element of scheduling, such as a CPU scheduler that would make the child processes wait for a signal from the parent process that would allow them to begin or pause their work. Once a child was finished, the parent process would no longer continue to call on them, but instead tend to the child processes that still had work to do.

All the code was written using C and was intented to be ran in Linux OS.
