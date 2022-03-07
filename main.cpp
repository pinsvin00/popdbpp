#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "expression.hpp"

using namespace std;


int main() {
    ifstream myfile ("test.pop");
    std::stringstream ss;
    std::string line;
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            ss << line << '\n';
        }
        myfile.close();
    }

    std::string to_execute = ss.str();

    SEQL::Engine e  = SEQL::Engine();
    e.evaluate_expression(to_execute);
    //e.evaluate_expression(exp2);

    return 0;
}