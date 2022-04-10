#ifndef EXPRESSION_COMMON_H
#define EXPRESSION_COMMON_H

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <functional>
#include <memory>

namespace SEQL {
    enum class FragmentType {
        LITERAL,
        OPERATOR,
        VARIABLE,
        KEYWORD,
        FUNCTION,
        NATIVE_FUNCTION,
        UNKNOWN,
    };

    enum class LiteralType {
        STRING,
        NUMBER,
        NIL,
    };


    class Variable {
    public:
        int type;
        std::string value;
        std::string name;
    };

    class Fragment {
    public:
        SEQL::FragmentType type;
        SEQL::LiteralType literal_type;
        int priority = -1;

        std::string value;
        std::string _string() const;
        Fragment() = default;
        Fragment(std::string val);
        Fragment(std::string val, const std::map<std::string, SEQL::Variable>& variables);
    };

    typedef std::function< Fragment(const std::vector<Fragment>&, std::map<std::string, Variable>&) > Executor;
    class Function : public Variable {
    public:
        std::vector<Fragment> argument_pattern;
    };

    class Expression {
    public:
        std::vector<Fragment> fragments;
        std::vector<std::shared_ptr<Expression>> children;
        std::vector<std::shared_ptr<Expression>> sub_expr;
        void convert_to_rpn();
        Fragment result;

        Expression() = default;

        std::string _string();
    };

    class Operator : Fragment {
    public:
        int argument_count = 0;
        Executor executor;
        Operator() = default;
        Operator(int arg_c, Executor executor);
    };

    class Keyword : public Fragment {
    public:
        Executor executor;
        int argument_count;
        Keyword() = default;
        Keyword(int arg_c, Executor executor);
    };



}

#endif