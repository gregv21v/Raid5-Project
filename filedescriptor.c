 #include <stdlib.h> 
 #include <string.h> 
 
 

 
 #include "filedescriptor.h" 
 
 
 
  /* 
        Creates a file
        Pre: None
        Post: Allocates memory for a file descriptor
  */ 
 file_t * file_create()  
 { 
     file_t * newFile = (file_t *) malloc(sizeof(file_t)); 
      
     strcpy(newFile->name, ""); 
     newFile->start = 0; 
     newFile->blockCount = 0; 
      
     return newFile; 
 } 
 
 /* 
        Destroys a file
        Pre: None
        Post: Frees memory for the file descriptor
  */ 
 void file_destroy(file_t * file) 
 { 
     free(file); 
 } 
 
 /* 
        Displays all the data of a file descriptor
        Pre: None
        Post: Displays the file descriptor data
  */ 
 void file_display(file_t * file) 
 { 
     printf("%s |  %d  |  %d  \n", file->name, file->start, file->blockCount); 
 } 
