
#include "Number.h"

// rvalue link && catch temporary object Number&& N.
// gives the oppotunity to get or modify tmp obj
Number::Number(Number&& N)
{
    arrSize = N.arrSize;
    zeros = N.zeros;
    arr = N.arr;
    N.arr = nullptr;
}

//! 
Number Number::operator+(const Number& N)
{
    Number Sum;

    // todo ...

    return Sum;
}


Number::Number(
    const std::filesystem::path inputPath, 
    const uint16_t lineNum)
{
    LOG("begin");

    std::ifstream inputFile;
    std::streampos startPos(0);

    try
    {
        checkPath(inputPath);
        inputFile.open(inputPath);
        startPos = moveToLine(inputFile, lineNum);
        initFields(inputFile, startPos);
        inputFile.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << '\n';
    }

    LOG("end");
}

void Number::checkPath(
    const std::filesystem::path& inputPath)
{
    LOG("begin \n");
    
    if (!std::filesystem::exists(inputPath)) 
    throw std::runtime_error
    (LOC_STR + "input file does not exist: " 
        + inputPath.string());

    OUT_TEXT(canonical(inputPath).string());
    
    LOG("end");
}

std::streampos Number::moveToLine(
    std::ifstream& inputFile,
    const uint16_t lineNum)
{
    LOG("begin \n");

    if (lineNum == 0)
    {
        LOG("lineNum == 0");
        LOG("end");
        return 0;
    }

    inputFile.seekg(0);
    OUT(inputFile.tellg());

    int ch(inputFile.get()); // get [ 0 ]

    for (uint16_t i = 0; i < lineNum; ++i)
    {
        while (ch != '\n')
        {
            if (ch == EOF)
            throw std::runtime_error
            (LOC_STR + "unexpected input EOF");

            ch = inputFile.get();
            // ch = get [ 1 ]
            // ...
            // ch = get [ \n ]
        }
    }

    // inputFile.tellg() == [ begin of new line ]
    // int ch            == '\n'

    OUT(inputFile.tellg());
    LOG("end");

    return inputFile.tellg();
}

//!
void Number::initFields(
    std::ifstream& inputFile,
    const std::streampos& startPos)
{
    LOG("begin \n");
    OUT(startPos);
    
    uint64_t  length        = 0;
    uint64_t  inputZeros    = 0;
    bool      zerosChecked  = false;
    int       ch            = inputFile.get();
    word_t    bit           = 1;

    LOC();
    OUT((int)'0');
    OUT((int)'1');

    LOC();

    while (ch != '\n' && ch != EOF)
    {
        OUT(ch - '0');

        ++length;

        if (ch != '0' && ch != '1')
        {
            throw std::runtime_error
            (LOC_STR + "input file is not binary");
        }

        if (!zerosChecked)
        {
            if (ch == '0') ++inputZeros;
            else zerosChecked = true;
        }

        ch = inputFile.get();
    }

    LOC();
    OUT(inputZeros);
    OUT(length);

    if (length == 0)
    throw std::runtime_error
    (LOC_STR + "empty string in input file");

    if (length == inputZeros)
    {
        LOG("length == inputZeros");

        arrSize = 1;
        arr = new word_t[1]();
        zeros = WORD_BITS - 1;
    }
    else
    {
        LOC();

        arrSize = (length - inputZeros - 1) / WORD_BITS + 1;
        OUT(arrSize);

        arr = new word_t[arrSize]();
        OUT(arr[0]);

        zeros = WORD_BITS * arrSize - (length - inputZeros);
        OUT(zeros);

        if (zeros >= WORD_BITS)
        std::logic_error
        (LOC_STR + "zeros >= WORD_BITS");

        inputFile.seekg(startPos + std::streamoff(inputZeros));
        OUT(inputFile.tellg());

        bit = 1;
        arr[0] = 0;

        LOC();
            std::cout << "    ";
        OUT(*arr);
            std::cout << '\n';

        for (int16_t i = WORD_BITS - zeros - 1; i >= 0; --i)
        {
            ch = inputFile.get();
            bit = (word_t)(ch - '0') << i;
            arr[0] |= bit;

            OUT(ch - '0');
                std::cout << "     ";
            OUT(bit);
                std::cout << "       ";
            OUT(i);
                std::cout << '\n';
        }

        bit = 1;

        for (word_t* w = arr + 1; w < arr + arrSize; ++w)
        {
            *w = 0;

            LOC(); 
                std::cout << ' ';
            OUT(w - arr);
                std::cout << '\n';

            for (int16_t i = WORD_BITS - 1; i >= 0; --i)
            {
                ch = inputFile.get();
                bit = (word_t)(ch - '0') << i;
                *w |= bit;

                OUT(ch - '0');
                    std::cout << "     ";
                OUT(bit);
                    std::cout << "       ";
                OUT(i);
                    std::cout << '\n';
            }
        }
    }

    LOG("end");
}

Number& Number::operator=(
    const Number& N)
{
    arrSize = N.arrSize;
    zeros = N.zeros;
    delete[] arr;
    arr = new word_t[arrSize]();
    for (uint64_t i = 0; i < arrSize; ++i) 
        { *(arr + i) = *(N.arr + i); }
    return *this;
}

std::ostream& operator<<(
    std::ostream& os, 
    const Number& N)
{
    word_t bit(1);

    os << Y_;

    if (N.arrSize == 0)
    {
        return os << "empty" << _END;
    }

    for (word_t* w = N.arr; w < N.arr + N.arrSize; ++w) 
    {
        bit = (word_t)1 << (WORD_BITS - 1);

        for (int16_t i = 0; i < WORD_BITS; ++i)
        {
            os << ((*w & bit) ? '1' : '0');
            bit >>= 1;
        }

        (WORD_BITS == 8) ? (os << ' ') : (os << "\n    ");
    }

    return os << _END;
}
