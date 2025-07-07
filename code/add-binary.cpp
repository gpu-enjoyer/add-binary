
#include "Number.h"

int main()
{
    std::filesystem::path inputPath("../input.txt");

    Number A(inputPath, 0);
    Number B(inputPath, 1);

    Number C(A + B);

    printABC(A, B, C);

    std::filesystem::path outputPath("../output.txt");

    C.out(outputPath);

    return 0;
}
