/*
 * filetable.h
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure.
 */
#ifndef __FILETABLE_H
#define __FILETABLE_H

#include <unistd.h>
#include <string.h>

#include "descriptorblock.h"

typedef struct Table {
	fileDescriptor_t * currentFile;
	descriptorBlock_t * currentFileBlock;
	descriptorBlock_t * lastFileBlock;
} table_t;


table_t * filetable_create(); /* initializes last file block */
int filetable_find_last_free(table_t * table);
int filetable_add_file(table_t * table, char * name, int blockCount); /* adds to the end of the last file block */



#endif	/* __FILETABLE_H */
