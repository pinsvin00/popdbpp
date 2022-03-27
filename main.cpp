#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "expression.hpp"
#include "base_reader.hpp"
#include "base.hpp"

using namespace std;


int main() {

    Base::Reader reader = Base::Reader("dbs/test.def");
    std::shared_ptr<Base::Info> base_information = make_shared<Base::Info>(reader.load());
    Base::Engine base_engine = Base::Engine(base_information, Base::EngineMode::FILE);

    //jak przekazywac eventy do silnika z SEQLa?
    //seql_engine.evaluate_expression("INSERT_BEGIN test ID = '1' DATE = '2022-27-03' MESSAGE = 'Hello World';");

    std::shared_ptr<Base::Record> record = make_shared<Base::Record>(); 
    base_engine.insert_begin(record);



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