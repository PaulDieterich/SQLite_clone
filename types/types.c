#include <stdio.h>
#include <stdlib.h>



typedef enum{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;


typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
} Statement;


// INPUT BUFFER


