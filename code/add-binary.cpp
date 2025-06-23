
#include <fstream>
#include <filesystem>
#include <iostream>

struct Number
{
    uint8_t  length;
    uint8_t* byte_arr;

    Number() : length(0), byte_arr(nullptr) {};

    // todo 
    // Number(const Number& N);

    Number(std::filesystem::path inputPath, uint8_t inputRaw)
    {
        if (!std::filesystem::exists(inputPath))
        {
            throw std::runtime_error
                ("error: inputPath " + inputPath.string() + " does not exist \n");
        }
        std::fstream input(inputPath);

        // todo 
        // ...

    }
    
    // todo 
    // ~Number();

    Number& operator=(const Number& N)
    {
        length = N.length;

        if (byte_arr != nullptr) delete[] byte_arr;
        byte_arr = new uint8_t[length];

        for (uint8_t i = 0; i < length; ++i)
        {
            *(byte_arr + i) = *(N.byte_arr + i);
        }

        return std::cout << "operation \"=\" done \n", *this;
    }

};

int main()
{
    // todo 
    // Number A("../input.txt", 1);
    // Number B("../input.txt", 2);

    Number A;
    Number B;

    A = B;

    return 0;
}
