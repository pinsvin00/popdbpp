#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <queue>
#include "base_common.hpp"
#include "record_constructor.hpp"

namespace Base {
    enum EngineMode {
        MEMORY, 
        FILE,
    };
    class Engine {
        std::shared_ptr<Base::Info> info;
        EngineMode mode;

        std::string temp_file_path;
        std::string base_file_path;

        public:
            Engine() {
                this->base_file_path = this->info->data_path.c_str();
                this->temp_file_path = (this->info->data_path + "temp").c_str();
            };

            void entry(Event evnt) {
                Base::RecordConstructor constructor;
                std::shared_ptr<Record> record = std::make_shared<Record>(constructor.construct( this->info->record_definition, evnt.arguments));
                if(evnt.function_identifier == INSERT_BEGIN) {
                    this->insert_begin(record);
                }
            }


            Engine(std::shared_ptr<Base::Info> info, EngineMode mode) {
                this->info = info;
                this->mode = mode;

                this->base_file_path = this->info->data_path.c_str();
                this->temp_file_path = (this->info->data_path + "temp").c_str();
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

#endif