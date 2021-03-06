 /*
 * descriptorblock.h
 * Author: Gregory Venezia
 * Date: 4/27/2015
 * Course: CSC3320
 * Description: The descriptor block is a subunit of the filetable. It's the data that would be 
 	stored in one block of memory in the volume.
 */
 
 /* 
 	File Descriptor Block on Disk 
 	previous block address | descriptor data | next block addresss  
 	 
 	FILES_PER_BLOCK * FILES_SIZE = 504, leaving 8 bytes for the next and previous block addresses 
 	 
 	FILES_PER_BLOCK = 14  
 	FILES_DESCRIPTOR_SIZE = 36 
 */ 

 
 #ifndef __DESCRIPTORBLOCK_H 
 #define __DESCRIPTORBLOCK_H 
 
 #include "filedescriptor.h" 
 
 #define FILES_PER_BLOCK 14 

 typedef struct DescriptorBlock { 
 	unsigned int address; /* not saved */ 
 	file_t * descriptors[FILES_PER_BLOCK]; /* these are the files */
 	unsigned int previousBlock; /* address of next block of the file table */
 	unsigned int nextBlock; /* address of the block that came before this in the file table */
 } descriptorBlock_t; 

 descriptorBlock_t * descriptorBlock_create(unsigned int address); /* tested */ 
 void descriptorBlock_destroy();  /* tested */
 void descriptorBlock_store(descriptorBlock_t * block); /* tested */ 
 descriptorBlock_t * descriptorBlock_load(unsigned int address); /* tested */ 
 descriptorBlock_t * descriptorBlock_load_last(); /* tested */ 
 									 
 int descriptorBlock_find_file(descriptorBlock_t * block, char * name); /* returns -1 if the file is not found */ 
 									                                                                  /* tested */ 
 void descriptorBlock_list_files(descriptorBlock_t * block); /* tested */ 
 void descriptorBlock_display_details(descriptorBlock_t * block); /* tested */

 void descriptorBlock_attach(descriptorBlock_t * block, descriptorBlock_t * blockToAttach); /* tested */ 
 
 int descriptorBlock_add_file(descriptorBlock_t * block, char * filename, unsigned int blockCount); /* tested */
 int descriptorBlock_find_last_free(descriptorBlock_t * block); /* tested */
 
 
  #endif  
