
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* Define the maximum command length */
#define CMDLEN	1024
#define MAX_FILES 20


typedef struct File {
	char name[30];
	unsigned int start;
	unsigned int blockNumber;
	unsigned char diskNumber;
} file_t;


/* List of Commands: 
	ls
	makedisk [disk-name]
	opendisk [disk-name]
	closedisk [disk-name]
	writefile [file-name] [local-file-name]
	readfile [file-name] 

*/


void sanitize_string(char *);
int build_argument_array(char***, int*, char*);
void get_command(char *);
void list_files(file_t * files);
void write_table(file_t * table);

int main(int argc, char **argv)
{

	file_t table[MAX_FILES];

	/* initialize files */
	int i = 0;
	for(i = 0; i < MAX_FILES; i++) 
	{
		strcpy(table[i].name, "");
		table[i].start = 0;
		table[i].blockNumber = 0;
		table[i].diskNumber = 0;
	}

	strcpy(table[0].name, "Car Facts");
	strcpy(table[1].name, "Some other stuff");
	strcpy(table[2].name, "Something else");

	printf("Unsigned Int Length: %d\n", sizeof(unsigned int));


	char ** arguments;
	int argumentCount;
	
	char command[CMDLEN];
  	printf("Welcome to your file system\n");

	while(strcmp(command, "exit") != 0 && strcmp(command, "quit") != 0)
	{
		get_command(command);
		build_argument_array(&arguments, &argumentCount, command);

		if(strcmp(command, "ls") == 0) {
			list_files(table);
		}
	}
	
	
  	return 0;

}


void write_table(file_t * table) 
{
	
}
/* load_table(file_t * table) */
/* find_in_table(char * filename) */





void list_files(file_t * table) 
{
	int i = 0; 
	for(i = 0; i < MAX_FILES; i++) 
	{
		if(strcmp(table[i].name, "") != 0)
			printf("%s\n", table[i].name);
	}
}


/*
 * get_command
 * Description: This function fills the array cmd with the 
 * 		command run from the comamnd line. The
 * 		command line itself has no trailing spaces.
 */
void get_command(char *cmd)
{
	/* Zero the cmd buffer. */
	memset(cmd, CMDLEN, 0);
	
	/* Prompt for the command. */
	printf("msh> ");
	fgets(cmd, CMDLEN, stdin);

	/* Strip a trailing newline. */
	cmd[strlen(cmd) - 1] = '\0';

	sanitize_string(cmd);
}

/*
 * sanitize_string
 * Description: This function removes all the leading and trailing
 * 		spaces from a string.
 * Pre: cmd is not NULL
 * Post: The cmd pointed to is stripped of leading and trailing white space.
 */
void sanitize_string(char* cmd)
{
	int idx;		/* Use to track position in the string. */

	/* Strip all trailing white space. */
	for(idx = strlen(cmd) - 1; isspace(cmd[idx]) != 0 && 
	    idx >= 0; idx--)
		cmd[idx] = '\0';
	
	/* Strip leading white space. */
	while (isspace(cmd[0]) != 0)
		for (idx = 0; idx < strlen(cmd) - 1; idx++)
			cmd[idx] = cmd[idx + 1];
}

/*
 * build_argument_array
 * Description: This function takes a command an breaks it up into
 * 		an array of arguments. The last command is NULL.
 * Pre: cmd != NULL
 * Post: argc holds the number of arguments and argv holds the
 * 	 command line arguements themselves. The function returns
 * 	 ENOMEM on bad allocations and 0 on success.
 */
int build_argument_array(char***argv, int* argc, char* cmd)
{
	int idx;	/* The current location in cmd. */
	char *tok;	/* The current token. */
	
	assert(cmd != NULL);	/* Make sure the command is NOT NULL. */
	
	/* 
	 * Determine the number of arguemtns by counting the number
 	 * of spaces.
 	 */
	*argc = 1;
	for (idx = 0; idx < strlen(cmd); idx++)
		if (cmd[idx] == ' ')
			(*argc)++;

	/*
 	 * Allocate the space for you argument array.
 	 */
	*argv = (char **)calloc(*argc + 1, sizeof (char *));

	/* Check to see if the allocation succeeded. */
	if (*argv == NULL)
		return ENOMEM;

	/* Set all strings to NULL. */
	for (idx = 0; idx < *argc + 1; idx++)
	    (*argv)[idx] = NULL;


	/* 
	 * Walk through the command creating each necessary
 	 * command.
 	 */
	idx = 0;
	tok = strtok(cmd, " ");
	while(tok != NULL)
	{
		/* Copy the string into the argument array. */
		(*argv)[idx] = (char *)calloc(strlen(tok) + 1,  sizeof (char));
		strncpy((*argv)[idx], tok, strlen(tok));
 		idx++;

 		/* Continue tokenizing the same strings. */
		tok = strtok(NULL, " ");
	}

	return 0;
}
