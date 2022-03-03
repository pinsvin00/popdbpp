#include "db_info.hpp"


Base::Field::Field(std::string field_raw) {
    size_t i = 0;
    std::string name = "";

    for (; i < field_raw.size(); i++){
        if(field_raw[i] == ':'){
            i++;
            break;
        } 
        else name += field_raw[i];
    }
    this->name = name;
    
    std::string value = "";
    for (; i < field_raw.size(); i++){
        if(field_raw[i] == ' ') {
            this->parameters.push_back(value);
            value = "";
            continue;
        }
        else value += field_raw[i];
    }
    




}

Base::Info Base::Reader::load() {
    std::cout << "Loading base on path: " << this->path;
    std::fstream fs;
    fs.open(this->path.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
    if(!fs.is_open()) {
        std::cout << "ERR! Failed to open database!" << std::endl;
    }
    std::string buffer;
    while( getline(fs, buffer) ) {
        this->process_line(trim(buffer));
    }

    fs.close();
}

void Base::Reader::process_line(std::string line) {
    bool is_markup = prefix(";;", line);
    bool is_terminating = prefix(";;END", line);

    if(is_markup) {
        std::string markup_name = line.substr(6, line.size() - 2);
        this->mutate_stream(markup_name, is_terminating);
        return;
    }
    else {
        if(IS_BIT_SET( this->stream_controller, this->FIELDS_READING_BIT)) {
            Field f = Field(line);
            this->current_info.options.push_back(f);
        }
        else if (IS_BIT_SET(this->stream_controller, this->DEFINITION_READING_BIT)) {
            RecordField f = RecordField(line);
            f.nth = this->current_info.fields.size();
            this->current_info.fields.push_back(f);
            this->record_field_offset += f.size;
        }
    }
}
