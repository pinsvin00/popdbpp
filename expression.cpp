#include "expression.hpp"

int oper_prio(std::string oper) {
    if(oper == "-") return 1;
    if(oper ==  "/") return 2;
    return 0;
}

SEQL::Fragment::Fragment(std::string val) {
    std::vector<std::string> operators = { "-", "=", "+", "AND", "OR", "/", "%", "(", ")" };

    auto it = std::find(operators.begin(), operators.end(), val);

    bool is_operator = it != operators.end() ? true : false;
    if(is_operator) {
        this->type = OPERATOR;
        this->value = val;
        return;
    }
    
    if(val[0] == '\"' || isdigit(val[0]) ) {
        this->type = LITERAL;
        this->value = val;
        return;
    }

    this->type = VARIABLE;
    this->value = val;
    return;


}
