#ifndef __DESCRIPTORBLOCK_H
#define __DESCRIPTORBLOCK_H

#define FILES_PER_BLOCK 14


/*
	File Descriptor Block on Disk
	previous block address | descriptor data | next block addresss 
	
	FILES_PER_BLOCK * FILES_SIZE = 504, leaving 8 bytes for the next and previous block addresses
	
	FILES_PER_BLOCK = 14 
	FILES_DESCRIPTOR_SIZE = 36
*/

#include "filedescriptor.h"

typedef struct DescriptorBlock {
	int address; /* not saved */
	file_t * descriptors[FILES_PER_BLOCK];
	unsigned int previousBlock;
	unsigned int nextBlock;
} descriptorBlock_t;

descriptorBlock_t * descriptorBlock_create();
void descriptorBlock_store(descriptorBlock_t * block);
descriptorBlock_t * descriptorBlock_load(int address);
descriptorBlock_t * descriptorBlock_load_last();
int descriptorBlock_find_file(descriptorBlock_t * block, char * filename); /* returns -1 if the file is not found */
void descriptorBlock_list_files(descriptorBlock_t * block);

#endif 
