
#include "Number.h"

int main()
{
    std::filesystem::path p("../input.txt");

    Number A(p, 0);
    Number B(p, 1);
    Number C(A + B);

    outputABC(A, B, C);

    // OUT_NUM(N);

    return 0;
}
