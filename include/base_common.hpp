
#ifndef BASE_COMMON_H
#define BASE_COMMON_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "expression_common.hpp"
#include "function_registry.hpp"
namespace Base {

    const std::string CHARFIELD = "CHARFIELD";
    const std::string INT = "INT";
    const std::string DATE = "DATE";
    const std::string BIGINT = "BIGINT";

    class Field {
    public:
        std::string raw_field;
        std::string name;
        std::vector<std::string> parameters;
        Field(std::string field_raw);
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
                this->size = std::stoi(parameters[1]);
                this->data_type = this->parameters[0];
            }
            catch(const std::invalid_argument & e) {
                std::cout << e.what() << std::endl;
            }

            if(this->parameters.size() > 3) {
                this->can_be_null = (this->parameters[3] == "NULLABLE");
            }

        }
    };

    class RecordDefinition {
        public:
        std::vector<RecordField> fields;
        size_t size = 0;
    };


    class Info {
    public:
        std::string name;
        std::string definition_path;
        std::string data_path;
        Base::RecordDefinition record_definition;
        std::vector<Field> options;
        
        bool is_busy;
        int record_size;
        int definition_end_byte;
        unsigned long long int record_count;

        Info(std::string name, std::string definition_path, Base::RecordDefinition record_definition) {
            this->name = name;
            this->definition_path = definition_path;
            this->record_definition = record_definition;
        }
        Info() = default;

        void load();
    };


    

    class Record {
        public:
        //we should move record definition to the shared_ptr, definetaly
        RecordDefinition definition;
        std::vector<char> data;
    };

    


    enum EventType {
        InvokeFunction,
    };

    class Event {
        public:
        std::vector<SEQL::Fragment> arguments;
        EventType type;
        NativeFunctionIdentifier function_identifier;
        Event() = default;
        Event(std::vector<SEQL::Fragment> argument, EventType) {

        }
    };

    class FunctionDispatchEvent : public Event {
        public:
        NativeFunctionIdentifier code;
        FunctionDispatchEvent(std::string function_name, std::vector<SEQL::Fragment> args) {
            std::map<std::string, int> function_name_identifier = {
                {"INSERT_BEGIN", INSERT_BEGIN},
                {"POP", POP},
            };

            this->code = Base::NativeFunctionIdentifier::INSERT_BEGIN;
            this->arguments = args;
            this->type = (Base::EventType) function_name_identifier[function_name];
        }
    };

}

#endif