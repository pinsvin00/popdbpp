#include "expression.hpp"

int oper_prio(std::string oper) {
    if(oper == "-") return 1;
    if(oper ==  "/") return 2;
    return 0;
}

SEQL::Fragment::Fragment(std::string val) {
    std::map<std::string, int> operator_priority = {
        {"=", 0} , {"+", 1}, {"-", 1}, {"*", 2}, {"{", 0},
    };

    std::map<std::string, int> keyword_priority = {
        {"FUN", 1}, {"VAR", 1}
    };

    std::map<std::string, int> native_func_priority = {
        {"INSERT_BEGIN", 1},
    };

    if(operator_priority.count(val)) {
        this->type  = OPERATOR;
        this->value = val;
        this->prio  = operator_priority[val]; 
        return;
    }

    if(keyword_priority.count(val)) {
        this->type = KEYWORD;
        this->value = val;
        this->prio = keyword_priority[val];
        return;
    }

    if(native_func_priority.count(val)) {
        this->type = NATIVE_FUNCTION;
        this->value = val;
        this->prio = native_func_priority[val];
        return;
    }

    if(val[0] == '\"' || isdigit(val[0]) ) {
        this->type = LITERAL;
        this->value = val;
        return;
    }

    this->type = LITERAL;
    this->value = val;
    return;
}

SEQL::Keyword::Keyword(int arg_c, std::function<Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&)> executor)  {
    this->type = KEYWORD;
    this->executor = executor;
    this->arg_c = arg_c;
}


void SEQL::Engine::initialize_keywords() {
    this->keywords["FUN"] = Keyword(-1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        Function v  = Function();
        v.is_function = true;

        vars[args[0].value] = v;
        for (size_t i = 1; i < args.size() ; i++){
            if(args[i].type != VARIABLE) break;
            else v.argument_pattern.push_back(args[i]);
        }
        
        Fragment f = Fragment(args[0].value);
        f.type = FUNCTION;

        return f;
    });

    
    this->keywords["VAR"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        //jedno argumentowiec : (
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });

    this->keywords["IF"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        //jedno argumentowiec : (
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });


    this->keywords["FOR"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        //jedno argumentowiec : (
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });

}

void SEQL::Engine::initialize_operators() {
    this->operators["+"] = Operator(2, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args[0].type != args[1].type) {
            throw std::invalid_argument("SEQL ERROR : + operator cannot perform addition on 2 different types.");
        }

        return Fragment(std::to_string( std::stoi(args[0].value) + std::stoi(args[1].value) ));
    });
    this->operators["-"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 2){
            throw std::invalid_argument("SEQL ERROR : - operator requires 2 arguments");
        }
        if(args[0].type == 0) {
            throw std::invalid_argument("SEQL ERROR : - operator cannot perform operations on type NONE");
        }
        if(args[0].type != args[1].type) {
            throw std::invalid_argument("SEQL ERROR : - operator cannot perform addition on 2 different types.");
        } 


        return Fragment(std::to_string( std::stoi(args[0].value) - std::stoi(args[1].value) ));
    });
    this->operators["="] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 2){
            throw std::invalid_argument("SEQL ERROR : = operator requires 2 arguments");
        }
        vars[args[1].value].value = args[0].value;
        vars[args[1].value].type  = args[0].type;

        return Fragment(std::string(""));
    });


    this->operators["{"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : { expected statement");
        }

        return Fragment(std::string(""));
    });
}


void SEQL::Engine::evaluate_expression(const std::string& command) {
    std::vector<Fragment> current_fragments;
    std::string fragment_buffer = "";

    bool reading_string_literal = false;
    for (size_t i = 0; i < command.size(); i++){
        if(command[i] == '\"') reading_string_literal = !reading_string_literal;
        else if(command[i] == ' ' && !reading_string_literal) {
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

    std::stack<Fragment> stack;
    std::vector<Fragment> output;
    for(auto element : e.fragments) {
        if(element.type == OPERATOR) {
            Operator o = this->operators[element.value];
            std::vector<Fragment> args;
            for (size_t i = 0; i < o.argument_count; i++){
                args.push_back(stack.top());
                stack.pop();
            }
        
            Fragment f = o.executor(args, this->variables);
            stack.push(f);
        }
        if(element.type == NATIVE_FUNCTION) {
            std::vector<Fragment> strings;
            while(!stack.empty() ) {
                //that may be quite problematic, we should limit to taking n arguments
                strings.push_back(stack.top());
                stack.pop();
            }
            std::reverse(strings.begin(), strings.end());
            Base::Event event;
            event.arguments = strings;
            event.type = Base::EventType::InvokeFunction;
            this->base_engine->entry(event);
        }
        else {
            stack.push(element);
        }
    }


}
SEQL::Expression::Expression(std::vector<Fragment> fragments) {
    this->fragments = fragments;
}

void SEQL::Expression::convert_to_rpn() {
    std::vector<Fragment> stack;
    std::vector<Fragment> output;
    while( this->fragments.size() != 0 )  {
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
        else if(f.type == OPERATOR || f.type == KEYWORD || f.type == NATIVE_FUNCTION) {
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
