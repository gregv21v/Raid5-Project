/*
 * filetable.c
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure.
 */


#include "filetable.h"

table_t * filetable_create()
{
	table_t * table = (table_t *) malloc(sizeof(table_t));
	
	/* find the last descriptor block */
	table->lastFileBlock = descriptorBlock_load_last();
	/* TODO: Add condition for empty file table */
	table->currentFileBlock = descriptorBlock_load(0); 
	table->currentFile = NULL;
}


int filetable_find_last_free(table_t * table)
{
	int index = descriptorBlock_find_file(table->lastFileBlock, "");
	if(index != -1) 
		return table->lastFileBlock->descriptors[index]->start 
			+ table->lastFileBlock->descriptors[index]->blockCount;
}

void filetable_add_file(table_t * table, char * name, int blockCount)
{
	int index = descriptorBlock_find_file(table->lastFileBlock, "");
	
	if(index != -1) 
	{
		memcpy(table->lastFileBlock->descriptors[index]->name, name, MAX_NAME_LENGTH);
		table->lastFileBlock->descriptors[index]->start = filetable_find_last_free(table);
		table->lastFileBlock->descriptors[index]->blockCount = blockCount;
	}
}



/* private function */
descriptorBlock_t * filetable_find_last_descriptor_block()
{
	descriptorBlock_t * last = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	descriptorBlock_t * current
	
	
	descriptorBlock = load_descriptor_block(currentBlock);
	
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
	
}

/*
	Adds a file to the list
	Whenever you add a new file, add an empty file after it. ()
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
		descriptorBlock = load_descriptor_block(currentBlock);
		
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

file_t * nextFile(table_t * table)
{
	
}



descriptorBlock_t * load_descriptor_block(int address)
{
	
}


char * convertToBlock(fileDescriptors * descriptors)
{

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




