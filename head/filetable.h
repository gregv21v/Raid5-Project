/*
 * filetable.h
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure for storing information about the files.
 */
#ifndef __FILETABLE_H
#define __FILETABLE_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "descriptorblock.h"

typedef struct Table {
	descriptorBlock_t * firstFileBlock;
	descriptorBlock_t * lastFileBlock;
} table_t;

table_t * filetable_create(); /* initializes last file block */
int filetable_add_file(table_t * table, char * name, unsigned int blockCount); /* adds to the end of the last file block */
file_t * filetable_find_file(table_t * table, char * name);
void filetable_remove_file(table_t * table, char * name);
void filetable_list_files(table_t * table);
void filetable_display_details(table_t * table);

#endif	/* __FILETABLE_H */
