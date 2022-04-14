//
// Created by pnsv0 on 13.04.2022.
//

#ifndef POPDB_SEQLCODESTRUCTURE_HPP
#define POPDB_SEQLCODESTRUCTURE_HPP
#include <vector>
#include <iostream>
#include "expression_common.hpp"

using namespace SEQL;


class Statement {
public:
    virtual void invoke();
};


class Block {
    std::vector<std::shared_ptr<Statement>> lines;
};

class IfStatement : public Statement {
public:
    Expression condition;
    Block success_block;

    std::shared_ptr<IfStatement> next_statment;
    IfStatement();
    void invoke() override;
};


class ForStatement : public Statement {
public:
    Expression start;
    Expression break_condition;
    Expression each;

    Block for_block;
    ForStatement();
    void invoke() override;

};


class CodeElement {
    Expression expr;
};

class SEQLCodeStructure  {
public:
    std::vector<CodeElement> code_elements;
};


#endif //POPDB_SEQLCODESTRUCTURE_HPP
