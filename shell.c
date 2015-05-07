/*test
 * shell.c
 * Author: Andrew Joyal, Evan Vadenais, Greg Venezia 
 * Date: 5/6/2015
 * Course: CSC3320
 * Description: The shell of the program.
 */

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <disk.h>
#include <unistd.h>
#include <libgen.h>

#include "filetable.h"
#include "volume.h"

/* Define the maximum command length */
#define CMDLEN	1024


int rebuild_disk(int);
void * threadHandler();
void sanitize_string(char *);
int build_argument_array(char***, int*, char*);
void get_command(char *);

pthread_mutex_t lock;
table_t * table;

int main(int argc, char **argv)
{
	char command[CMDLEN]; /* command string */
	char ** arguments; /* arguments of the command */
	int argumentCount; /* number of arguments the command has */
	int error; /* for disk errors */

	/* initialize table */


	/* Thread stuff */
	pthread_t tid; /* threads id */
	
	pthread_mutex_init(&lock, NULL);
	
	/* Create the thread, giving it threadHandler() as a function */
	pthread_create(&tid, NULL, threadHandler, NULL);
	sleep(1);
  	/* Start the rebuild thread */
	
	while(strcmp(command, "exit") != 0 && strcmp(command, "quit") != 0)
	{
		pthread_mutex_lock(&lock);
		get_command(command);
		pthread_mutex_unlock(&lock);
		build_argument_array(&arguments, &argumentCount, command);
		
		if(strcmp(command,"ls")==0)/*List contents of table*/
		{
			filetable_list_files(table);
		}
		else if(strcmp(command,"format") == 0)/* create the three disks*/
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
		else if(strcmp(arguments[0],"write") == 0)/*Write files to the disks*/
		{
			char* buffer; /*The buffer to hold the text of the file*/
			char* fileName;
			int fileSize; /*the size of the file*/
			int numBlocks; /*number of blocks the file will require*/
			int startBlock; /*The block the file will end on*/
			int i; /*iterator*/
			int j;
			int error=0;
			
			/*Open the file and get its size*/
			FILE* f = fopen(arguments[1],"r");
			fileName=arguments[2];
			fseek(f,0,SEEK_END);
			fileSize = ftell(f);
			numBlocks = (int) (fileSize/512);
			if(fileSize%512 != 0)
			{
				numBlocks++;
			}
			printf("The number of Blocks: %d\n",numBlocks);
			fseek(f,0,SEEK_SET);
			
			buffer = (char *)malloc(sizeof(char) * (fileSize+1));/*Allocate space for the buffer*/
			fread(buffer,sizeof(char),fileSize+1,f);/*Read the file into the buffer*/
			
			for(i=0;i<fileSize+1;i++)
			{
				printf("%c",buffer[i]);
			}
			
			/*Add fileto the table*/
			startBlock = filetable_add_file(table,fileName, numBlocks);
			
			for(i = startBlock, j = 0; i <= (startBlock+numBlocks); i++, j++)/*Write to the volume*/
			{
			
				volume_store_block(i, buffer + (j*512));
			}
		
			if(error!=0)
			{
				/*filetable_remove_file(table,newFile->name);*/
				printf("Failed to execute all writes\n");
			}
			
			free(buffer);/*Free the buffer*/
			fclose(f);
		}
		else if(strcmp(arguments[0],"readfile")==0)
		{
			file_t * file = filetable_find_file(table, arguments[1]);
			if( file != NULL)/*Read the file if it was found*/
			{
				int startBlock = file->start;
				int currentBlock = startBlock;
				int endBlock = startBlock + (file->blockCount);
				printf("Block count: %d",file->blockCount);
				while(currentBlock<=endBlock)/*Iterate through the blocks of the file*/
				{
					volume_display_block_raw(currentBlock);
					currentBlock++;/*Iterate through the blocks of the file*/
				}
			}
			else /*The file was not found*/
			{
				printf("File not found\n");
			}
		}
		else if(strcmp(arguments[0],"delete")==0)
		{
			int error;
			error=filetable_remove_file(table,arguments[1]);
			if(error==-1)
			{
				printf("File not found\n");
			}
			else
			{
				printf("File was removed successfully\n");
			}
		}
		else if(strcmp(command,"exit") != 0 && strcmp(command,"quit") != 0)/*Some other command was given*/
		{
			/*
			error = execvp(arguments[0],arguments);
			if(error == -1)
			{
				printf("Execution failed\n");
			}*/
		}
	}

  	return 0;

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
	
	fflush(stdin);
	/* Prompt for the command. */
	printf("msh> ");
	fgets(cmd, CMDLEN, stdin);
	/*scanf("%s",cmd);*/

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

void* threadHandler()
{
	/* Check to see if all of the disks are there */
	/* if any disk is not there, note which one is gone */
	int open_error;
	int disk_number;
	int make_error;
	char choice;
	int disk_errors;
	
	while(1)
	{
		/* grab the lock */
		
		
		disk_errors = 0;
		disk_number = 0;
		make_error = 0;
		choice = 'n';
		
		/* try to open disk 0 */
		open_error = open_disk(DISK_0);
		if(open_error == -1)
		{
			disk_number = 0;
			disk_errors++;
		}
		close_disk();
		
		/* try to open disk 1 */
		open_error = open_disk(DISK_1);
		if(open_error == -1)
		{
			disk_number = 1;
			disk_errors++;
		}
		close_disk();
		
		/* try to open disk 2 */
		open_error = open_disk(DISK_2);
		if(open_error == -1)
		{
			disk_number = 2;
			disk_errors++;
		}
		close_disk();
		pthread_mutex_lock(&lock);
		printf("Locked\n");
		/* if all three are gone, we need to initialize the system */
		switch(disk_errors)
		{
		case 1:
			do
			{
				printf("Would you like to rebuild the disk? (y/n)\n");
				choice = getchar();
				
				if(choice == 'y')
				{
					printf("Rebuilding...");
					rebuild_disk(disk_number);
					printf("done\n");
				}
				
			}while(choice != 'y');
			break;
		case 2:
			/* nothing we can do here really */
			printf("We have encountered a problem with 2 out of 3 disks. Sorry, we cannot rebuild\n");
			break;
		case 3:
			/* prompt the user to initialize the three disks */
			do
			{
				printf("Would you like to initialize the system? (y/n)\n");
				choice = getchar();
				/*printf("%c",choice);*/
				if(choice == 'y')
				{
					printf("Initialize\n");
					table = filetable_create();
					
					make_error = make_disk("disk_0");
					if(make_error == 0)
					{
						printf("Disk created\n");
					}
					else
					{
						printf("Error creating disk 1.\n");
					}
					
					make_error = make_disk("disk_1");
					if(make_error == 0)
					{
						printf("Disk created\n");
					}
					else
					{
						printf("Error creating disk 2.\n");
					}
					
					make_error = make_disk("disk_2");
					if(make_error == 0)
					{
						printf("Disk created\n");
					}
					else
					{
						printf("Error creating disk 3.\n");
					}
				}
			}while(choice != 'y');
			
			break;
		default:
			/* no error, do nothing */
			break;	
		}/* end switch */
		
		pthread_mutex_unlock(&lock);
		sleep(1);
	}/* end outside while */
	
}

int rebuild_disk(int disk_number)
{
	/* a 512 byte section off a disk */
	char * buffer_a = (char *) malloc(512);
	char * buffer_b = (char *) malloc(512);
	char * parity_buffer = (char *) malloc(512);
	int block_number;
	int i;
	
	/* make a new disk */
	if(disk_number == 0)
	{
		make_disk(DISK_0);
		
		for(block_number = 0;block_number < DISK_BLOCKS;block_number++)
		{
			open_disk(DISK_1);
			block_read(block_number,buffer_a);
			close_disk();
			
			open_disk(DISK_2);
			block_read(block_number,buffer_b);
			close_disk();
			
			for(i = 0;i<512;i++)
			{
				parity_buffer[i] = buffer_a[i] ^ buffer_b[i];
			}
			
			open_disk(DISK_0);
			block_write(block_number,parity_buffer);
			close_disk();
		}
	}
	else if(disk_number == 1)
	{
		make_disk(DISK_1);
		printf("make 1\n");
		for(block_number = 0;block_number < DISK_BLOCKS;block_number++)
		{
			open_disk(DISK_0);
			block_read(block_number,buffer_a);
			close_disk();
			
			open_disk(DISK_2);
			block_read(block_number,buffer_b);
			close_disk();
			
			printf("enter for loop\n");
			for(i = 0;i<512;i++)
			{
				parity_buffer[i] = buffer_a[i] ^ buffer_b[i];
			}
			
			open_disk(DISK_1);
			block_write(block_number,parity_buffer);
			close_disk();
		}
	}else if(disk_number == 2)
	{
		make_disk(DISK_2);
		
		for(block_number = 0;block_number < DISK_BLOCKS;block_number++)
		{
			open_disk(DISK_0);
			block_read(block_number,buffer_a);
			close_disk();
			
			open_disk(DISK_1);
			block_read(block_number,buffer_b);
			close_disk();
			
			for(i = 0;i<512;i++)
			{
				parity_buffer[i] = buffer_a[i] ^ buffer_b[i];
			}
			
			open_disk(DISK_2);
			block_write(block_number,parity_buffer);
			close_disk();
		}
	}
	printf("free");
	free(buffer_a);
	free(buffer_b);
	free(parity_buffer);
	printf("free complete");
	
	return 0;
}
