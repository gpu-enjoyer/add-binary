
#pragma once

#include <filesystem>
#include <iostream> // <string>
#include <fstream>
#include <cstring>


#define WHERE \
("\033[90m" + std::to_string(__LINE__) + ": " + std::string(__func__) + "(): " + "\033[0m")

#define LOC() { std::cout << WHERE; }

#define LOG(s) { std::cout << WHERE << s << '\n'; }

#define OUT_INT(i) { out_impl<uint64_t>(i, #i); }

#define OUT_STR(str) { out_impl<std::string>(str, #str); }

#define OUT_NUM(num) \
{ std::cout << "\033[93m"; out_impl<Number>(num, #num); std::cout << "\033[0m"; }

template <typename T>
inline void out_impl(const T& expr, const char* exprName)
{ std::cout << exprName << " = " << expr << '\n'; }
