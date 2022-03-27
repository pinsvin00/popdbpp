
#include <iostream>
#include <vector>

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
        std::string definition_path;
        std::string data_path;
        std::vector<Field> fields;
        std::vector<Field> options;
        
        bool is_busy;
        int record_size;
        int definition_end_byte;
        unsigned long long int record_count;

        Info(std::string name, std::string definition_path) {
            this->name = name;
            this->definition_path = definition_path;
        }
        Info() = default;

        void load();
    };


    class RecordDefinition {
        public:
        std::vector<RecordField> record_fields;
        size_t size;
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

    enum EventType {
        InvokeFunction,
    };

    enum NativeFunctionCode {
        INSERT_BEGIN,
        INSERT,
        SELECT,
        POP,
        POP_NTH,
        DELETE_NTH,
    };

    class Event{
        public:
        std::vector<std::string> arguments;
        EventType type;
    };

    class FunctionDispatchEvent : public Event {
        public:
        NativeFunctionCode code;
    };

}