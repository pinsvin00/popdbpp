
#ifndef FUNCTION_REGISTRY_H
#define FUNCTION_REGISTRY_H

#include <map>
#include <string>

namespace Base {
    enum NativeFunctionIdentifier {
        INSERT_BEGIN,
        INSERT,
        SELECT,
        POP,
        POP_NTH,
        DELETE_NTH,
    };
}
#endif