#ifndef RECORD_CONSTRUCTOR_H
#define RECORD_CONSTRUCTOR_H

#include <iostream>
#include <string>
#include <vector>
#include "base_common.hpp"


namespace Base {
    class RecordConstructor {
        public:
        RecordConstructor() = default;
        Record construct(std::vector<SEQL::Fragment> arguments);
    };
}

#endif