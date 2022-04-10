
#include <string>
#include <map>
#include "include/expression.hpp"
#include "include/base_reader.hpp"
#include "include/base.hpp"

using namespace std;

int main() {

    Base::Reader reader = Base::Reader("dbs/test.def");
    std::shared_ptr<Base::Info> base_information = make_shared<Base::Info>(reader.load());

    map<string ,shared_ptr<Base::Engine>> base_repository;
    shared_ptr<Base::Engine> testBase = make_shared<Base::Engine>(Base::Engine(base_information, Base::EngineMode::FILE));
    shared_ptr<SEQL::Engine> seql_engine = std::make_shared<SEQL::Engine>();

    base_repository["test"] = testBase;
    seql_engine->engine_repository = base_repository;
    1 + 2, 1 + 3;
    seql_engine->execute("VAR funny = 548");
    seql_engine->execute("VAR monke = funny + 200 * 2");
    seql_engine->execute("INSERT_BEGIN ( test , monke - funny , 3 + 10 , \"somedata\" )");
    std::map<std::string, std::string> record
        = testBase->record_to_map( testBase->pop_record() );

    return 0;
}