#ifndef __DESCRIPTORBLOCK_H
#define __DESCRIPTORBLOCK_H

#define FILES_PER_BLOCK 18

/*
	File Descriptor Block on Disk
	previous block address | descriptor data | next block addresss 
*/

#include "filedescriptor.h"

typedef struct DescriptorBlock {
	int address; /* not saved */
	fileDescriptor_t * descriptors[FILES_PER_BLOCK];
	unsigned int previousBlock;
	unsigned int nextBlock;
} descriptorBlock_t;

void descriptorBlock_save(descriptorBlock_t * block);
descriptorBlock_t * descriptorBlock_load(int address);
descriptorBlock_t * descriptorBlock_load_last();
int descriptorBlock_find_file(descriptorBlock_t * block, char * filename); /* returns -1 if the file is not found */
void descriptorBlock_list_files(descriptorBlock_t * block);

#endif 
