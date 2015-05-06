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
	table->firstFileBlock = descriptorBlock_create(0);
	/* TODO: Add condition for empty file table */
	table->lastFileBlock = table->firstFileBlock; 
}


int filetable_add_file(table_t * table, char * name, int blockCount)
{
	int added = descriptorBlock_add_file(table->lastFileBlock, name, blockCount);
	
	printf("Added %d ? ", added);
	
	if(added == -1) 
	{
		/* a new block needs to be created to add this file */
		descriptorBlock_t * newBlock = descriptorBlock_create(descriptorBlock_find_last_free(table->lastFileBlock));
	
		descriptorBlock_attach(table->lastFileBlock, newBlock);
		
		/* update the blocks on the disk */
		descriptorBlock_store(table->lastFileBlock);
		descriptorBlock_store(newBlock);
		
		free(table->lastFileBlock);

		table->lastFileBlock = newBlock;
		
		int index = descriptorBlock_add_file(newBlock, name, blockCount);
		
		return newBlock->descriptors[index]->start;
	}
	else
	{
		return table->lastFileBlock->descriptors[added]->start;
	}
	
	
}

void filetable_list_files(table_t * table)
{
	descriptorBlock_t * current = table->lastFileBlock;
	
	while(current->nextBlock != 0)
	{
		descriptorBlock_list_files(current);
		if(current->nextBlock != 0)
			current = descriptorBlock_load(current->nextBlock);
	}
}

void filetable_display_details(table_t * table)
{
	descriptorBlock_t * current = table->lastFileBlock;
	
	while(current->nextBlock != 0)
	{
		descriptorBlock_display_details(current);
		if(current->nextBlock != 0)
			current = descriptorBlock_load(current->nextBlock);
	}
}

void filetable_find_file(table_t * table, char * name)
{
	
}








