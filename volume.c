#include <stdio.h> /* printf */

#include "disk.h"
#include "volume.h"




/* displays the raw data at the specified address */
void volume_display_block_raw(int address)
{
    	char * buffer = volume_load_block(address);
    
    	int i = 0;
	for(; i < size; i++) 
	{
	    if(i % 16 == 0 && i != 0)
		    printf("%c", buffer[i]);
	}
}


char * volume_load_block(int address)
{
    char * temp;
    /* Open Disk */
    if(address % 2 == 0) 
    {
        error = open_disk(DISK_0);
    }
     else 
    {
        error = open_disk(DISK_1);
    }
      
      read_block((int) (address / 2), temp);
      
    close_disk();
    return temp;
  
}

void volume_store_block(int address, char * block)
{
  int error = 0;
  
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
  write_block((int) (address / 2), block);
  
  
  /* Close Disk */
  if(address % 2 == 0) 
  {
    error = open_disk(DISK_0);
  }
  else 
  {
    error = open_disk(DISK_1);
  }
  
  char * disk0Block;
  char * disk1Block;
  char * parityBlock;
  
  /* update the parity disk */
  open_disk(DISK_0);
    read_block((int) (address / 2), disk0Block);
  close_disk();
  
  open_disk(DISK_1);
    read_block((int) (address / 2), disk1Block);
  close_disk();
  
  open_disk(DISK_2);
    read_block((int) (address / 2), parityBlock);
  close_disk();
  
  /* XOR disk 0 and 2's blocks */
  int i = 0;
  for(i = 0; i < 512; i++)
  {
      parityBlock[i] = disk0Block[i] ^ disk1Block[i];
  }
  
  
  write_block((int) (address / 2), parityBlock);

  
  

}
