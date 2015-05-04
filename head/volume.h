
#ifndef __VOLUME_H
#define __VOLUME_H

#include "disk.h"

/*Define the disk names*/
#define DISK_0 "disk_0"
#define DISK_1 "disk_1"
#define DISK_2 "disk_2"


char * volume_load(int address);
void volume_store(int address, char * block);





#endif  
