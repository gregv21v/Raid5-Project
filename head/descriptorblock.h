#ifndef __DESCRIPTORBLOCK_H
#define __DESCRIPTORBLOCK_H

typedef struct DescriptorBlock {
	int address; /* not saved */
	fileDescriptor_t * descriptors[FILES_PER_BLOCK];
	unsigned int previousBlock;
	unsigned int nextBlock;
} descriptorBlock_t;

void descriptorBlock_save(descriptorBlock * block);
descriptorBlock_t * descriptorBlock_load(int address);
descriptorBlock_t * descriptorBlock_find_last();

#endif 
