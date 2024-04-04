#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "types/table.h"
#include "types/inputBuffer.h"


/**
 *  read input line and write the input_length from input_buffer
 *  and set buffer from inputBuffer
 *
 * @param input_buffer
 */
void read_input(InputBuffer* input_buffer){
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if(bytes_read <= 0){
        printf("Error reading input \n");
        exit(EXIT_FAILURE);
    }
    //Ignore trailing newline
    input_buffer->input_length =  bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

/**
 *  free the input_buffer
 * @param input_buffer
 */
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}

/**
 *
 * all commands starts with . is a meta command
 * @example exit programm by .exit
 *
 * @param inputBuffer
 * @return MetaCommandResult
 *          - Enum MapCommandRsult
 */
MetaCommandResult do_meta_command(InputBuffer* inputBuffer){
    if(strcmp(inputBuffer->buffer, ".exit") == 0){
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

/**
 *  check if sql command is available,
 *  if avalable return PREPARE_SUCCESS else PREPARE_UNRECOGNIZED_STATEMENT
 *
 * @param inputBuffer
 * @param statement
 * @return PrepareResult
 *          - PrepareResult enum
 */
PrepareResult prepare_statement(InputBuffer* inputBuffer, Statement* statement){

    if(strncmp(inputBuffer->buffer,"insert",6) == 0){
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
                inputBuffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id), statement->row_to_insert.username, statement->row_to_insert.email);

        if(args_assigned < 3){
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }
    if(strcmp(inputBuffer->buffer,"select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

/**
 * Execute statement
 * @param statement
 */
 ExecuteResult execute_statement(Statement* statement, Table* table){
    switch (statement->type) {
        case (STATEMENT_SELECT):
            return execute_select(statement, table);
        case (STATEMENT_INSERT):
            return execute_insert(statement, table);
    }
}

/**
 *  Main Database sql console
 *  this programm floowes the tutorial of
 *  https://cstack.github.io/db_tutorial
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]){
    Table* table = new_table();
    InputBuffer* input_buffer = newInputBuffer();

    while(true) {
        print_promt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error. Could not prepare statement: %u. \n", statement.type);
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'. \n", input_buffer->buffer);
                continue;
        }

        switch (execute_statement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                printf("Executed.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                printf("Error: Table full.\n");
                break;
        }

    }

}
