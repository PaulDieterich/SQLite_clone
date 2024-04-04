
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define TABLE_MAX_PAGES 100

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;


typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

void serialize_row(Row* source, void * destination);
void deserialize_row(void * source, Row * destination);
typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;

typedef struct {
    uint32_t num_rows;
    void* pages[TABLE_MAX_PAGES];
} Table;

void* row_slot(Table* table, uint32_t row_num);

Table* new_table();

void free_table(Table* table);

void print_promt();

void print_row(Row* row);

ExecuteResult execute_select(Statement* statement, Table* table);

ExecuteResult  execute_insert(Statement* statement, Table* table);