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

#define FILES_PER_BLOCK 18
#define NAME_MAX_LENGTH 30


/* Each file descriptor is 28 bytes large. */
/* That leaves 8 bytes to store the address to the next block */
typedef struct FileDescriptor {
	char name[NAME_MAX_LENTH];	/* 29 bytes */
	unsigned int start; /* 4 bytes: The block that the file starts on in the volume. */ 
	unsigned int blockCount; /* 4 bytes: The number of blocks that the file takes up. */
} fileDescriptor_t;


typedef struct DescriptorBlock {
	fileDescriptor_t * descriptors[FILES_PER_BLOCK];
	unsigned int previousBlock;
	unsigned int nextBlock;
} descriptorBlock_t;







void filetable_add_file(list_t * table, file_t * file);
void filetable_display(list_t * table);
void filetable_list_files(list_t * table);
file_t * filetable_find(list_t * table, char * filename);





#endif	/* __FILETABLE_H */
