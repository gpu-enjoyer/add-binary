
#pragma once

#include "conf.h"

struct Number
{
    uint64_t arrSize;
    uint16_t zeros;
    word_t*  arr;

    Number() : arrSize(0), zeros(0), arr(nullptr) {};
    ~Number() { delete[] arr; }

    Number(const Number& N);
    Number(Number&& N);       //* done (1)

    Number& operator=(const Number& N);
    Number& operator=(Number&& N);

    Number operator+(const Number& N); // todo (2)

    Number(const std::filesystem::path inputPath, const uint16_t lineNum);

    // check whether input.txt exist
    void checkPath(const std::filesystem::path& inputPath);

    // move next line "lineNum" times, return position offset
    std::streampos moveToLine(std::ifstream& inputFile, const uint16_t lineNum);

    // init fields, fill arr, check binarity
    void initFields(std::ifstream& inputFile, const std::streampos& startPos);
};

std::ostream& operator<<(std::ostream& os, const Number& N);
