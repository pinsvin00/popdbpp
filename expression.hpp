#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <vector>

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


    class Variable { 
        int type;
        bool is_callable;
        bool has_native_code = false;
        std::string value;
        std::string name;
    };


    class PredefinedFunction : Variable {
        std::function<void(std::vector<std::string>)> executor;
        PredefinedFunction();

    };

    class Engine {
        std::map<std::string, Variable> variables;
    public:
        Engine();
        void evaluate_command(const std::string& command);
    };

    class Fragment {
        public:
        int type;
        std::string value;
        int prio;

        Fragment() = default;
        Fragment(std::string val);
    };

    class Expression {
        public:
        bool valid;
        std::vector<Fragment> expression_fragments;

        void to_rpn();
        Expression(std::vector<Fragment>& fragments);
        
        //evaluate constant expressions
        //then evaluate dynamic expressions, that are based on variables/function

    };

}




#endif