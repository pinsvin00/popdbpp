#include <iostream>
#include <functional>
#include "base_common.hpp"
#include "base.hpp"
#include "expression.hpp"

namespace Base {
    class Controller {
        public:
        SEQL::Engine seql_engine;
        Base::Engine base_engine;

        std::function<void(Event)> event_dispatch;
        
        Controller() {
            seql_engine = SEQL::Engine();
            base_engine = Base::Engine();
        }
    };
}
