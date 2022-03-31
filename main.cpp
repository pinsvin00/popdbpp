#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "expression.hpp"
#include "base_reader.hpp"
#include "base_controller.hpp"
#include "base.hpp"

using namespace std;


int main() {

    Base::Reader reader = Base::Reader("dbs/test.def");
    vector<byte> chuj;
    std::shared_ptr<Base::Info> base_information = make_shared<Base::Info>(reader.load());
    std::shared_ptr<Base::Engine> base_engine = std::make_shared<Base::Engine>(Base::Engine(base_information, Base::EngineMode::FILE));
    std::shared_ptr<SEQL::Engine> seql_engine = std::make_shared<SEQL::Engine>();

    //todo some facade of interaction ? :thinking:
    seql_engine->base_engine = base_engine;
    seql_engine->evaluate_expression("INSERT_BEGIN test 1 2022-27-03 HelloWorld");


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