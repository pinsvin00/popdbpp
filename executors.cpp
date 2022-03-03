#include "executors.hpp"


Field* get_field(char * var_name, Node * field) {
    while(field != NULL) {
        Field * field_data = (Field*) field->data;
        if(strcmp(var_name, field_data->field_name) == 0) {
            return field_data;
        }
        field = field->next;
    }
    return NULL;
}

enum BitCommandFlag{
    DELETE,
    COPY_RECORD,
};


void * evalute_expression(ExpressionFragment * exp, int expression_type) {
    
}


unsigned int pop_nth(Command * command, unsigned int nth, uint16_t flags) {
    DBInfo * db = command->db;

    db->busy = true;
    
    char * temp_ext = "temp";
    char * temp_file = malloc(strlen(db->path) + strlen(temp_ext));

    size_t record_size_newline = db->size_of_record + 1;
    
    strcpy(temp_file, db->path);
    strcat(temp_file, temp_ext);
    
    FILE * fr = fopen(db->path, "r");
    FILE * frw = fopen(temp_file, "rb+");

    //copying definition
    char * definition_buffer = malloc(db->definition_end_byte);
    fread(definition_buffer, db->definition_end_byte, 1, fr);
    fwrite(definition_buffer, db->definition_end_byte,1, frw);

    int nth_row = 0;
    char * record_buffer = malloc(db->size_of_record + 1);
    while(fread(record_buffer, record_size_newline, 1, fr) != 0) { 
        if (nth_row == nth){
            if(IS_BIT_SET(flags, COPY_RECORD)) {
                command->data = record_buffer;
            }
        }
        else {
            if(!IS_BIT_SET(flags, DELETE)) {
                fwrite(record_buffer, record_size_newline, 1, frw);
            }
        }
    }

    free(record_buffer);

    fclose(fr);
    fclose(frw);

    db->busy = false;
}

unsigned int pop_record(Command * command) {
    char * nth_str = stack_at(command->expression_stack, 2);
    unsigned int nth = 0;
    if(nth_str != NULL) {
        nth = atoi(nth_str);
    }
    uint16_t flags = 0;

    SET_BIT(flags, DELETE);
    SET_BIT(flags, COPY_RECORD);

    return pop_nth(command, nth, flags);
}


unsigned int sel_record(Command * command) {
    char * nth_str = stack_at(command->expression_stack, 2);
    if(nth_str == NULL) {
        perror("Invalid command usage, nth_row, wasn't specified for SEL");
        return COMMAND_FAILURE;
    }
    int nth = atoi(nth_str);
    uint16_t flags = 0;

    SET_BIT(flags, COPY_RECORD);

    return pop_nth(command, nth, flags);
}

unsigned int sel_record(Command * command) {
    char * nth_str = stack_at(command->expression_stack, 2);
    if(nth_str == NULL) {
        perror("Invalid command usage, nth_row, wasn't specified for DEL");
        return COMMAND_FAILURE;
    }
    int nth = atoi(nth_str);
    uint16_t flags = 0;

    SET_BIT(flags, COPY_RECORD);

    return pop_nth(command, nth, flags);
}

unsigned int create_new_record(Command * command) {
    DBInfo * dbi = command->db;
    Node * field_arr = dbi->field_arr;
    Stack * exp_stack = (Stack*) command->expression_stack;

    FILE * fp = fopen(dbi->path, "rb+");
    uint64_t curr_id = db_id(command, fp);
    char * record = malloc(dbi->size_of_record); 

    for (size_t i = 0; i < exp_stack->top; i++){
        ExpressionFragment * expression = (ExpressionFragment*) stack_at(exp_stack, i);

        if(expression->type == VARIABLE) {
            char * var_name = expression->expression;

            Field* field = get_field(var_name, field_arr);

            if (field == NULL) {
                return COMMAND_FAILURE;
            };
            ExpressionFragment * assumed_operator  = (ExpressionFragment*) stack_at(exp_stack, i+1);
            ExpressionFragment * assumed_literal  = (ExpressionFragment*) stack_at(exp_stack, i+2);

            if(assumed_literal->type == LITERAL && assumed_operator->type == OPERATOR){
                char * sized_literal = malloc(field->size);
                int s = strlen(assumed_literal->expression);

                memcpy(sized_literal, assumed_literal->expression, s);
                memcpy((record + field->offset), sized_literal, field->size );
            }
            else {
                printf("Invalid usage of variable\n");
                return COMMAND_FAILURE;
            }

        }
    }
    

    fseek(fp, 0, SEEK_END);
    fwrite(record, 1, dbi->size_of_record, fp);
    free(record);
    fclose(fp);
}

unsigned int unhandled(Command * command) {
    printf("Command not handled yet\n");
    return COMMAND_SUCCESS;
}