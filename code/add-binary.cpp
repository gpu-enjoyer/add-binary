
#include "Number.h"

int main()
{
    std::filesystem::path 
    p("/home/user/Desktop/add-binary/input.txt");

    Number A(p, 0), B(p, 1), C(A + B);

    OUT_NUM(A);
    OUT_NUM(B);
    OUT_NUM(C);

    return 0;
}
