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




