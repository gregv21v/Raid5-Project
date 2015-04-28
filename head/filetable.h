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

typedef void* FileTable;
typedef void* File;

FileTable * filetable_create();
void filetable_add_file(FileTable * table, File * file);
void filetable_display(FileTable * table);
void filetable_list_files(FileTable * table);





#endif	/* __FILETABLE_H */
