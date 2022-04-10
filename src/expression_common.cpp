//
// Created by pnsv0 on 06.04.2022.
//

#include "../include/expression_common.hpp"

#include <utility>
#include "../include/expression.hpp"

SEQL::Fragment::Fragment(std::string val) {
    if(val == "NIL") {
        this->type = SEQL::FragmentType::LITERAL;
        this->literal_type = SEQL::LiteralType::NIL;
        this->value = "NIL";
        this->priority = 0;
        return;
    }

    std::map<std::string, int> operator_priority = {
            {"=", 0} , {"+", 1}, {"-", 1}, {"*", 2}, {"{", 0},
            {",", 3},
    };

    std::map<std::string, int> keyword_priority = {
            {"FUN", 0}, {"VAR", 0}
    };

    std::map<std::string, int> native_func_priority = {
            {"INSERT_BEGIN", 0},
    };

    if(operator_priority.count(val)) {
        this->type  = SEQL::FragmentType::OPERATOR;
        this->value = val;
        this->priority  = operator_priority[val];
        return;
    }

    if(keyword_priority.count(val)) {
        this->type = SEQL::FragmentType::KEYWORD;
        this->value = val;
        this->priority = keyword_priority[val];
        return;
    }

    if(native_func_priority.count(val)) {
        this->type = SEQL::FragmentType::NATIVE_FUNCTION;
        this->value = val;
        this->priority = native_func_priority[val];
        return;
    }

    bool is_number = isdigit(val[0]);
    bool is_string = val[0] == '\"';
    if(is_number || is_string) {
        if(is_number) {
            this->literal_type = SEQL::LiteralType::NUMBER;
        }
        else {
            this->literal_type = SEQL::LiteralType::STRING;
        }
        this->type = SEQL::FragmentType::LITERAL;
        this->value = val;
        return;
    }

    this->type = SEQL::FragmentType::UNKNOWN;
    this->value = val;
}

SEQL::Fragment::Fragment(std::string val, const std::map<std::string, SEQL::Variable>& variables) : Fragment(val) {
    if(variables.count(val) != 0) {
        this->type = SEQL::FragmentType::VARIABLE;
    }
}

std::string SEQL::Fragment::_string() const {
    return this->value;
}

SEQL::Keyword::Keyword(int arg_c, std::function<Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&)> executor)  {
    this->type = SEQL::FragmentType::KEYWORD;
    this->executor = std::move(executor);
    this->argument_count = arg_c;
}


void SEQL::Expression::convert_to_rpn() {
    std::vector<Fragment> stack;
    std::vector<Fragment> output;
    while(!this->fragments.empty())  {
        Fragment f = this->fragments[0];
        this->fragments.erase(this->fragments.begin());
        if(f.value == "(") {
            stack.push_back(f);
        }
        else if(f.value == ")") {
            while(stack.back().value != "(") {
                Fragment p = stack[stack.size() - 1];
                stack.pop_back();
                output.push_back(p);
            }
            stack.pop_back();
        }
        else if(f.value == ",") {
            while(!stack.empty()){
                output.push_back(stack.back());
                stack.pop_back();
            }
        }
        else if(f.type == SEQL::FragmentType::OPERATOR || f.type == SEQL::FragmentType::KEYWORD ||
        f.type == SEQL::FragmentType::FUNCTION || f.type == SEQL::FragmentType::NATIVE_FUNCTION) {
            while(!stack.empty()){
                if (f.priority > stack.back().priority) break;
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(f);
        }
        else{
            output.push_back(f);
        }
    }
    while(!stack.empty()) {
        Fragment f = stack[stack.size() - 1];
        output.push_back(f);
        stack.pop_back();
    }
    this->fragments = output;
}

std::string SEQL::Expression::_string() {
    std::string s;
    for(const auto & element : fragments) {
        s += element.value + " ";
    }
    return s;
}

SEQL::Operator::Operator(int arg_c, SEQL::Executor executor)  {
    this->executor = std::move(executor);
    this->type = SEQL::FragmentType::OPERATOR;
    this->argument_count = arg_c;
}
