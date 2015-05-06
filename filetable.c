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
	table->firstFileBlock = descriptorBlock_load(0); 
	table->currentFile = NULL;
}


void filetable_add_file(table_t * table, char * name, int blockCount)
{
	int index = descriptorBlock_find_file(table->lastFileBlock, "");
	
	if(index != -1) 
	{
		/* There is space in the existing block to place this file */
		memcpy(table->lastFileBlock->descriptors[index]->name, name, MAX_NAME_LENGTH);
		table->lastFileBlock->descriptors[index]->start = descriptorBlock_find_last_free(table->lastFileBlock);
		table->lastFileBlock->descriptors[index]->blockCount = blockCount;
		
		/* update the block on the disk */
		descriptorBlock_store(table->lastFileBlock);
	}
	else 
	{
		/* a new block needs to be created to add this file */
		descriptorBlock_t * newBlock = descriptorBlock_create(descriptorBlock_find_last_free(table->lastFileBlock));
	
		descriptorBlock_attach(table->lastFileBlock, newBlock);
		
		/* update the blocks on the disk */
		descriptorBlock_store(table->lastFileBlock);
		descriptorBlock_store(newBlock);
		
		free(table->lastFileBlock);

		table->lastFileBlock = newBlock;
	}
}

void filetable_list_files(table_t * table)
{
	descriptorBlock_t * current = table->lastFileBlock;
	
	while(current->nextBlock != 0)
	{
		descriptorBlock_list_files(current);
		if(current->nextBlock != 0)
			current = descriptorBlock_store(current->nextBlock);
	}
}

void filetable_find_file(table_t * table, char * name)
{
	
}








