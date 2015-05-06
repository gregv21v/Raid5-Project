#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <string.h>

#include "descriptorblock.h"
#include "volume.h"


descriptorBlock_t * descriptorBlock_create()
{
	descriptorBlock_t * block = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	
	block->previousBlock = 0;
	block->nextBlock = 0;
	
	int i = 0;
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		block->descriptors[i] = file_create();
	}
	
	
	
	return block;
}

void descriptorBlock_save(descriptorBlock_t * block)
{
	char * buffer = (char *) malloc(512); /* a buffer to temporarily hold the descriptorBlock data. */
	
	int offset = 0; /* the offset from the beginning of the buffer that you are looking at. */
	int i = 0; /* general purpose iterator */
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(buffer, &(block->previousBlock), 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		memcpy(buffer + offset, block->descriptors[i]->name, NAME_LENGTH);
		offset += NAME_LENGTH;
		
		/* Copy the start block address from the block */
		memcpy(buffer + offset, &(block->descriptors[i]->start), 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(buffer + offset,  &(block->descriptors[i]->blockCount), 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(buffer + offset, &(block->nextBlock), 4);
	
	volume_store_block(block->address, buffer);   
}

descriptorBlock_t * descriptorBlock_load(int address)
{
	char * buffer = volume_load_block(address);
	printf("%s", buffer);
	
	
	descriptorBlock_t * block = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	int offset = 0;
	int i = 0;
	
	block->address = address;
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(&(block->previousBlock), buffer, 4);
	offset += 4;
	
	printf("%d", block->previousBlock);
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
	
		/* Copy the name from the block */
		memcpy(block->descriptors[i]->name, buffer + offset, NAME_LENGTH);
		offset += NAME_LENGTH;
		
		/* Copy the start block address from the block */
		memcpy(&(block->descriptors[i]->start), buffer + offset, 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(&(block->descriptors[i]->blockCount), buffer + offset, 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(&(block->nextBlock), buffer + offset, 4);
	
	return block;  
}

descriptorBlock_t * descriptorBlock_load_last()
{
    descriptorBlock_t * current = descriptorBlock_load(0);
    while(current != NULL)
    {
        /* Go to next block */
        if(current->nextBlock != 0) 
        {
            current = descriptorBlock_load(current->nextBlock);
        }
        else 
        {
            current = NULL;
        }
    }
    return current;
}

int descriptorBlock_find_file(descriptorBlock_t * block, char * filename)
{
  int index = 0;
	for(; index < FILES_PER_BLOCK && strcmp(block->descriptors[index]->name, filename) != 0; index++);

  if(index < FILES_PER_BLOCK)
	  return index;
	else 
	  return -1;
}


void descriptorBlock_list_files(descriptorBlock_t * block)
{
	int i = 0;
	for(i = 0; i < FILES_PER_BLOCK; i++)
	{
		printf("%s\n", block->descriptors[i]->name);
	}
}
