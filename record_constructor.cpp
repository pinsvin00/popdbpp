#include "record_constructor.hpp"

Base::Record Base::RecordConstructor::construct(std::vector<SEQL::Fragment> arguments) {
    std::vector<Base::RecordField> fields = {};//{definition.record_fields};
    Base::Record record;
    std::vector<char> record_byte_arr(512);

    int record_pointer = 0;
    for (size_t i = 0; i < arguments.size(); i++){
        SEQL::Fragment element = arguments[i];
        Base::RecordField rf = fields[i];

        arguments.erase(arguments.begin());

        std::vector<char> bytes(element.value.begin(), element.value.end());

        int to_full = rf.size - bytes.size(); 
        if(to_full < 0 ) {
            //todo proper error handling
            std::cout << "ERROR too long record" << std::endl;
        }

        for (size_t i = 0; i < bytes.size(); i++){
            record_byte_arr[record_pointer] = bytes[i];
            record_pointer++;
        }
        for (size_t i = 0; i < to_full; i++){
            record_byte_arr[record_pointer] = 0;
            record_pointer++;
        }
        
    }


    record.data = record_byte_arr;
    //record.definition = definition;

    return record;
}