 #include <stdlib.h> 
 #include <string.h> 
 
 

 
 #include "filedescriptor.h" 
 
 
 
 
 file_t * file_create()  
 { 
     file_t * newFile = (file_t *) malloc(sizeof(file_t)); 
      
     strcpy(newFile->name, ""); 
     newFile->start = 0; 
     newFile->blockCount = 0; 
      
     return newFile; 
 } 
 
 
 void file_destroy(file_t * file) 
 { 
     free(file); 
 } 
 
 
 void file_display(file_t * file) 
 { 
     printf("%s |  %d  |  %d  \n", file->name, file->start, file->blockCount); 
 } 
