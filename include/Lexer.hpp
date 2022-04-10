//
// Created by pnsv0 on 09.04.2022.
//

#ifndef POPDB_LEXER_H
#define POPDB_LEXER_H

#include <iostream>
#include "expression_common.hpp"

class Lexer {
public:
    std::vector<SEQL::Fragment> tokenize(const std::string& line);
};


#endif //POPDB_LEXER_H