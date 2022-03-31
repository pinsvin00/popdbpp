#ifndef EXPRESSION_COMMON_H
#define EXPRESSION_COMMON_H

#include <iostream>
#include <vector>
#include <map>

namespace SEQL {
    class Fragment {
        public:
        int type;
        std::string value;
        int prio;

        Fragment() = default;
        Fragment(std::string val);
    };
}

#endif