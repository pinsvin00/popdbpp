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
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });

    this->keywords["IF"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });


    this->keywords["FOR"] = Keyword(1, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args.size() != 1){
            throw std::invalid_argument("SEQL ERROR : var operator requires 1 argument");
        }
        vars[args[0].value] = Variable();          
        return Fragment(args[0].value);
    });

}

void SEQL::Engine::initialize_operators() {
    this->operators["+"] = Operator(2, [](const std::vector<Fragment>& args, std::map<std::string, Variable>& vars) {
        if(args[0].literal_type != args[1].literal_type) {
            throw std::invalid_argument("SEQL ERROR : + operator cannot perform addition on 2 different types.");
        }

        if(args[0].literal_type == STRING) {
            return Fragment(args[0].value + args[1].value);
        }
        else if(args[1].literal_type == NUMBER) {
            return Fragment(std::to_string( std::stoi(args[0].value) + std::stoi(args[1].value) ));
        }

    });
    this->operators["-"] = Operator(2, [](std::vector<Fragment> args, std::map<std::string, Variable>& vars) {
        if(args.size() != 2){
            throw std::invalid_argument("SEQL ERROR : - operator requires 2 arguments");
        }
        if(args[0].literal_type != NUMBER || args[1].literal_type != NUMBER) {
            throw std::invalid_argument("SEQL ERROR : - operator cannot perform addition on 2 different types.");
        }

        return Fragment(std::to_string( std::stoi(args[1].value) - std::stoi(args[0].value) ));
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
void SEQL::Engine::tokenize(const std::string& command) {

    std::string fragment_buffer;
    bool reading_string_literal = false;

    std::vector<Expression> expressions;
    Expression current_expression;

    for (size_t i = 0; i < command.size(); i++){
        if (command[i] == ',') {
            auto fragment = Fragment(fragment_buffer);
            current_expression.fragments.push_back(fragment);
            expressions.push_back(current_expression);
            current_expression = Expression();
            fragment_buffer = "";
        }
        else if(command[i] == '\"') reading_string_literal = !reading_string_literal;
        else if(command[i] == ' ' && !reading_string_literal) {
            if(fragment_buffer.empty() || fragment_buffer == " ") continue;

            Fragment f = Fragment(fragment_buffer);
            current_expression.fragments.push_back(f);
            fragment_buffer = "";
        }
        else {
            fragment_buffer += command[i];
        }
    }


    auto fragment = Fragment(fragment_buffer);
    current_expression.fragments.push_back(fragment);
    expressions.push_back(current_expression);
    std::reverse(expressions.begin(), expressions.end());
    for(auto element : expressions) {
        element.convert_to_rpn();
        this->evaluate(element);
        this->expression_stack.push(element);
    }

}

void SEQL::Engine::evaluate(Expression& e) {
    std::stack<Fragment> stack;
    std::vector<Fragment> output;

    for(const auto &element : e.fragments) {
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
        else if(element.type == NATIVE_FUNCTION) {
            std::vector<Fragment> strings;
            while( !expression_stack.empty() ) {
                strings.push_back( expression_stack.top().result );
                expression_stack.pop();
            }
            Base::FunctionDispatchEvent event (element.value, strings);
            std::string base_name = strings[0].value;
            if(this->engine_repository.count(base_name) == 0) {
                std::cout << "ERROR! Base doesn't exist." << std::endl;
                return;
            }
            std::shared_ptr <Base::Engine> base = this->engine_repository[base_name];
            base->entry(event);
        }
        else {
            stack.push(element);
        }
    }
    if(stack.empty()) {
        e.result = Fragment("0");
    }
    else {
        e.result = stack.top();
    }

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
        else if(f.type == OPERATOR || f.type == KEYWORD || f.type == NATIVE_FUNCTION) {
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
