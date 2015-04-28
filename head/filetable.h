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



typedef struct node {
	char name[33];	/* 33 bytes */
	unsigned int start; /* 4 bytes */ 
	unsigned int blockNumber; /* 4 bytes */
	unsigned char diskNumber; /* 1 byte */
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





#endif	/* __FILETABLE_H */
