//
// Created by pnsv0 on 13.04.2022.
//

#include "../include/seql_reader.hpp"
#include <fstream>
#include <iostream>
#include "utils.hpp"

void SEQLReader::loadFile(std::string file_dir) {
    debug_log("Loading file at " + file_dir);
    std::fstream file;
    file.open(file_dir, std::fstream::in);

    std::string buffer;
    while(getline(file, buffer)) {
        std::cout << buffer << std::endl;
    }

}
