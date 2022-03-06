#include "expression.hpp"

int oper_prio(std::string oper) {
    if(oper == "-") return 1;
    if(oper ==  "/") return 2;
    return 0;
}

SEQL::Fragment::Fragment(std::string &val) {
    std::vector<std::string> operators  = {"=", "+", "-", "*", "var"};
    std::vector<int> priorities = {0, 1, 2, 1, 1};
    auto it = std::find(operators.begin(), operators.end(), val);
    bool is_operator = it != operators.end() ? true : false;

    if(is_operator) {
        this->is_operator = true;
        this->type  = OPERATOR;
        this->value = val;
        this->prio  = priorities.at(it - operators.begin()); 
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


SEQL::Engine::Engine() {
    this->initalize_operators();
}

void SEQL::Engine::initalize_operators() {
    // this->operators["+"] = Operator(2, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
    //     if(args.size() != 2){
    //         throw std::invalid_argument("SEQL ERROR : + operator requires 2 arguments");
    //     }
    //     if(args[0].type == 0) {
    //         throw std::invalid_argument("SEQL ERROR : + operator cannot perform operations on type NONE");
    //     }
    //     if(args[0].type != args[1].type) {
    //         throw std::invalid_argument("SEQL ERROR : + operator cannot perform addition on 2 different types.");
    //     } 
    //     return Fragment(std::to_string( std::stoi(args[0].value) + std::stoi(args[1].value) ));
    // });
    // this->operators["-"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
    //     if(args.size() != 2){
    //         throw std::invalid_argument("SEQL ERROR : - operator requires 2 arguments");
    //     }
    //     if(args[0].type == 0) {
    //         throw std::invalid_argument("SEQL ERROR : - operator cannot perform operations on type NONE");
    //     }
    //     if(args[0].type != args[1].type) {
    //         throw std::invalid_argument("SEQL ERROR : - operator cannot perform addition on 2 different types.");
    //     } 


    //     return Fragment(std::to_string( std::stoi(args[0].value) - std::stoi(args[1].value) ));
    // });
    // this->operators["var"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
    //     if(args.size() != 1){
    //         throw std::invalid_argument("SEQL ERROR : = operator requires 2 arguments");
    //     }
    //     vars[args[0].value] = Variable();          
    //     return Fragment(std::string(""));
    // });

    // this->operators["="] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
    //     if(args.size() != 2){
    //         throw std::invalid_argument("SEQL ERROR : = operator requires 2 arguments");
    //     }
    //     vars[args[0].value].value = args[1].value;
    //     vars[args[0].value].type  = args[1].type;

    //     return Fragment(s"");
    // });
}


void SEQL::Engine::evaluate_expression(const std::string& command) {
    //std::cout << command << std::endl;
    std::vector<Fragment> current_fragments;
    std::string fragment_buffer = "";

    for (size_t i = 0; i < command.size(); i++){
        if(command[i] == ' ') {
            Fragment f = Fragment(fragment_buffer); 
            current_fragments.push_back(f);
            fragment_buffer = "";
        }
        else {
            fragment_buffer += command[i];
        }
    }

    Fragment f = Fragment(fragment_buffer); 
    fragment_buffer = "";
    current_fragments.push_back(f);


    Expression e = Expression(current_fragments);
    e.convert_to_rpn();

    //should we convert to rpn on demand or during call of evaluate?
    std::stack<Fragment> stack;
    for(auto element : e.fragments) {
        
    }


}
SEQL::Expression::Expression(std::vector<Fragment> fragments) {
    this->fragments = fragments;
}

void SEQL::Expression::convert_to_rpn() {
    //this->expression_fragments = {};
    std::vector<Fragment> stack;
    std::vector<Fragment> output;
    while( this->fragments.size() != 0 )  {
        Fragment f = this->fragments[0];
        this->fragments.erase(this->fragments.begin());
        if(f.value == "(") {
            stack.push_back(f);
        }
        else if(f.value == ")") {
            while(stack.back().value != ")") {
                Fragment p = stack[stack.size() - 1]; 
                stack.pop_back();
                output.push_back(p);
            }
        }
        else if(f.is_operator) {
            while(!stack.empty()){
                if (f.prio > stack.back().prio) break;
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
