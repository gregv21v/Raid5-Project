/*
 * filetable.c
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure.
 */

#include <stdio.h>
#include <stdlib.h>


/*
	Saving files
	-------------
	Each block is 42 bytes large.
	12 file descriptors are stored in a block.
	The last 8 bytes store the address of the next block.
*/

#include "filetable.h"


int currentBlockNumber = 0;
char * currentBlock = NULL; /* Buffer to store the currently selected block temporarily. */



/*
	Adds a file to the list
	pre: File points to a valid file_t struct in memory, and table points to a valid list_t in memory.
	post: Adds the file to the end of the table.
*/
void filetable_add_file(list_t * table, file_t * )
{

	/* find the last entry in the file table */
	fileDescriptor_t * descriptors[FILES_PER_BLOCK];
	
	
	int i = 0; 
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		memcpy(descriptors[i]->name, currentBlock
	}
	
	
	/* add the file at that position */
}

/*
	Displays the file names along with all the other data in the file_t struct.
	pre: table points to a valid list_t struct.
	post: Displays the file names, and other details about the files in the table.
*/
void filetable_display(list_t * table) 
{

	file_t * current = table->head;

	while(current != NULL)
	{
		/* display the details of the current file */
		printf("%s\n", current->name);
		printf("Start: %d\n", current->start);
		printf("Block Count: %d\n", current->blockCount);
		printf("Disk Number: %d\n", current->diskNumber);

		current = current->next;
	}

}

/*
	Displays the file names. This is used for the ls command.
	pre: table points to a valid list_t struct.
	post: Displays the file names.
*/
void filetable_list_files(list_t * table)
{
	file_t * current = table->head;

	while(current != NULL)
	{
		/* display the details of the current file */
		printf("%s\n", current->name);

		current = current->next;
	}
}


/*
	Finds a file by name in the table.
	pre: The table refers to a valid list_t struct, and the filename is a valid string
	post: Returns a pointer to the file found or NULL if not found
*/
file_t * filetable_find(list_t * table, char * filename) 
{
	file_t * current = table->head;

	for(; strcmp(current->name, filename) != 0; current = current->next);

	return current;
}




