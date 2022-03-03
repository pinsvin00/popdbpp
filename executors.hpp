#ifndef EXECUTORS_H
#define EXECUTORS_H

#include "expression.hpp"
#include "utils.hpp"

enum CommandStatus {
    COMMAND_SUCCESS,
    COMMAND_FAILURE,
};


unsigned int pop_record(Command * command);
unsigned int delete_record(Command * command);
unsigned int sel_record(Command * command);
unsigned int create_new_record(Command * command);
unsigned int unhandled(Command * command);


#endif