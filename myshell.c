/**
	@file
	@author Julia Frances <jrf3433@drexel.edu>
	@date November 12, 2022
	@section DESCRIPTION
 
	This program is the first part of my shell project. It includes tokenizing, forking, and a built-in CD command.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

char** msTokenize(char *input);
void msExec(char **commands);
void msChDir(char **commands);

int main(int argc, char **argv)
{
	char input[256];
	char **commands;
	// main loop, always true 
	while(1){
		printf("$ "); // prompt
		fgets(input,256,stdin);
		if(feof(stdin)){
			break;
		}
		commands = msTokenize(input);
		msExec(commands);
	}

	return EXIT_SUCCESS;
}	

// tokenization function for command line input 
char** msTokenize(char *input)
{
	char **tokens = malloc(256*sizeof(char*));
	char *limits = " \t\n";
	char *token = strtok(input,limits);
	int position = 0;
	while(token != NULL){
		tokens[position] = token;
		token = strtok(NULL,limits);
		position++;
	}
	tokens[position] = NULL;
	return tokens;
}

// execution function for launching commands, forking into parent and child processes 
void msExec(char **commands)
{	
	// if the command line input is cd, use built-in cd function
	if(strcmp(commands[0],"cd") == 0){
		msChDir(commands);
		return;
	}
	
	// otherwise, fork as usual	
	pid_t t = fork();
	if(t == 0){
		execvp(commands[0],commands);
		perror("msExec");
		exit(1);
	}else{
		int status;
		waitpid(t, &status, WUNTRACED); // WUNTRACED so parent can return regardless of child outcome
	}
	
}

// built-in cd function
void msChDir(char **commands)
{
	// if no command after "cd", cannot change directory to anything
	if(commands[1] == NULL){
		fprintf(stderr, "Not enough arguments.");
	// run function as normal
	}else{
		if(chdir(commands[1]) != 0){ // should return 0 on success 
			perror("msChDir");
		}
	}
}
