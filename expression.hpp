#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <algorithm>

#include "db_info.hpp"
#include "opcodes.hpp"


namespace SEQL {
    enum CommandIdParserError {
        INVALID_COMMAND_TYPE = 1,
    };


    enum FragmentType {
        LITERAL,
        OPERATOR,
        VARIABLE,
        DATABASE,
    };

    class Fragment {
        int type;
        std::string value;

        Fragment() = default;
        Fragment(std::string val);
    };

    class Expression {
        bool valid;
        std::vector<Fragment> expression_fragments;
    };

    int determine_exp_type(ExpressionFragment * curr_expr, Stack * stack);
    unsigned int get_command_id(char * command);
Command * create_command(char * command_raw, size_t n);
}




#endif