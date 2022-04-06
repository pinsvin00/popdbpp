#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <vector>
#include <stack>
#include "base_common.hpp"
#include "expression_common.hpp"
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
        UNKNOWN,
        NIL,
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
        std::vector<Fragment> fragments;
        void convert_to_rpn();
        Fragment result;

        Expression() = default;
    };

    class Operator : Fragment {
        public:
        int argument_count = 0;
        Executor executor;
        Operator() = default;
        Operator(int arg_c, Executor executor);
    };

    class Keyword : public Fragment {
        public:
        Executor executor;
        int argument_count;
        Keyword() = default;
        Keyword(int arg_c, Executor executor);
    };



    class Engine {
        std::map<std::string, Variable> variables;
        std::map<std::string, Keyword>  keywords;
        std::map<std::string, Operator> operators;
        std::stack<Expression> expression_stack;
    public:
        Engine();
        std::map<std::string, std::shared_ptr<Base::Engine>> engine_repository;
        void execute(const std::string& command);
        void initialize_keywords();
        void initialize_operators();
        void evaluate(Expression& e);
    };

}




#endif