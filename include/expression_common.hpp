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

    class Fragment {
        public:
        int type;
        int literal_type;
        int priority;

        std::string value;

        Fragment() = default;
        Fragment(std::string val);
    };
}

#endif