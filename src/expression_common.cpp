//
// Created by pnsv0 on 06.04.2022.
//

#include "../include/expression_common.hpp"
#include "../include/expression.hpp"

SEQL::Fragment::Fragment(std::string val) {
    std::map<std::string, int> operator_priority = {
            {"=", 0} , {"+", 1}, {"-", 1}, {"*", 2}, {"{", 0},
    };

    std::map<std::string, int> keyword_priority = {
            {"FUN", 0}, {"VAR", 0}
    };

    std::map<std::string, int> native_func_priority = {
            {"INSERT_BEGIN", 0},
    };

    if(operator_priority.count(val)) {
        this->type  = OPERATOR;
        this->value = val;
        this->priority  = operator_priority[val];
        return;
    }

    if(keyword_priority.count(val)) {
        this->type = KEYWORD;
        this->value = val;
        this->priority = keyword_priority[val];
        return;
    }

    if(native_func_priority.count(val)) {
        this->type = NATIVE_FUNCTION;
        this->value = val;
        this->priority = native_func_priority[val];
        return;
    }

    bool is_number = isdigit(val[0]);
    bool is_string = val[0] == '\"';
    if(is_number || is_string) {
        if(is_number) {
            this->literal_type = NUMBER;
        }
        else {
            this->literal_type = STRING;
        }
        this->type = LITERAL;
        this->value = val;
        return;
    }

    this->type = VARIABLE;
    this->value = val;
}

SEQL::Keyword::Keyword(int arg_c, std::function<Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&)> executor)  {
    this->type = KEYWORD;
    this->executor = executor;
    this->arg_c = arg_c;
}