#include "inputBuffer.h"


InputBuffer* newInputBuffer(){
    InputBuffer* inputBuffer = malloc(sizeof(inputBuffer));
    inputBuffer->buffer = NULL;
    inputBuffer->buffer_length = 0;
    inputBuffer->input_length = 0;
}