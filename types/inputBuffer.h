
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;

}InputBuffer;


InputBuffer* newInputBuffer();