#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "volume.h"




/* displays the raw data at the specified address */
void volume_display_block_raw(int address)
{
    	char * buffer = volume_load_block(address);
    
    	int i = 0;
	for(; i < 512; i++) 
	{
	    if(i % 16 == 0 && i != 0)
		    printf("%c", buffer[i]);
	}
}


char * volume_load_block(int address)
{
	int error = 0;	
	char * temp = (char *) malloc(512);
	/* Open Disk */
	if(address % 2 == 0) 
	{
	    error = open_disk(DISK_0);
	}
	 else 
	{
	    error = open_disk(DISK_1);
	}
	  
	block_read((int) (address / 2), temp);
	
	printf("In volume load block: %s", temp);
	  
	close_disk();
	return temp;
  
}

void volume_store_block(int address, char * block)
{
	char * disk0Block = (char *) malloc(512);
	char * disk1Block = (char *) malloc(512);
	char * parityBlock = (char *) malloc(512);
	int error = 0;
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
		error = open_disk(DISK_0);
	}
	else 
	{
		error = open_disk(DISK_1);
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
	
	/*free(disk0Block);
	free(disk1Block);
	free(parityBlock);*/

  
  

}
