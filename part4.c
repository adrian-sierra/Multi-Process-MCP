#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "string_parser.h"

char *display_vmsize(){
    char *vmsize = malloc(sizeof(char) * 128);
    char filename[1000];
    
    sprintf(filename, "/proc/%d/status", getpid());

    FILE *f = fopen(filename, "r");
    char arr[1024][128];
    
    int c = 0;
    while(fscanf(f, "%s", arr[c]) != EOF)
   	 c++;
    
    for (int t = 0; t < c; t++){
   	 if (strcmp(arr[t], "VmSize:") == 0)
   		 snprintf(vmsize, 128, "%s %s", arr[t+1], arr[t+2]);
    }
    fclose(f);
    
    return vmsize;
}

char *display_stack(){
    char *stack = malloc(sizeof(char) * 128);
    char filename[1000];
    
    sprintf(filename, "/proc/%d/statm", getpid());

    FILE *f = fopen(filename, "r");
    char arr[1024][128];
    
    int c = 0;
    while(fscanf(f, "%s", arr[c]) != EOF)
   	 c++;
    
    for (int t = 0; t < c; t++){
   	 if(t == 5)
   		 snprintf(stack, 128, "%s", arr[t]);
    }
    fclose(f);
    
    return stack;
}

char *display_run(){
    char *run = malloc(sizeof(char) * 128);
    char filename[1000];
    
    sprintf(filename, "/proc/%d/sched", getpid());

    FILE *f = fopen(filename, "r");
    char arr[1024][128];
    
    int c = 0;
    while(fscanf(f, "%s", arr[c]) != EOF)
   	 c++;
    
    for (int t = 0; t < c; t++){
   	 if (strcmp(arr[t], "se.sum_exec_runtime") == 0)
   		 snprintf(run, 128, "%s", arr[t+2]);
    }
    fclose(f);
    
    return run;
}

char *display_start(){
    char *start = malloc(sizeof(char) * 128);
    char filename[1000];
    
    sprintf(filename, "/proc/%d/stat", getpid());

    FILE *f = fopen(filename, "r");
    char arr[1024][128];
    
    int c = 0;
    while(fscanf(f, "%s", arr[c]) != EOF)
   	 c++;
    
    for (int t = 0; t < c; t++){
   	 if (t == 21)
   		 snprintf(start, 128, "%s", arr[t]);
    }
    fclose(f);
    
    return start;
}

char *display_processor(){
    char *processor = malloc(sizeof(char) * 128);
    char filename[1000];
    
    sprintf(filename, "/proc/%d/stat", getpid());

    FILE *f = fopen(filename, "r");
    char arr[1024][128];
    
    int c = 0;
    while(fscanf(f, "%s", arr[c]) != EOF)
   	 c++;
    
    for (int t = 0; t < c; t++){
   	 if (t == 38)
   		 snprintf(processor, 128, "%s", arr[t]);
    }
    fclose(f);
    
    return processor;
}

void table(){
    char *vmsize = display_vmsize();
    char *stack = display_stack();
    char *run = display_run();
    char *start = display_start();
    char *processor = display_processor();

    printf("PID: %d	VMSIZE: %s	STACK SIZE: %s kB	RUN TIME: %s ms	START TIME: %s	PROCESSOR: %s\n", getpid(), vmsize, stack, run, start, processor);
    
    free(vmsize);
    free(stack);
    free(run);
    free(start);
    free(processor);    
}

typedef struct
{
	char** command_arguments;
	char* command;
	int num_token;

}c_line;

void sig_handler (int signal){
}

void signaler(pid_t* pid_ary, int size, int sig){
    sleep(2);
    for(int i = 0; i < size; i++)
   	 kill((int)pid_ary[i], sig);
}

int count_commands(char *filename){
    FILE *file;
    size_t len = 128;
    char *line = malloc(len);

    file = fopen(filename, "r");

    int count = 0;
    while(getline(&line, &len, file) != -1)
   	 count++;

    free(line);

    return count;
}

int main(int argc, char **argv){
    if (argc < 2){
   	 printf("Error!: Missing file\n");
   	 exit(0);
    }

    int lines = count_commands(argv[1]);

    c_line *cmds = malloc(sizeof(c_line) * lines);

    FILE *file;
    size_t len = 128;
    char *line = malloc(len);

    file = fopen(argv[1], "r");

    int c = 0;
    int slen;


    while(getline(&line, &len, file) != -1){

    	char *buf2 = strtok(line, "\n");

    	int count = count_token(buf2, " ");
    	cmds[c].num_token = count;

    	char *s = strdup(buf2);
    	char *token;
    	char *rest = s;

   	 cmds[c].command_arguments = malloc(sizeof(char*) * count);

    	int i = 0;

    	while ((token = strtok_r(rest, " ", &rest))){
   		 if (token != NULL){
            	slen = strlen(token) + 1;
            	cmds[c].command_arguments[i] = malloc(sizeof(char) * slen);
            	strcpy(cmds[c].command_arguments[i],token);
   		 }
        	i++;
    	}
    	cmds[c].command = cmds[c].command_arguments[0];
    	cmds[c].command_arguments[i] = NULL;

    	c++;
    	free(s);
    }

    pid_t *pid_ary = malloc(sizeof(pid_t) * lines);

	signal(SIGUSR1, sig_handler);
	signal(SIGSTOP, sig_handler);
	signal(SIGCONT, sig_handler);

	int sig;
	sigset_t set;

	for (int i = 0; i < lines; i++){
   	 pid_ary[i] = fork();
   	 if (pid_ary[i] == -1)
   		 perror("fork");
    	if (pid_ary[i] == 0){
   		 sigwait(&set, &sig);
   		 if (sig == SIGUSR1){
   			 table();
   			 if (execvp(cmds[i].command, cmds[i].command_arguments) == -1){
   				 perror("Error!");
   				 exit(0);
   			 }
   		 }
    	}
    }

	signaler(pid_ary, lines, SIGUSR1);
	signaler(pid_ary, lines, SIGSTOP);
	sleep(5);
	signaler(pid_ary, lines, SIGCONT);

	for (int x = 0; x < lines; x++){
   	 wait(NULL);
	}

    for(int t = 0; t < lines; t++){
   	 for(int y = 0; y < cmds[t].num_token; y++){
   		 free(cmds[t].command_arguments[y]);
   	 }
   	 free(cmds[t].command_arguments);
    }

    free(cmds);
    free(pid_ary);
    free(line);
    fclose(file);

    return 0;
}





