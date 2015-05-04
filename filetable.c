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
/*
	Important Facts
	----------------
	
	

*/



#include "filetable.h"



/*
	fileBlock 
	previous block address | descriptor data | next block addresss 


*/
/* 
	Finding the next avaliable address:
		


/*
	Adds a file to the list
	pre: Name should be of length NAME_MAX_LENGTH
	post: Adds the file to the end of the table.
*/
int filetable_add_file(char * name, int blockCount)
{
	int i = 0; /* A general iterator for reuse */
	
	
		
	
	char found = 0;
	int index = 0; /* the index where the next empty file descriptor is located. */
	char * currentBlock = volume_load(0);
	int currentBlockAddress = 0;
	int nextFreeBlock = 0;
	descriptorBlock_t * descriptorBlock;
	
	/* find the last file descriptor */
	/*
		found = 0 <-- Last not found
		found = 1 <-- Last found in current block
		found = 2 <-- Next descriptor will start at the beginning of a new block
	*/
	while(!found) 
	{
		descriptorBlock = convertToDescriptorBlock(currentBlock);
		
		/* Check to see if the file is in this block */
		/* "" means you have an empty file descriptor */
		for(i = 0; i < FILES_PER_BLOCK && !found; i++) 
		{
			/* you have found the last file */
			if(strcmp(descriptorBlock->descriptors[i]->name, "") == 0) 
			{
				index = i;
				found = 1;
			}
		}
		
		if(!found) 
		{
			if(descriptorBlock->nextBlock != 0) 
			{
				currentBlock = volume_load(descriptorBlock->nextBlock);
				currentBlockAddress = descriptorBlock->nextBlock;
			}
			else /* next descriptor will start on new block */
				found = 2;
		}
	}
	
	if(found == 2) {
		/* Get a new address for the nextBlock */
		descriptorBlock->nextBlock = descriptorBlock->descriptors[FILES_PER_BLOCK-1]->start 
					+ descriptorBlock->descriptors[FILES_PER_BLOCK-1]->blockCount;
					
		/* Create a new descriptor block */
		descriptorBlock_t * newDescriptorBlock = (descriptorBlock_t *) malloc (sizeof(descriptorBlock_t));
		newDescriptorBlock->previousBlock = currentBlockAddress;
		newDescriptorBlock->nextBlock = 0;
		
		
		memcpy(newDescriptorBlock->descriptors[i]->name, name, NAME_MAX_LENGTH);
		newDescriptorBlock->descriptors[i]->start = descriptorBlock->nextBlock + 1;
		newDescriptorBlock->descriptors[i]->blockCount = blockCount;
		
		volume_store(descriptorBlock->nextBlock, convertToBlock(descriptorBlock));
		
		return newDescriptorBlock->descriptors[i]->start;
	}
	else 
	{
		memcpy(descriptorBlock->descriptors[index]->name, name, NAME_MAX_LENGTH);
		descriptorBlock->descriptors[index]->blockCount = blockCount;	
		if(index == 0) 
		{
			/* load the previous descriptor block */
			descriptorBlock_t * previous = convertToDescriptorBlock(volumn_load(descriptorBlock->previousBlock));

			descriptorBlock->descriptors[index]->start = previous->descriptors[FILES_PER_BLOCK-1]->start
									+ previous->descriptors[FILES_PER_BLOCK-1]->blockCount;
		}
		else 
		{
			descriptorBlock->descriptors[index]->start = descriptorBlock->descriptors[index-1]->start 
									+ descriptorBlock->descriptors[index-1]->blockCount;
		}
	}
	
}

char * volume_load(int blockNumber)
{
		
	
	return NULL;
}




void volume_store(int address, char * data)
{
	
}

descriptorBlock_t * convertToDescriptorBlock(char * block)
{
	descriptorBlock_t * block = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	
	int offset = 0;
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(block->previousBlock, block, 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		memcpy(descriptors[i]->name, currentBlock + offset, 29);
		offset += 29;
		
		/* Copy the start block address from the block */
		memcpy(descriptors[i]->start, currentBlock + offset, 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(descriptors[i]->blockCount, currentBlock + offset, 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(block->nextBlock, block+offset, 4);
	
	return block;
}


char * convertToBlock(fileDescriptors * descriptors)
{
	char * buffer = (char *) malloc(512)
	
	int offset = 0;
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(block, block->previousBlock, 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		memcpy(currentBlock + offset, descriptors[i]->name, 29);
		offset += 29;
		
		/* Copy the start block address from the block */
		memcpy(currentBlock + offset, descriptors[i]->start, 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(currentBlock + offset, descriptors[i]->blockCount, 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(block+offset, block->nextBlock, 4);
	
	return block;
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
file_t * filetable_find(char * filename) 
{
	file_t * current = table->head;

	for(; strcmp(current->name, filename) != 0; current = current->next);

	return current;
}




