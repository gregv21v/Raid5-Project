/* Authors: Andrew Joyal, Evan Vadenais, Greg Venezia */

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <disk.h>
#include <unistd.h>

#include "filetable.h"

/* Define the maximum command length */
#define CMDLEN	1024



/*Define the disk names*/
#define DISK_0 "disk_0"
#define DISK_1 "disk_1"
#define DISK_2 "disk_2"




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
char * write_table(list_t * table); 

int main(int argc, char **argv)
{
	char command[CMDLEN];
	char ** arguments;
	int argumentCount;
		
	int error;

	/* initialize files */
	list_t * table = filetable_create();

	file_t * files[5];

	int i;
	for(i = 0; i < 5; i++)
	{
		files[i] = (file_t *) malloc(sizeof(file_t));
	}

	strcpy(files[0]->name, "File1");
	strcpy(files[1]->name, "File2");
	strcpy(files[2]->name, "File3");
	strcpy(files[3]->name, "File4");
	strcpy(files[4]->name, "File5");
	
	
	


	for(i = 0; i < 5; i++)
	{
		filetable_add_file(table, files[i]);
	}
	
	
	char * buf = write_table(table);
	display_buffer(buf, 18 * table->size);



  	printf("Welcome to your file system\n");

	while(strcmp(command, "exit") != 0 && strcmp(command, "quit") != 0)
	{
		get_command(command);
		build_argument_array(&arguments, &argumentCount, command);

		if(strcmp(command, "ls") == 0) 
		{
			filetable_list_files(table);
			/*filetable_display(table); */
		} 
		else if(strcmp(arguments[0],"makedisk") == 0)/* create the three disks*/
		{
			error = make_disk("disk_0");
			if(error == 0)
			{
				printf("Disk created\n");
			}
			error= make_disk("disk_1");
			if(error==0)
			{
				printf("Disk created\n");
			}
			error=make_disk("disk_2");
			if(error==0)
			{
				printf("Disk created\n");
			}
		
		}
		/* writes a file that is on the local filesystem to the disk */
		else if(strcmp(arguments[0],"write") == 0)
		{
			/* load a file from the local filesystem */
			
			error = open_disk("DISK_0");
			error = open_disk("DISK_1");
			error = open_disk("DISK_2");


			/* write to the disks*/
			/* find the last entry in the file table */
			/* store the new file after that entry */
			

			error=close_disk("DISK_0");
			error=close_disk("DISK_1");
			error=close_disk("DISK_2");

			
		}
		/* Read a file from the filesystem */
		else if(strcmp(arguments[0],"readfile")==0)
		{
			char buffer [512]; /*Buffer to read from the block*/
			file_t* file = filetable_find(table, arguments[1]);
			int startBlock = file->start;
			int currentBlock = startBlock;
			int endBlock = startBlock + (file->blockCount);
			while(currentBlock <= endBlock) /*Iterate through the blocks of the file*/
			{
				int i=0;
				block_read(currentBlock,buffer); /*Read the block into the buffer string*/
				
				/* while the character is not the termination character */
				while( buffer[i] != '\0') /*Print out the block*/
				{
					printf("%c", buffer[i]);
					i++;
				}
				currentBlock++;
			}
		}
		else if(strcmp(command,"exit") != 0 && strcmp(command,"quit") != 0)
		{
			/*We can change the creation of files later if we want*/	
			error = execvp(arguments[0],arguments);
			if(error == -1)
			{
				printf("Execution failed\n");
			}
		}
	}
	
	
  	return 0;

}




char * write_table(list_t * table)
{
	int size = 38 * table->size; /* 38 is the file descriptor size in bytes */
	char * buffer = (char *) malloc(size);
	int current = 0; /* the position you are writing to in the buffer */
	int error = 0;

	int i;
	file_t * currentNode = table->head;
	
	/* Write all the file descriptors to the buffer */
	while(currentNode != NULL) 
	{
		/* Write the name */
		memcpy(buffer+current, currentNode->name, 29);
		current += 29;
		
		/* Write the start */
		memset(buffer+current, currentNode->start, 4);
		current += 4;
		
		/* Write the blockCount */
		memset(buffer+current, currentNode->blockCount, 4);
		current += 4;
		
		/* Write the disk number */
		memset(buffer+current, currentNode->diskNumber, 1);
		current += 1;
		
		currentNode = currentNode->next;
	}
	
	/* Store the data in the volumn */
	
	return buffer;
	
	
}


void display_buffer(char * buffer, int size) 
{
	int i = 0;
	for(i = 0; i < size; i++) 
	{
		printf("%c", buffer[i]);
	}
}


/* list_t * load_table() */
/* 	Adds a new file to the volumn */
/* void store_in_volumn(file_t * file, char * data, int dataSize) */
/*Loads the file from the local file system */
/* char * load_file(char * filename) */






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
