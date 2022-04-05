
#include <string>
#include "include/expression.hpp"
#include "include/base_reader.hpp"

#include "include/base.hpp"

using namespace std;


/*
Current issues
-> Validation of given records, type checking
-> NULL Value
-> Evalutor fixes
*/



int main() {

    Base::Reader reader = Base::Reader("dbs/test.def");
    std::shared_ptr<Base::Info> base_information = make_shared<Base::Info>(reader.load());

    std::map<std::string , std::shared_ptr<Base::Engine>> base_repository;
    std::shared_ptr<Base::Engine> base_engine = std::make_shared<Base::Engine>(Base::Engine(base_information, Base::EngineMode::FILE));
    std::shared_ptr<SEQL::Engine> seql_engine = std::make_shared<SEQL::Engine>();

    base_repository["test"] = base_engine;

    seql_engine->engine_repository = base_repository;
    seql_engine->initialize_operators();
    seql_engine->initialize_keywords();
    seql_engine->tokenize("INSERT_BEGIN,test,20 - 9,3 + 10,\"somedata\"");
    //seql_engine->tokenize("2 + 22 , 3 + 33 , 4 + 44");

    return 0;
}