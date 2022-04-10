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
#include "Lexer.hpp"

namespace SEQL {
    enum class CommandIdParserError {
        INVALID_COMMAND_TYPE = 1,
    };


    class Engine {
        std::map<std::string, Variable> variables;
        std::map<std::string, Keyword>  keywords;
        std::map<std::string, Operator> operators;
        std::stack<Expression> expression_stack;
        Lexer lexer;
    public:
        Engine();
        std::map<std::string, std::shared_ptr<Base::Engine>> engine_repository;
        void execute(const std::string& command);
        void initialize_keywords();
        void initialize_operators();
        void evaluate(Expression& e);
        void build_expression_tree(std::shared_ptr<Expression> expr);

        void evaluate_start(std::shared_ptr<Expression> start_expr);

        void evaluate(std::shared_ptr<Expression> e);
    };

}




#endif