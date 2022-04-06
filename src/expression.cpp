#include "../include/expression.hpp"

void SEQL::Engine::execute(const std::string& command) {

    std::string fragment_buffer;
    bool reading_string_literal = false;

    std::vector<Expression> expressions;
    Expression current_expression;

    for (size_t i = 0; i < command.size(); i++){
        if (command[i] == ',') {
            auto fragment = Fragment(fragment_buffer, this->variables);
            current_expression.fragments.push_back(fragment);
            expressions.push_back(current_expression);
            current_expression = Expression();
            fragment_buffer = "";
        }
        else if(command[i] == '\"') reading_string_literal = !reading_string_literal;
        else if(command[i] == ' ' && !reading_string_literal) {
            if(fragment_buffer.empty() || fragment_buffer == " ") continue;
            Fragment f = Fragment(fragment_buffer, this->variables);
            current_expression.fragments.push_back(f);
            fragment_buffer = "";
        }
        else {
            fragment_buffer += command[i];
        }
    }


    auto fragment = Fragment(fragment_buffer, this->variables);
    current_expression.fragments.push_back(fragment);
    expressions.push_back(current_expression);
    std::reverse(expressions.begin(), expressions.end());
    for(auto element : expressions) {
        element.convert_to_rpn();
        this->evaluate(element);
        this->expression_stack.push(element);
    }
    while(!this->expression_stack.empty()) this->expression_stack.pop();
}

void SEQL::Engine::evaluate(Expression& e) {
    std::stack<Fragment> stack;
    std::vector<Fragment> output;

    for(auto &element : e.fragments) {
        if(element.type == KEYWORD) {
            Keyword k = this->keywords[element.value];
            std::vector<Fragment> args;
            for (size_t i = 0; i < k.argument_count; i++){
                args.push_back(stack.top());
                stack.pop();
            }

            Fragment f = k.executor(args, this->variables);
            f.type = VARIABLE;
            stack.push(f);
        }
        else if(element.type == OPERATOR) {
            Operator o = this->operators[element.value];
            std::vector<Fragment> args;
            for (size_t i = 0; i < o.argument_count; i++){
                args.push_back(stack.top());
                stack.pop();
            }
        
            Fragment f = o.executor(args, this->variables);
            stack.push(f);
        }
        else if(element.type == VARIABLE)  {
            if(this->variables.count(element.value) == 0) {
                throw std::invalid_argument("SEQL Error: Variable with such name doesn't exist");
            }
            Variable v = this->variables[element.value];
            element.type = LITERAL;
            element.literal_type = NUMBER;
            element.value = v.value;
            stack.push(element);
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
                throw std::invalid_argument("SEQL Error: Base " + base_name + " doesn't exist.");
            }
            std::shared_ptr <Base::Engine> base = this->engine_repository[base_name];
            base->entry(event);
        }
        else {
            stack.push(element);
        }
    }
    if(stack.empty()) {
        e.result = Fragment("NULL");
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

SEQL::Operator::Operator(int arg_c, SEQL::Executor executor)  {
    this->executor = std::move(executor);
    this->type = OPERATOR;
    this->argument_count = arg_c;
}
