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
	So with this new volumn system are we just storing 
*/

#include "filetable.h"






/*
	Creates the list

*/
list_t * filetable_create() 
{
	list_t * new_list = NULL;
	new_list = (list_t *) calloc(1, sizeof(list_t));
	new_list->size = 0;
	new_list->head = NULL;
	new_list->tail = NULL;

	return new_list;
}


/*
	Adds a file to the list

*/
void filetable_add_file(list_t * table, file_t * file)
{

	if(table->head == NULL) /* this is the first element in the list */
	{
		table->head = (file_t *) file;
		table->tail = (file_t *) file;
	}
	else /* there are other elements in the list */
	{
		table->tail->next = (file_t *) file;
		table->tail = table->tail->next;
	}
	table->size++;
}


void filetable_display(list_t * table) 
{

	file_t * current = table->head;

	while(current != NULL)
	{
		/* display the details of the current file */
		printf("%s\n", current->name);
		printf("Start: %d\n", current->start);
		printf("Block Number: %d\n", current->blockNumber);
		printf("Disk Number: %d\n", current->diskNumber);

		current = current->next;
	}

}

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



