/*
 * filetable.c
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure.
 */


#include "filetable.h"


/*
	Creates the table
	Pre: None
	Post: Adds a table to the volume with a reference in memory.
*/
table_t * filetable_create()
{
	table_t * table = (table_t *) malloc(sizeof(table_t));
	
	/* find the last descriptor block */
	table->firstFileBlock = descriptorBlock_create(0);
	/* TODO: Add condition for empty file table */
	table->lastFileBlock = table->firstFileBlock;
	
	return table;
}



/*
	Frees up the memory used for the table
	Pre: None
	Post: Adds a table to the volume with a reference in memory.
*/
void filetable_destroy(table_t * table)
{
	if(table->firstFileBlock != NULL)
		free(table->firstFileBlock);
	if(table->lastFileBlock != NULL)
		free(table->lastFileBlock);
	if(table != NULL)	
		free(table);
}

table_t * filetable_load()
{
	table_t * table = (table_t *) malloc(sizeof(table_t));
	
	/* find the last descriptor block */
	table->firstFileBlock = descriptorBlock_load(0);
	/* TODO: Add condition for empty file table */
	table->lastFileBlock = descriptorBlock_load_last();
	
	return table;
}


/*
	Adds a file to the table
	Pre: table must be created, name length must be less than or equal to NAME_LENGTH.
	Post: Adds a file to the table.
*/
int filetable_add_file(table_t * table, char * name, unsigned int blockCount)
{
	int index = descriptorBlock_add_file(table->lastFileBlock, name, blockCount);
	
	if(index == -1) 
	{
		/* a new block needs to be created to add this file */
		descriptorBlock_t * newBlock = descriptorBlock_create(descriptorBlock_find_last_free(table->lastFileBlock));
	
		descriptorBlock_attach(table->lastFileBlock, newBlock);

		index = descriptorBlock_add_file(newBlock, name, blockCount);
		
		/* update the blocks on the disk */
		descriptorBlock_store(table->lastFileBlock);
		descriptorBlock_store(newBlock);
		
		table->lastFileBlock = newBlock;
		
		free(table->lastFileBlock);

		return newBlock->descriptors[index]->start;
	}
	else
	{
		return table->lastFileBlock->descriptors[index]->start;
	}
	
	
}

/*
	Lists the files in the table
	Pre: table has been created
	Post: Lists the files int the table.
*/
void filetable_list_files(table_t * table)
{
	descriptorBlock_t * current = table->lastFileBlock;

	do {
		descriptorBlock_list_files(current);
		if(current->nextBlock != 0)
			current = descriptorBlock_load(current->nextBlock);
	} while(current->nextBlock != 0);
}

/*
	Displays all information about the files in the table
	Pre: table has been created
	Post: Lists all information about the files in the table.
*/
void filetable_display_details(table_t * table)
{
	descriptorBlock_t * current = table->lastFileBlock;

	do {
		descriptorBlock_display_details(current);
		if(current->nextBlock != 0)
			current = descriptorBlock_load(current->nextBlock);
	} while(current->nextBlock != 0);
}


/*
	Removes a file from the table
	Pre: table has been created, name is less than or equal to NAME_LENGTH
	Post: The file is removed from the table.
*/
void filetable_remove_file(table_t * table, char * name)
{
	descriptorBlock_t * current = table->lastFileBlock;
	int index = -1; /* the index of the file once its found */

	do {
		current = descriptorBlock_load(current->nextBlock);
		
		index = descriptorBlock_find_file(current, name);
		
	} while(current->nextBlock != 0 && index == -1);
	
	strcpy(current->descriptors[index]->name, "*removed*");
	
	return index;
}

/*
	Finds a file by name in the table
	Pre: table has been created, name is less than or equal to NAME_LENGTH
	Post: The file found is returned.
*/
file_t * filetable_find_file(table_t * table, char * name)
{
	descriptorBlock_t * current = table->lastFileBlock;
	int index = -1; /* the index of the file once its found */

	do {
		current = descriptorBlock_load(current->nextBlock);
		
		index = descriptorBlock_find_file(current, name);
	} while(current->nextBlock != 0 && index == -1);
	
	return current->descriptors[index];
	
}








