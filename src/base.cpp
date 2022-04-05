#include "../include/base.hpp"


std::fstream Base::Engine::obtain_base_file_ptr() {
    std::fstream fs;
    fs.open(this->base_file_path, std::fstream::in | std::fstream::out | std::fstream::app);
    return fs;
}

std::fstream Base::Engine::obtain_temp_file_ptr() {
    std::fstream fs;
    fs.open(this->temp_file_path, std::fstream::in | std::fstream::out | std::fstream::app);
    return fs;
}


Base::Record Base::Engine::pop_record() {
    std::fstream temp_file = this->obtain_temp_file_ptr();
    std::fstream base_file = this->obtain_base_file_ptr();
    std::string popped_record = "";
    getline(base_file, popped_record);
    
    std::string line = "";
    while(getline(base_file, line)) {
        temp_file << line << std::endl;
    }

    Record record = Record();
    record.data = std::vector<char>(popped_record.begin(), popped_record.end());
    record.definition = this->info->record_definition;
    
    temp_file.close();
    base_file.close();

    this->swap_buffer_file();

}

void Base::Engine::insert_begin(std::shared_ptr<Record> record) {
    std::fstream temp_file = this->obtain_temp_file_ptr();
    std::fstream base_file = this->obtain_base_file_ptr();

    for(const auto& element : record->data) {
        temp_file << (char)element;
    }
    temp_file << std::endl;

    std::string buffer;
    while(getline(base_file, buffer)) {
        temp_file << buffer << std::endl;
    }

    temp_file.close();
    base_file.close();

    this->swap_buffer_file();
}

void Base::Engine::insert(std::shared_ptr<Record> record) {
    std::fstream base_file = this->obtain_base_file_ptr();
    for(const auto& element : record->data) {
        base_file << (char)element;
    }
    
    base_file << std::endl;
    base_file.close();
}