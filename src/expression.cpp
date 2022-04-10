#include "../include/expression.hpp"

void SEQL::Engine::execute(const std::string& command) {
    std::vector<Fragment> fragments = this->lexer.tokenize(command);
    std::shared_ptr<Expression> expr = std::make_shared<Expression>();
    expr->fragments = fragments;
    this->build_expression_tree(expr);
    this->evaluate_start(expr);

//    e.convert_to_rpn();
//    std::cout << "Evaluating expression : " << e._string()  << std::endl;
//    this->evaluate(e);
}

void SEQL::Engine::build_expression_tree(std::shared_ptr<Expression> expr) {
    std::shared_ptr<Expression> sub_expr;


    bool is_poly_expression = false;
    std::vector<Fragment> frag_buffer;
    for (int i = 0; i < expr->fragments.size(); ++i) {
        auto element = expr->fragments[i];
        if(element.type == SEQL::FragmentType::UNKNOWN){
            if(this->variables.count(element.value) != 0)
                element.type = SEQL::FragmentType::VARIABLE;
        }
        else if(element.value == "(") {
            bool closed = false;
            std::vector<Fragment> child_expr_flags;
            std::shared_ptr<Expression> child_expr = std::make_shared<Expression>();
            int j;
            //todo implement with std::find
            for (j = i; j < expr->fragments.size(); ++j) {
                auto sub_element = expr->fragments[j];
                if(sub_element.value == ")") {
                    child_expr->fragments = child_expr_flags;
                    closed = true;
                    break;
                }
                else {
                    child_expr_flags.push_back(sub_element);
                }
            }
            if(closed) {
                i = j;
                child_expr->fragments = child_expr_flags;
                this->build_expression_tree(child_expr);
            }
            else
                return;

        }
        else if(element.value == ",") {
            is_poly_expression = true;
            sub_expr->fragments = frag_buffer;
            expr->sub_expr.push_back(sub_expr);

            sub_expr = std::make_shared<Expression>();
            frag_buffer.clear();
        }
        else {
            frag_buffer.push_back(expr->fragments[i]);
        }
    }

    if(is_poly_expression) {
        sub_expr->fragments = frag_buffer;
        expr->sub_expr.push_back(sub_expr);
        frag_buffer.clear();
    }
    else {
        expr->fragments = frag_buffer;
    }
}

void SEQL::Engine::evaluate_start(std::shared_ptr<Expression> start_expr) {
    std::queue<std::shared_ptr<Expression>> eval_queue;
    std::stack<std::shared_ptr<Expression>> trail;

    eval_queue.push(start_expr);

    while(!eval_queue.empty()) {
        auto expr = eval_queue.front();
        eval_queue.pop();
        trail.push(expr);
        for(const auto& child : expr->children) {
            eval_queue.push(child);
        }
    }

    while(!trail.empty()) {
        auto expr = trail.top();
        trail.pop();
        expr->convert_to_rpn();
        this->evaluate(expr);
    }

    this->evaluate(start_expr);


}

void SEQL::Engine::evaluate(std::shared_ptr<Expression> e) {

    for(auto & element : e->sub_expr) {
        element->convert_to_rpn();
        evaluate(element);
    }

    std::stack<Fragment> stack;
    std::vector<Fragment> output;

    for(auto &element : e->fragments) {
        if(element.type == SEQL::FragmentType::KEYWORD) {
            Keyword k = this->keywords[element.value];
            std::vector<Fragment> args;
            for (size_t i = 0; i < k.argument_count; i++){
                if(stack.empty()) {
                    throw std::invalid_argument("SEQL ExpressionParser : Too few arguments! In expression : "  + e._string() );
                }
                args.push_back(stack.top());
                stack.pop();
            }

            Fragment f = k.executor(args, this->variables);
            f.type = SEQL::FragmentType::VARIABLE;
            stack.push(f);
        }
        else if(element.type == SEQL::FragmentType::OPERATOR) {
            Operator o = this->operators[element.value];
            std::vector<Fragment> args;
            for (size_t i = 0; i < o.argument_count; i++){
                if(stack.empty()) {
                    throw std::invalid_argument("SEQL ExpressionParser : Too few arguments! In expression : "  + e._string() );
                }

                args.push_back(stack.top());
                stack.pop();
            }

            std::cout << "Running operator " << element.value << " with arguments :" << std::endl;
            for(const auto& el : args) {
                std::cout << el._string() << std::endl;
            }

        
            Fragment f = o.executor(args, this->variables);
            stack.push(f);
        }
        else if(element.type == SEQL::FragmentType::VARIABLE)  {
            if(this->variables.count(element.value) == 0) {
                throw std::invalid_argument("SEQL Error: Variable with name " + element.value + " doesn't exist");
            }
            Variable v = this->variables[element.value];
            element.type = SEQL::FragmentType::LITERAL;
            element.literal_type = SEQL::LiteralType::NUMBER;
            element.value = v.value;
            stack.push(element);
        }
        else if(element.type == SEQL::FragmentType::NATIVE_FUNCTION) {
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
    std::cout << "SUCCESS : Evaluated expression: " << e->_string()  << std::endl;
    if(stack.empty()) {
        e->result = Fragment("NIL");
    }
    else {
        e->result = stack.top();
    }

}