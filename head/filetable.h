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


typedef struct fileDescriptor {
	char name[29];	/* 29 bytes */
	unsigned int start; /* 4 bytes: The block that the file starts on. */ 
	unsigned int blockCount; /* 4 bytes: The number of blocks that the file takes up. */
	unsigned char diskNumber; /* 1 byte: The disk the block stays on */
} fileDescriptor_t;








#endif	/* __FILETABLE_H */
