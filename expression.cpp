#include "expression.hpp"

int oper_prio(std::string oper) {
    if(oper == "-") return 1;
    if(oper ==  "/") return 2;
    return 0;
}

SEQL::Fragment::Fragment(std::string val) {
    std::vector<std::string> operators = { "-", "=", "+", "WHERE", "AND", "OR", "/", "%", "(", ")" };

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


void SEQL::Engine::evaluate_command(const std::string& command) {
    std::vector<Expression> expressions;
    std::vector<Fragment> current_fragments;
    std::string fragment_buffer = "";

    for (size_t i = 0; i < command.size(); i++){
        if(command[i] == ' ') {
            Fragment f = Fragment(fragment_buffer); 
            fragment_buffer = "";
        }

        if(command[i] == '=') {
            expressions.push_back(Expression(current_fragments));
            current_fragments = {};
        }
    }

    
    
}


SEQL::Expression::Expression(std::vector<Fragment>& fragments) {
    this->expression_fragments = fragments;
}

void SEQL::Expression::to_rpn() {
    //this->expression_fragments = {};

    std::vector<Fragment> stack;
    std::vector<Fragment> output;
    while( this->expression_fragments.size() != 0 )  {
        Fragment f = this->expression_fragments[0];
        this->expression_fragments.erase(this->expression_fragments.begin());
        
        if(f.value == "(") {
            stack.push_back(f);
        }
        else if(f.value == "(") {
            while(stack[stack.size() - 1].value != ")") {
                Fragment p = stack[stack.size() - 1]; 
                stack.pop_back();
                output.push_back(p);
            }
        }
        else { 
            output.push_back(f);
        }
    }

    while(!stack.empty()) {
        Fragment f = stack[stack.size() - 1];
        output.push_back(f);
        stack.pop_back();
    }

    this->expression_fragments = output; 

}
