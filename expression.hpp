#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>

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
        public:
        int type;
        std::string value;
        int prio;

        bool is_operator = false;
        bool is_variable = false;

        Fragment() = default;
        Fragment(std::string &val);
    };


    class Variable {
        public:

        int type;
        bool is_callable;
        bool has_native_code;
        std::string value;
        std::string name;
    };


    class PredefinedFunction : Variable {
        std::function<void(std::vector<std::string>)> executor;
        PredefinedFunction();

    };

    class Expression {
        public:
        bool valid;
        std::vector<Fragment> fragments;

        void convert_to_rpn();
        Variable evaluate();


        Expression(std::vector<Fragment> fragments);
        
        //evaluate constant expressions
        //then evaluate dynamic expressions, that are based on variables/function

    };

    class Operator : Fragment {
        public:
        int argument_count = 0;

        //TODO instead of passing whole engine, lets just pass some fragment of engine that contains needed data
        std::function<Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&)> executor;
        Operator() = default;
        Operator(int arg_c, std::function<Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&)> executor) {
            this->is_operator = true;
            this->executor = executor;
            this->argument_count = arg_c;
        }         
    };


    class Engine {
        std::map<std::string, Variable> variables;
        std::map<std::string, Operator> operators;
    public:
        Engine();
        void evaluate_expression(const std::string& command);
        void initalize_operators();
    };

}




#endif