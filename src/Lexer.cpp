//
// Created by pnsv0 on 09.04.2022.
//

#include "../include/Lexer.hpp"

using namespace SEQL;
std::vector<Fragment> Lexer::tokenize(const std::string& line){
    std::string fragment_buffer;
    std::vector<Fragment> fragments;
    bool reading_string_literal = false;

    for (size_t i = 0; i < line.size(); i++){
        if(line[i] == '\"') reading_string_literal = !reading_string_literal;
        else if(line[i] == ' ' && !reading_string_literal) {
            if(fragment_buffer.empty() || fragment_buffer == " ") continue;
            Fragment fragment = Fragment(fragment_buffer);
            fragment_buffer = "";
            fragments.push_back(fragment);
        }
        else {
            fragment_buffer += line[i];
        }
    }


    auto fragment = Fragment(fragment_buffer);
    fragments.push_back(fragment);

    return fragments;
}