#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GNU_SOURCE

int count_token (char* buf, const char* delim)
{
	char *s = strdup(buf);
	char *token;
	char *rest = s;

	int count = 0;
	while ((token = strtok_r(rest, delim, &rest))){
    	count++;
	}
    
	free(s);
	return count + 1;
}

command_line str_filler (char* buf, const char* delim)
{
	command_line cmd;

	char *buf2 = strtok(buf, "\n");
	int count = count_token(buf2, delim);

	cmd.num_token = count;

	char *s = strdup(buf2);
	char *token;
	char *rest = s;

	cmd.command_list = malloc(sizeof(char*) * count);

	int len;
	int c = 0;
   
    
	while ((token = strtok_r(rest, delim, &rest)))
	{
   	 if (token != NULL){
        	len = strlen(token) + 1;      	 
        	cmd.command_list[c] = malloc(sizeof(char) * len);
        	strcpy(cmd.command_list[c],token);
   	 }
    	c++;
	}
	cmd.command_list[c] = NULL;
	free(s);
	return cmd;
}

void free_command_line(command_line *command)
{
	for (int i = 0; i < command->num_token; i++){
    	free(command->command_list[i]);
	}
	free(command->command_list);
}

/*
int main(){
	char *buf = "pwd ; mkdir test ; cd test";
	//char *buf = " mkdir test ";
	//char *buf = "sfjlksagjlks;ls; pwd;";
	//const char *delim = " ";
	const char *delim = ";";

	command_line line = str_filler(buf, delim);

	//free_command_line(&line);
//	for (int i = 0; i < line.num_token; i++){
//    	printf("%s\n", line.command_list[i]);
//	}

//	printf("%d\n", count_token(buf, delim));

	//printf("%d\n", count_token("pwd ; mkdir test ; cd test", ";"));    
	return 0;
}

*/



