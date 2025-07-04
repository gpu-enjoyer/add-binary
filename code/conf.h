
#pragma once

#include <filesystem>
#include <iostream> // -> <string>
#include <fstream>

#define BYTE_WORD

#if defined(BYTE_WORD)
    using word_t = uint8_t;
    #define WORD_BITS (int16_t)8
#elif UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF  // 16^16 == 2^64
    using word_t = uint64_t;
    #define WORD_BITS (int16_t)64
#else
    using word_t = uint32_t;
    #define WORD_BITS (int16_t)32
#endif

#define  BOLD_  std::string("\033[1m")
#define  DIM_   std::string("\033[90m")
#define  Y_     std::string("\033[93m")
#define  _END   std::string("\033[0m")

#define LINE (BOLD_ + std::to_string(__LINE__) + ": " + _END)
#define FUNC (DIM_ + std::string(__func__) + "(): " + _END)

#define LOC_STR ("\n" + LINE + FUNC)

#define LOC() { std::cout << LOC_STR << "\n\n"; }

#define LOG(s) { std::cout << LOC_STR << s << "\n"; }

#define OUT(expr) { out_impl<uint64_t>(expr, #expr); }
#define OUT_TEXT(expr) { out_impl<std::string>(expr, #expr); }
#define OUT_NUM(num) { std::cout << '\n'; out_impl<Number>(num, #num); }

template <typename T>
inline void out_impl(
    const T& expr, 
    const char* exprName)
{
    std::cout << exprName << " = " << expr << '\n';
    return;
}
