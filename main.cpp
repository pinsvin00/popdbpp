#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "expression.hpp"
#include "base_reader.hpp"
#include "base_controller.hpp"
#include "base.hpp"

using namespace std;


/*
Current issues
-> Statically assigned identifier of function
-> Non implemented in memory base
-> Validation of given records, type checking
-> NULL Value
-> Named parameters
-> Multiple databases

-> Non extensible base event class
*/



int main() {

    Base::Reader reader = Base::Reader("dbs/test.def");
    std::shared_ptr<Base::Info> base_information = make_shared<Base::Info>(reader.load());
    std::shared_ptr<Base::Engine> base_engine = std::make_shared<Base::Engine>(Base::Engine(base_information, Base::EngineMode::FILE));
    
    std::shared_ptr<SEQL::Engine> seql_engine = std::make_shared<SEQL::Engine>();

    seql_engine->base_engine = base_engine;
    seql_engine->evaluate_expression("INSERT_BEGIN test \"4\" \"2022-01-04\" \"Macieksmierdzigownem\"");


    // ifstream myfile ("test.pop");
    // std::stringstream ss;
    // std::string line;
    // if (myfile.is_open()){
    //     while ( getline (myfile,line) ){
    //         ss << line << '\n';
    //     }
    //     myfile.close();
    // }

    // std::string to_execute = ss.str();

    // SEQL::Engine e  = SEQL::Engine();
    // e.evaluate_expression(to_execute);
    //e.evaluate_expression(exp2);

    return 0;
}