
#include <stdio.h> /* printf */ 
#include <stdlib.h> /* malloc */ 


#include "volume.h" 

/* 
        Displays the raw data at the specified address.
        pre: An address within the volume.
        pos: Displays the bytes of data at that address.
*/ 
void volume_display_block_raw(int address) 
{ 
        char * buffer = volume_load_block(address); /* holds the block from the volume */
        
        int i; /* iterator */ 
        for(; i < 512; i++)  
        { 
                printf("%c", buffer[i]); 
                if(i % 32 == 0 && i != 0) 
                        printf("\n");
        } 
} 




/* 
        Loads the block from the volume at the given address
        pre: An address within the volume.
        pos: Returns the bytes of data at the address.
*/
char * volume_load_block(int address) 
{ 
        char * temp = (char *) malloc(512); 
        
        /* Open Disk */ 
        if(address % 2 == 0)  
            open_disk(DISK_0); 
        else  
            open_disk(DISK_1); 
    
        block_read((int) (address / 2), temp); 

        close_disk(); 
        
        return temp; 
} 

/* 
        Stores a block at the address
        pre: An address within the volume, and a 512 byte block of data.
        pos: The data has been stored in the volume at the specified address.
*/ 
void volume_store_block(int address, char * block) 
{ 
    char * disk0Block = (char *) malloc(512); /* buffer for first disk block */
    char * disk1Block = (char *) malloc(512); /* buffer for second disk block */
    char * parityBlock = (char *) malloc(512); /* buffer for parity disk block */
    int i = 0;	/* a general iterator variable */ 
    
  
    /* disk setup: 
     	Disk 0        Disk 1 
     	  0             1 
     	  2             3 
     	  4             5 
     	  6             7 
     	  ...          etc. 
    */ 
  
     /* Open Disk */ 
     if(address % 2 == 0)  
     { 
     	open_disk(DISK_0); 
     } 
     else  
     { 
     	open_disk(DISK_1); 
     } 
  
    /* write to the address space (disks 0, and 1) */ 
    block_write((int) (address / 2), block); 
  
  
    /* Close Disk */ 
    close_disk(); 
  
  
  
  
     open_disk(DISK_0); 
     	block_read((int) (address / 2), disk0Block); 
     close_disk(); 
      
     open_disk(DISK_1); 
     	block_read((int) (address / 2), disk1Block); 
     close_disk(); 
      
     /* Update the parity disk */ 
     open_disk(DISK_2); 
 	    block_read((int) (address / 2), parityBlock); 
  
     		/* XOR disk 0 and 2's blocks */ 
     		for(i = 0; i < 512; i++) 
     		{ 
     			parityBlock[i] = disk0Block[i] ^ disk1Block[i]; 
     		} 
      
     	block_write((int) (address / 2), parityBlock); 
     close_disk(); 
  
     /*
     free(disk0Block); 
     free(disk1Block); 
     free(parityBlock);*/
} 
