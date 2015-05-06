/*
 * filetable.h
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a VTOC data structure.
 */
#ifndef __FILETABLE_H
#define __FILETABLE_H

#include <unistd.h>
#include <string.h>

#include "descriptorblock.h"

typedef struct Table {
	descriptorBlock_t * firstFileBlock;
	descriptorBlock_t * lastFileBlock;
} table_t;

table_t * filetable_create(); /* initializes last file block */
				/* tested */
void filetable_destroy(); /* not tested */
int filetable_add_file(table_t * table, char * name, int blockCount); /* adds to the end of the last file block */
									/* tested */
file_t * filetable_find_file(table_t * table, char * name); /* tested */
void filetable_remove_file(table_t * table, char * name); /* not tested */
void filetable_list_files(table_t * table); /* tested */
void filetable_display_details(table_t * table); /* tested */

#endif	/* __FILETABLE_H */
