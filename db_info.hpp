#ifndef DB_INFO_H
#define DB_INFO_H

#include <vector>
#include "utils.hpp"
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>


namespace Base {
    class Field {
    public:
        std::string raw_field;
        std::string name;
        std::vector<std::string> parameters;
        Field(std::string field_raw);
    };

    class Info {
    public:
        std::string name;
        std::string path;
        std::vector<Field> fields;
        std::vector<Field> options;
        bool is_busy;
        int record_size;
        int definition_end_byte;
        unsigned long long int record_count;

        Info(std::string name, std::string path) {
            this->name = name;
            this->path = path;
        }
        Info() = default;

        void load();
    };



    class RecordField : public Field {
    public:
        unsigned int size;
        unsigned int nth;
        unsigned int offset;
        bool can_be_null = true;

        std::string data_type;

        RecordField(std::string field_raw) : Field(field_raw) {

            if(this->parameters.size() < 2) {
                throw std::invalid_argument("Record field requires having at least 2 parameters");
            }

            try {
                this->size = std::stoi(parameters[0]);
                this->data_type = this->parameters[1];
            }
            catch(const std::invalid_argument & e) {
                std::cout << e.what() << std::endl;
            }

            if(this->parameters.size() > 3) {
                this->can_be_null = (this->parameters[3] == "NULLABLE");
            }

        }
    };

    class Reader {
        public:

            const int DEFINITION_READING_BIT = 0;
            const int FIELDS_READING_BIT = 1;

            std::string path;
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
    };



}



#endif