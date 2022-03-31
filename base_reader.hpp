#ifndef DB_READER
#define DB_READER

#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>

#include "utils.hpp"
#include "base_common.hpp"

namespace Base {
    class Reader {
        public:

            const int DEFINITION_READING_BIT = 0;
            const int FIELDS_READING_BIT = 1;
            bool terminate = false;
            std::string definition_path;
            std::string base_data_path;
            unsigned int stream_controller;
            unsigned int record_field_offset;


            Info load();
            void process_line(std::string line);
            void read_field(std::string field_line);
            Info current_info;
            void mutate_stream(std::string markup_name, bool terminating) {
                int nth_bit = 0;
                if(markup_name == "DEFINITION"){
                    nth_bit = DEFINITION_READING_BIT;
                }
                else if(markup_name == "FIELDS") {
                    nth_bit = FIELDS_READING_BIT;
                }

                if(terminating) {
                    if(nth_bit == DEFINITION_READING_BIT) {
                        this->terminate = true;
                    }
                    CLR_BIT(this->stream_controller, nth_bit);
                }
                else {
                    SET_BIT(this->stream_controller, nth_bit);
                }
                return;
            }
            Reader() {
                this->stream_controller = 0;
            }
            Reader(std::string definition_path) {
                this->stream_controller = 0;
                this->definition_path = definition_path;
                auto last_dot = definition_path.find_last_of('.');
                this->base_data_path = definition_path.replace(definition_path.begin() + last_dot, definition_path.end(), "popdb");
            }
    };



}



#endif