#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#define SET_BIT(BF, N) BF |= 1UL << N;
#define CLR_BIT(BF, N) BF &= ~(1UL << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)


std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);


bool prefix(const std::string &a, const std::string &b );

#endif