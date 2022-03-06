#include <iostream>
#include <string>
#include "expression.hpp"

using namespace std;


int main() {
    
    
    auto exp1 = "var result = 1 + 2";
    string exp2 = "var result = 1 + ( 2 * 3 )";

    SEQL::Engine e  = SEQL::Engine();
    e.evaluate_expression(exp2);
    //e.evaluate_expression(exp2);

    return 0;
}