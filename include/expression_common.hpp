#ifndef EXPRESSION_COMMON_H
#define EXPRESSION_COMMON_H

#include <iostream>
#include <vector>
#include <map>




namespace SEQL {

    enum LiteralType {
        STRING,
        NUMBER,
    };


    class Variable {
    public:
        int type;
        std::string value;
        std::string name;
    };


    class Fragment {
        public:
        int type;
        int literal_type;
        int priority;

        std::string value;

        Fragment() = default;
        Fragment(std::string val);
        Fragment(std::string val, const std::map<std::string, SEQL::Variable>& variables);
    };


}

#endif