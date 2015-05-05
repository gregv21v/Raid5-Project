/*
 * filedescriptor.h
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: This file provides a file table data structure.
 */
#ifndef __FILEDESCRIPTOR_H
#define __FILEDESCRIPTOR_H

#define NAME_LENGTH 28

/* Each file descriptor is 38 bytes large. */
/* That leaves 8 bytes to store the address to the next block */
typedef struct FileDescriptor {
	char name[NAME_LENGTH];	/* 29 bytes */
	unsigned int start; /* 4 bytes: The block that the file starts on in the volume. */ 
	unsigned int blockCount; /* 4 bytes: The number of blocks that the file takes up. */
} file_t;


file_t * file_create();



#endif	
