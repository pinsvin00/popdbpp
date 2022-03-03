#ifndef OPCODES_H
#define OPCODES_H

enum ExpressionType {
    COMMAND,
    DB_NAME, 
    VARIABLE,
    LITERAL,
    OPERATOR,
    MODIFIER,
    UNCLASSIFIED,
};

enum CommandID {
    NEW,
    POP,
    POP_ALL,
    SEL,
    SEL_ALL,
    DEL,
    DEL_ALL,
};

#endif