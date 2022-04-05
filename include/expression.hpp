#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>
#include "base_common.hpp"
#include "base.hpp"

namespace SEQL {
    enum CommandIdParserError {
        INVALID_COMMAND_TYPE = 1,
    };

    enum FragmentType {
        LITERAL,
        OPERATOR,
        VARIABLE,
        KEYWORD,
        FUNCTION,
        NATIVE_FUNCTION,
    };

    class Variable {
        public:

        int type;
        bool is_function;
        bool has_native_code;
        std::string value;
        std::string name;
    };

    typedef std::function< Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&) > Executor;
    class Function : public Variable {
        public:
        std::vector<Fragment> argument_pattern;
    };

    class NativeFunction : public Function {
        Base::NativeFunctionIdentifier code;  
    };


    class Expression {
        public:
        bool valid{};
        std::vector<std::shared_ptr<Expression>> children;
        std::vector<Fragment> fragments; 
        std::shared_ptr<std::vector<Fragment>> fragment_ptr;
        std::shared_ptr<size_t> iter;

        void convert_to_rpn();
        Expression() = default;
        Expression(std::shared_ptr<size_t> iter, std::shared_ptr<std::vector<Fragment>> fragments);
    };

    class Operator : Fragment {
        public:
        int argument_count = 0;

        //TODO instead of passing whole engine, lets just pass some fragment of engine that contains needed data
        Executor executor;
        Operator() = default;
        Operator(int arg_c, Executor executor) {
            this->executor = executor;
            this->type = OPERATOR;
            this->argument_count = arg_c;
        }         
    };

    class Keyword : public Fragment {
        public:

        Executor executor;
        int arg_c;
        Keyword() = default;
        Keyword(int arg_c, Executor executor);
    };



    class Engine {
        std::map<std::string, Variable> variables;
        std::map<std::string, Keyword>  keywords;
        std::map<std::string, Operator> operators;
    public:
        Engine() = default;
        std::map<std::string, std::shared_ptr<Base::Engine>> engine_repository;
        std::stack<Expression> expression_stack;
        void tokenize(const std::string& command);
        void initialize_keywords();
        void initialize_operators();
        void evaluate(Expression e);
        void stuff(std::vector<Fragment> fragments);
    };

}




#endif