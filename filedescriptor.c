#include <stdlib.h>


#include "filedescriptor.h"


file_t * file_create() 
{
    file_t * newFile = (file_t *) malloc(sizeof(file_t));
    
    strcpy(newFile->name, "");
    newFile->start = 0;
    newFile->blockCount = 0;
    
    return newFile;
}
