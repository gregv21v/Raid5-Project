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

/* Each file descriptor is 28 bytes large. */
/* That leaves 8 bytes to store the address to the next block */
typedef struct node {
	char name[29];	/* 29 bytes */
	unsigned int start; /* 4 bytes: The block that the file starts on. */ 
	unsigned int blockCount; /* 4 bytes: The number of blocks that the file takes up. */
	unsigned char diskNumber; /* 1 byte: The disk the block stays on */
	struct node * next; /* not saved */
} file_t;


typedef struct list {
	int size;
	struct node * head; /* link to the last element */
	struct node * tail; /* link to the first element */
} list_t;


list_t * filetable_create();
void filetable_add_file(list_t * table, file_t * file);
void filetable_display(list_t * table);
void filetable_list_files(list_t * table);
file_t * filetable_find(list_t * table, char * filename);





#endif	/* __FILETABLE_H */
