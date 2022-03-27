#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <queue>
#include "base_common.hpp"

namespace Base {
    enum EngineMode {
        MEMORY, 
        FILE,
    };

    class Record {
        public:
        std::shared_ptr<RecordDefinition> definition;
        std::vector<std::byte> data;
    };

    class Engine {
        std::shared_ptr<Base::Info> base_info;
        EngineMode mode;

        std::string temp_file_path;
        std::string base_file_path;

        public:
            Engine() {
                this->base_file_path = this->base_info->data_path.c_str();
                this->temp_file_path = (this->base_info->data_path + "temp").c_str();
            };

            Engine(std::shared_ptr<Base::Info> base_info, EngineMode mode) {
                this->base_info = base_info;
                this->mode = mode;
            }

            std::queue<Record> record_queue;
            std::fstream obtain_base_file_ptr();
            std::fstream obtain_temp_file_ptr();

            void swap_buffer_file() {
                remove(this->base_file_path.c_str());
                rename(this->temp_file_path.c_str(), this->base_file_path.c_str());
            };

            Record pop_record();
            Record pop_nth(size_t nth);
            Record select_nth(size_t nth);
            Record delete_nth(size_t nth);
            void   insert_begin(std::shared_ptr<Record> record);
            void   insert(std::shared_ptr<Record> record);
    };

    class InMemoryEngine : public Engine {

    };

    class DiskEngine : public Engine {

    };
}
