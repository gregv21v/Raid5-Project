#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <string.h>

#include "descriptorblock.h"
#include "volume.h"


descriptorBlock_t * descriptorBlock_create(int address)
{
	descriptorBlock_t * block = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	
	block->previousBlock = 0;
	block->nextBlock = 0;
	block->address = 0;
	
	int i = 0;
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		block->descriptors[i] = file_create();
	}

	return block;
}

void descriptorBlock_destroy(descriptorBlock_t * block)
{
	/* destroy file descriptors */
	int i = 0;
	for(; i < FILES_PER_BLOCK; i++)
	{
		file_destroy(block->descriptors[i]);
	}
	
	free(block);
	
}

void descriptorBlock_store(descriptorBlock_t * block)
{
	int i = 0; /* general purpose iterator */
	int offset = 0; /* the offset from the beginning of the buffer that you are looking at. */
	char * buffer = (char *) malloc(512); /* a buffer to temporarily hold the descriptorBlock data. */

	/* read the first 4 bytes as the address of the previous block */
	memcpy(buffer, &(block->previousBlock), 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		strncpy(buffer + offset, block->descriptors[i]->name, NAME_LENGTH);
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
	int i = 0;
	int offset = 0;
	char * buffer = volume_load_block(address);
	descriptorBlock_t * block = descriptorBlock_create(address);
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(&(block->previousBlock), buffer, 4);
	offset += 4;

	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
	
		/* Copy the name from the block */
		strncpy(block->descriptors[i]->name, buffer + offset, NAME_LENGTH);
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
    char last = 0;
    while(!last)
    {
        /* Go to next block */
        if(current->nextBlock != 0) 
        {
            current = descriptorBlock_load(current->nextBlock);
        }
        else 
        {
            last = 1;
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

void descriptorBlock_display_details(descriptorBlock_t * block)
{
	int i = 0;
	printf("Prev: %d\n", block->previousBlock);

	for(i = 0; i < FILES_PER_BLOCK; i++)
	{
		printf("Name: %s\n", block->descriptors[i]->name);
		printf("Start: %d\n", block->descriptors[i]->start);
		printf("BlockCount: %d\n", block->descriptors[i]->blockCount);
	}
	
	printf("Next: %d\n", block->nextBlock);
}


int descriptorBlock_find_last_free(descriptorBlock_t * block)
{
	int index = descriptorBlock_find_file(block, "");
	if(index != -1)
	{
		return block->descriptors[index]->start + block->descriptors[index]->blockCount;
	}
	else
	{
		return block->nextBlock + 1;
	}
}

int descriptorBlock_add_file(descriptorBlock_t * block, char * filename, int blockCount)
{
	int index = descriptorBlock_find_file(block, "");
	if(index != -1)
	{
		strcpy(block->descriptors[index]->name, filename);
		block->descriptors[index]->start = descriptorBlock_find_last_free(block);
		block->descriptors[index]->blockCount = blockCount;
		
		descriptorBlock_store(block);
		
		return index;
	} 
	else 
	{
		return -1; /* you need to create a new block */
	}
}


void descriptorBlock_attach(descriptorBlock_t * block, descriptorBlock_t * blockToAttach)
{
	block->nextBlock = blockToAttach->address;
	blockToAttach->previousBlock = block->address;
}

