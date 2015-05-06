 
 
 #ifndef __VOLUME_H 
 #define __VOLUME_H 
 
 
 #include "disk.h" 
 
 
 /*Define the disk names*/ 
 #define DISK_0 "disk_0" 
 #define DISK_1 "disk_1" 
 #define DISK_2 "disk_2" 
 
 
 void volume_display_block_raw(unsigned int address); 
 char * volume_load_block(unsigned int address); 
 void volume_store_block(unsigned int address, char * block); 
 
 

 #endif   
