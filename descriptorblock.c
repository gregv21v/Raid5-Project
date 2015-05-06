/*
 * descriptorblock.c
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: The descriptor block is a subunit of the filetable. It's the data that would be 
 	stored in one block of memory in the volume.
 */


#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <string.h>

#include "descriptorblock.h"
#include "volume.h"

/*
	Creates a descriptor block at the given address in the volume.
	Pre: An address in the volume
	Post: A pointer to that descriptor block is returned.
*/
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

/*
	Frees up the memory for a descriptor block.
	Pre: The block to be destroyed
	Post: The memory of the descriptor block is destroyed.
*/
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

/*
	Frees up the memory for a descriptor block.
	Pre: The block to be destroyed
	Post: The memory of the descriptor block is destroyed.
*/
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

/*
	Loads a descriptor block from a specified address in the volume.
	Pre: An address in the volume.
	Post: Returns a descriptor block from the volume.
*/
descriptorBlock_t * descriptorBlock_load(int address)
{
	int i = 0; /* a general iterator */
	int offset = 0; /* offset in the buffer */
	char * buffer = volume_load_block(address); /* data from the volume */
	descriptorBlock_t * block = descriptorBlock_create(address); /* loaded block */
	
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


/*
	Loads the last descriptor block of the VTOC.
	Pre: None.
	Post: Returns a pointer to the last descriptor block in the volume.
*/
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


int descriptorBlock_find_file(descriptorBlock_t * block, char * name)
{
  	int index = 0;
	for(; index < FILES_PER_BLOCK && strcmp(block->descriptors[index]->name, name) != 0; index++);

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
		file_display(block->descriptors[i]);
	}
	
	printf("Next: %d\n", block->nextBlock);
}


int descriptorBlock_find_last_free(descriptorBlock_t * block)
{
	int index = descriptorBlock_find_file(block, "");
	if(index > 0 && index < FILES_PER_BLOCK)
	{
		return block->descriptors[index-1]->start + block->descriptors[index-1]->blockCount;
	}
	else if(index == 0)
	{
		return block->address + 1;
	}
	else 
	{
		return block->descriptors[FILES_PER_BLOCK-1]->start + block->descriptors[FILES_PER_BLOCK-1]->blockCount + 1;
	}
}

int descriptorBlock_add_file(descriptorBlock_t * block, char * filename, int blockCount)
{
	int index = descriptorBlock_find_file(block, "");
	if(index != -1)
	{
		block->descriptors[index]->start = descriptorBlock_find_last_free(block);
		strcpy(block->descriptors[index]->name, filename);
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

