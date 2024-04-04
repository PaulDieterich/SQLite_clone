#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "types/table.h"
#include "types/inputBuffer.h"


PrepareResult prepare_insert(InputBuffer *buffer, Statement *statement);

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
        return prepare_insert(inputBuffer, statement);
    }
    if(strcmp(inputBuffer->buffer,"select") == 0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

PrepareResult prepare_insert(InputBuffer *inputBuffer, Statement *statement) {
    statement->type = STATEMENT_INSERT;

    char* keyword = strtok(inputBuffer->buffer, " ");
    char* id_string = strtok(NULL, " ");
    char* username = strtok( NULL, " ");
    char* email = strtok(NULL, " ");

    if(id_string == NULL || username == NULL || email == NULL){
        return PREPARE_SYNTAX_ERROR;
    }
    int id = atoi(id_string);
    if(strlen(username) > COLUMN_USERNAME_SIZE){
        return PREPARE_STRING_TOO_LONG;
    }
if(strlen(email) > COLUMN_EMAIL_SIZE){
    return PREPARE_STRING_TOO_LONG;
}
statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username,username);
    strcpy(statement->row_to_insert.email, email);

return PREPARE_SUCCESS;
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
            case(PREPARE_STRING_TOO_LONG):
                printf("String is too long. \n");
                continue;
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
