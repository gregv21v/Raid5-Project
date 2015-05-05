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

int main(int argc, char **argv)
{
	char command[CMDLEN];
	char ** arguments;
	int argumentCount;
		
	int error;

	/* initialize table */
	list_t * table = filetable_create();



  	/* Load the file table into memory */
  	/* Start the rebuild thread */

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
			else
			{
				printf("Error creating disk 1.\n");
			}
			
			error= make_disk("disk_1");
			if(error==0)
			{
				printf("Disk created\n");
			}
			else
			{
				printf("Error creating disk 2.\n");
			}
			
			error=make_disk("disk_2");
			if(error==0)
			{
				printf("Disk created\n");
			}
			else
			{
				printf("Error creating disk 3.\n");
			}
		
		}
		/* writes a file that is on the local filesystem to the disk */
		else if(strcmp(arguments[0],"write") == 0)
		{
			if()
			
			
			char* buffer; /*The buffer to hold the text of the file*/
			int fileSize; /*the size of the file*/
			int numBlocks; /*number of blocks the file will require*/
			int startBlock; /*The block the file will start on*/
			int endBlock; /*The block the file will end on*/
			
			/*Open the file and get its size*/
			FILE* f=fopen(arguments[1],"w");
			fseek(f,0,SEEK_END);
			fileSize=ftell(f);
			numBlocks= (fileSize%512)+1;/*calculate the blocks needed*/
			fseek(f,0,SEEK_SET);
			
			buffer=(char *)malloc(sizeof(char) * (fileSize+1));/*Allocate space for the buffer*/
			fread(buffer,fileSize,1,f);/*Read the file into the buffer*/

			/*Create new file_t and add to table*/
				file_t* newFile;
				newFile=(file_t*)malloc(sizeof(file_t));
				strcpy(newFile->name,arguments[1]);
				newFile->blockCount=numBlocks;
				newFile->start=filetable_add_file(table,newFile->name,newFile->blockCount);
				printf("file added to table\n");
			
			endBlock=(newFile->start)+(newFile->blockCount);
			
			/*Open the disks*/
			error = open_disk(DISK_0);
			error = open_disk(DISK_1);
			error = open_disk(DISK_2);

			/*Write the blocks*/
			error=0;
			for(i=(newFile->start);i<(endBlock+1);i++)
			{
				error+=block_write(i,buffer);
			}
			
			/*remove file if all writes not successful*/
			if(error!=0)
			{
				filetable_remove_file(table,newFile->name);
				printf("Failed to execute all writes\n");
			}
			
			free(buffer);
			
			/*Close the disks*/
			error=close_disk(DISK_0);
			error=close_disk(DISK_1);
			error=close_disk(DISK_2);

		}
		else if(strcmp(arguments[0],"readfile")==0)
		{
			char buffer [512]; /*Buffer to read from the block*/
			file_t * file = filetable_find(table, arguments[1]);
			if(file!=NULL)/*Read the file if it was found*/
			{
				int startBlock = file->start;
				int currentBlock = startBlock;
				int endBlock = startBlock + (file->blockCount);
				
				while(currentBlock<=endBlock)/*Iterate through the blocks of the file*/
				{
					int i = 0;
					block_read(currentBlock, buffer);/*Read the block into the buffer string*/
				
					/* while the character is not the termination character */
					while(buffer[i] != '\0')/*Print out the block*/
					{
						printf("%c", buffer[i]);
						i++;
					}
					currentBlock++;
				}
			}
			else /*The file was not found*/
			{
				printf("File not found\n");
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
	
	/* Store the file table on disks 1 and 2 */
	
  	return 0;

}




/* void write_table(file_t * table) */
/* list_t * load_table() */
/* 	Adds a new file to the volumn
	store_in_volumn(file_t * file, char * data, int dataSize) */
/*Loads the file from the local file system */






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
