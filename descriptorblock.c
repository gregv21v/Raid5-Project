
#include "descriptorblock.h"






void descriptorBlock_save(descriptorBlock_t * block)
{
    char * buffer = (char *) malloc(512)
	
	int offset = 0;
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(buffer, block->previousBlock, 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		memcpy(buffer + offset, block->descriptors[i]->name, 29);
		offset += 29;
		
		/* Copy the start block address from the block */
		memcpy(buffer + offset, block->descriptors[i]->start, 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(buffer + offset,  block->descriptors[i]->blockCount, 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(buffer + offset, block->nextBlock, 4);
	
	volume_store(block->address, buffer);   
}
descriptorBlock_t * descriptorBlock_load(int address)
{
    char * buffer = volume_load(address);
	
	descriptorBlock_t * block = (descriptorBlock_t *) malloc(sizeof(descriptorBlock_t));
	
	block->address = address;
	
	int offset = 0;
	
	/* read the first 4 bytes as the address of the previous block */
	memcpy(block->previousBlock, buffer, 4);
	offset += 4;
	
	/* Load the current block into the file descriptor array */
	for(i = 0; i < FILES_PER_BLOCK; i++) 
	{
		/* Copy the name from the block */
		memcpy(descriptors[i]->name, buffer + offset, 29);
		offset += 29;
		
		/* Copy the start block address from the block */
		memcpy(descriptors[i]->start, buffer + offset, 4);
		offset += 4;
		
		/* Copy the block cout from the block */
		memcpy(descriptors[i]->blockCount, buffer + offset, 4);
		offset += 4;
	}
	
	/* read the last 4 bytes as the address of the next block */
	memcpy(block->nextBlock, buffer + offset, 4);
	
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
}



int descriptorBlock_find_file(descriptorBlock_t * block, char * filename)
{
  file_t * current = table->head;
  
  int index = 0;
	for(; index < FILES_PER_BLOCK && strcmp(current->name, filename) != 0; index++);

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
