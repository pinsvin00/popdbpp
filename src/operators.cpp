//
// Created by pnsv0 on 06.04.2022.
//

#include "../include/expression_common.hpp"
#include "../include/expression.hpp"

void SEQL::Engine::initialize_keywords() {
    this->keywords["FUN"] = Keyword(-1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        Function v  = Function();
        vars[args[0].value] = v;
        for (size_t i = 1; i < args.size() ; i++){
            if(args[i].type != SEQL::FragmentType::VARIABLE) break;
            else v.argument_pattern.push_back(args[i]);
        }

        Fragment f = Fragment(args[0].value);
        f.type = SEQL::FragmentType::FUNCTION;

        return f;
    });

    this->keywords["IF"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();
        return Fragment(args[0].value);
    });

    this->keywords["VAR"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();
        Fragment f = Fragment(args[0].value);
        f.type = SEQL::FragmentType::VARIABLE;
        return f;
    });


}

void SEQL::Engine::initialize_operators() {
    this->operators["+"] = Operator(2, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args[0].literal_type != args[1].literal_type) {
            throw std::invalid_argument("SEQL ERROR : + operator cannot perform addition on 2 different types.");
        }

        if(args[0].literal_type == SEQL::LiteralType::STRING) {
            return Fragment(args[0].value + args[1].value);
        }
        else if(args[1].literal_type == SEQL::LiteralType::NUMBER) {
            return Fragment(std::to_string( std::stoi(args[0].value) + std::stoi(args[1].value) ));
        }

    });

    this->operators["*"] = Operator(2, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args[0].literal_type == SEQL::LiteralType::STRING) {
            throw std::invalid_argument("SEQL ERROR : * operator cannot perform multiplications on any string type");
        }
        else if(args[1].literal_type == SEQL::LiteralType::NUMBER) {
            return Fragment(std::to_string( std::stoi(args[0].value) * std::stoi(args[1].value) ));
        }

    });

    this->operators["-"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 2){
            throw std::invalid_argument("SEQL ERROR : - operator requires 2 arguments");
        }
        if(args[0].literal_type != SEQL::LiteralType::NUMBER || args[1].literal_type != SEQL::LiteralType::NUMBER) {
            throw std::invalid_argument("SEQL ERROR : - operator cannot perform addition on 2 different types.");
        }

        return Fragment(std::to_string( std::stoi(args[1].value) - std::stoi(args[0].value) ));
    });
    this->operators["="] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 2){
            throw std::invalid_argument("SEQL ERROR : = operator requires 2 arguments");
        }
        vars[args[1].value].value = args[0].value;
        vars[args[1].value].type  = static_cast<int>(args[0].type);

        return Fragment(std::string(""));
    });

}

SEQL::Engine::Engine() {
    this->initialize_operators();
    this->initialize_keywords();
    this->lexer = Lexer();
}
