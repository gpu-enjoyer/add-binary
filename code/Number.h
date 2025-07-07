
#pragma once

#include "conf.h"

using    word_t = uint8_t;
#define  WORD_BITS 8


struct Number
{
    uint64_t arrSize;
    uint16_t zeros;
    word_t*  arr;

    Number(
        const std::filesystem::path& inputPath, 
        const uint16_t lineNum);
    //
        // check if input.txt exists
        void checkPath(
            const std::filesystem::path& inputPath);

        // move next line "lineNum" times
        std::streampos moveToLine(
            std::ifstream& inputFile,
            const uint16_t lineNum);

        // init fields, fill arr, check binarity
        void initFields(
            std::ifstream& inputFile,
            const std::streampos& startPos);
    //

    Number operator+(
        const Number& N);
    //
        // fill arr[ arrSize ] with zeros
        Number(
            const uint64_t& arrSize);

        inline uint8_t getBit
            (const word_t* w, const word_t& bitMask)
                { return (*w & bitMask ? 1 : 0); }

        inline uint8_t currentBit
            (const uint8_t& a, const uint8_t& b, const uint8_t& c)
                { return (a ^ b) ^ c; }

        inline uint8_t carryBit
            (const uint8_t& a, const uint8_t& b, const uint8_t& c)
                { return (a & b) | (b & c) | (a & c); }
    //

    // output.txt << (N)
    void out(
        const std::filesystem::path& outputPath);

    Number();
    ~Number();

    Number(const Number& N);
    Number(Number&& N);
    Number& operator=(const Number& N);
    Number& operator=(Number&& N);
};


// std::cout << (N)
std::ostream& operator<<(
    std::ostream& os, 
    const Number& N);

// std::cout << (A + B = C)
void printABC(
    const Number& A,
    const Number& B,
    const Number& C);
