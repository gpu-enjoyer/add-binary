
#include "Number.h"


#pragma region Get_Numbers

Number::Number(
    const std::filesystem::path& inputPath, 
    const uint16_t lineNum)
{
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
}

void Number::checkPath(
    const std::filesystem::path& inputPath)
{ 
    if (!std::filesystem::exists(inputPath))
        throw std::runtime_error
            (WHERE + "input file does not exist: " + inputPath.string());
    LOC();
    OUT_STR(inputPath.string());
}

std::streampos Number::moveToLine(
    std::ifstream& inputFile,
    const uint16_t lineNum)
{
    if (lineNum == 0) return 0;

    inputFile.seekg(0);
    int ch(0);

    for (uint16_t i = 0; i < lineNum; ++i)
    {
        ch = inputFile.get();

        while (ch != '\n')
        {
            if (ch == EOF)
                throw std::runtime_error
                    (WHERE + "unexpected input EOF");

            ch = inputFile.get();
        }
    }

    return inputFile.tellg();
}

void Number::initFields(
    std::ifstream& inputFile,
    const std::streampos& startPos)
{
    inputFile.clear();
    inputFile.seekg(startPos);

    LOC();
    OUT_INT(inputFile.tellg());

    uint64_t  length        = 0;
    uint64_t  inputZeros    = 0;
    bool      zerosChecked  = false;
    int       ch            = inputFile.get();
    word_t    bit           = 1;

    while (ch != '\n' && ch != EOF)
    {
        LOC();
        OUT_INT(ch - '0');

        ++length;

        if (ch != '0' && ch != '1')
            throw std::runtime_error
                (WHERE + "input.txt is not binary");

        if (!zerosChecked)
        {
            if (ch == '0') ++inputZeros;
            else zerosChecked = true;
        }

        ch = inputFile.get();
    }

    if (length == 0)
        throw std::runtime_error
            (WHERE + "empty input.txt");

    if (length == inputZeros)
    {
        arrSize = 1;
        arr = new word_t[1]{ 0 };
        zeros = WORD_BITS - 1;
        return;
    }

    arrSize = (length - inputZeros - 1) / WORD_BITS + 1;
    arr = new word_t[arrSize]();
    zeros = WORD_BITS * arrSize - (length - inputZeros);

    if (zeros >= WORD_BITS)
        throw std::logic_error
            (WHERE + "zeros >= WORD_BITS");

    inputFile.clear();
    inputFile.seekg(startPos + std::streamoff(inputZeros));
    bit = 1;

    for (int16_t i = WORD_BITS - zeros - 1; i >= 0; --i)
    {
        ch = inputFile.get();
        bit = static_cast<word_t>(ch - '0') << i;
        arr[0] |= bit;
    }

    bit = 1;

    for (word_t* w = arr + 1; w < arr + arrSize; ++w)
    {
        *w = 0;

        for (int16_t i = WORD_BITS - 1; i >= 0; --i)
        {
            ch = inputFile.get();
            bit = (word_t)(ch - '0') << i;
            *w |= bit;
        }
    }
}

#pragma endregion


#pragma region Sum_Numbers

Number Number::operator+(
    const Number& N)
{
    Number         Sum;
    word_t*        pSumArr;

    const Number*  N1;
    const Number*  N2;

    const word_t*  pN1Arr;
    const word_t*  pN2Arr;

    uint8_t        carry(0);
    word_t         bit(1);

    if (arrSize >= N.arrSize)
    {
        Sum = Number(arrSize);
        N1 = this;
        N2 = &N;
        pN1Arr = arr + arrSize - 1;
        pN2Arr = N.arr + N.arrSize - 1;
    }
    else
    {
        Sum = Number(N.arrSize);
        N1 = &N;
        N2 = this;
        pN1Arr = N.arr + N.arrSize - 1;
        pN2Arr = arr + arrSize - 1;
    }

    pSumArr = Sum.arr + Sum.arrSize - 1;

    while (pN2Arr >= N2->arr)
    {
        for (int16_t i = 0; i < WORD_BITS; ++i)
        {
            if (currentBit(carry, getBit(pN1Arr, bit), getBit(pN2Arr, bit)))
            {
                *pSumArr |= bit;
            }
            carry = carryBit(carry, getBit(pN1Arr, bit), getBit(pN2Arr, bit));

            bit <<= 1;
        }

        --pN2Arr;

        --pN1Arr;
        --pSumArr;
        bit = 1;
    }

    while (pN1Arr >= N1->arr)
    {
        for (int16_t i = 0; i < WORD_BITS; ++i)
        {
            if (currentBit(carry, getBit(pN1Arr, bit), 0))
            {
                *pSumArr |= bit;
            }
            carry = carryBit(carry, getBit(pN1Arr, bit), 0);
            bit <<= 1;
        }

        --pN1Arr;
        --pSumArr;
        bit = 1;
    }

    //* think about how to do it better: 

    if (carry == 1)
    {
        LOG("omg! the last carry bit == 1");

        word_t* pSumNewArr = new word_t[arrSize + 1]();

        pSumNewArr[0] = 1;
        memcpy(pSumNewArr + 1, Sum.arr, Sum.arrSize * (WORD_BITS / 8));

        delete[] Sum.arr;
        Sum.arr = pSumNewArr;
        pSumNewArr = nullptr;

        ++Sum.arrSize;
        Sum.zeros = WORD_BITS - 1;
    }
    else if (Sum.arrSize == 1 && Sum.arr[0] == 0) 
    {
        Sum.zeros = WORD_BITS - 1;
    }
    else
    {
        Sum.zeros = 0;

        bool zerosChecked  = false;
        word_t bitMask = static_cast<word_t>(1) << (WORD_BITS - 1);

        while (!zerosChecked && bitMask != 0)
        {
            if (*Sum.arr & bitMask) zerosChecked = true;
            else ++Sum.zeros;

            bitMask >>= 1;
        }
    }

    LOC();
    OUT_INT(Sum.zeros);
    
    return Sum;
}

Number::Number(
    const uint64_t& arrSize)
{
    this->arrSize = arrSize;
    arr = new word_t[arrSize]();
    zeros = 8; // tmp unique value
}

#pragma endregion


#pragma region Special_Member_Functions

// arrSize > 0
// zeros in [0..7]

Number::Number()
{
    arrSize = 1;
    zeros = WORD_BITS - 1;
    arr = new word_t[1]();
}

Number::~Number()
{
    delete[] arr;
}

Number::Number(const Number& N)
{
    arrSize = N.arrSize;
    zeros = N.zeros;
    arr = new word_t[arrSize];

    word_t*        pArr   = arr; 
    const word_t*  pNArr  = N.arr;

    while (pArr < arr + arrSize)
    {
        *pArr = *pNArr;
        ++pArr;
        ++pNArr;
    }
}

Number::Number(Number&& N)
{
    arrSize = N.arrSize;
    zeros = N.zeros;
    arr = N.arr;
    N.arr = nullptr;
}

Number& Number::operator=(const Number& N)
{
    if (this == &N) return *this;

    arrSize = N.arrSize;
    zeros = N.zeros;

    delete[] arr;
    arr = new word_t[arrSize]();

    word_t*        pArr   = arr; 
    const word_t*  pNArr  = N.arr;

    while (pArr < arr + arrSize)
    {
        *pArr = *pNArr;
        ++pArr;
        ++pNArr;
    }

    return *this;
}

Number& Number::operator=(Number&& N)
{
    arrSize = N.arrSize;
    zeros = N.zeros;
    delete[] arr;
    arr = N.arr;
    N.arr = nullptr;
    return *this;
}

#pragma endregion


#pragma region Other

std::ostream& operator<<(
    std::ostream& os, 
    const Number& N)
{
    word_t bit(1);

    for (word_t* w = N.arr; w < N.arr + N.arrSize; ++w) 
    {
        bit = (word_t)1 << (WORD_BITS - 1);

        for (int16_t i = 0; i < WORD_BITS; ++i)
        {
            os << ((*w & bit) ? '1' : '0');
            bit >>= 1;
        }

        os << ' ';
    }

    return os;
}

void printABC(
    const Number& A,
    const Number& B,
    const Number& C)
{
    const uint64_t* maxArrSize =
    (
        A.arrSize >= B.arrSize
        ? (
            A.arrSize >= C.arrSize
            ? &(A.arrSize)
            : &(C.arrSize)
        )
        : (
            B.arrSize >= C.arrSize
            ? &(B.arrSize)
            : &(C.arrSize)
        )
    );

    std::ostringstream os;

    os << '\n';
    os << "\033[93m";

    os << " A = ";

    for (uint64_t i = 0; i < *maxArrSize - A.arrSize; ++i)
    {
        for(uint16_t j = 0; j < WORD_BITS; ++j)
        {
            os << ' ';
        }
        os << ' ';
    }

    os << A << '\n';
    std::cout << os.str();
    os.str("");
    os.clear();

    os << " B = ";
    
    for (uint64_t i = 0; i < *maxArrSize - B.arrSize; ++i)
    {
        for(uint16_t j = 0; j < WORD_BITS; ++j)
        {
            os << ' ';
        }
        os << ' ';
    }

    os << B << '\n';
    std::cout << os.str();
    os.str("");
    os.clear();

    os << " C = ";
    os << C;

    os << "\033[0m";
    os << "\n\n";

    std::cout << os.str();
}

void Number::out(
    const std::filesystem::path& outputPath)
{
    std::filesystem::path  finalPath;
    std::ofstream          outputFile;

    if (std::filesystem::is_directory(outputPath))
    {
        finalPath = outputPath / "output.txt";
    }
    else if (outputPath.extension() == ".txt")
    {
        finalPath = outputPath;
    }
    else
    {
        throw std::runtime_error
            (WHERE + "outputPath neither .txt nor dir");
    }

    LOC();
    OUT_STR(finalPath);

    outputFile.open(finalPath);
    word_t bitMask = static_cast<word_t>(1) << (WORD_BITS - 1 - zeros); 
    
    for (word_t* pArr = arr; pArr < arr + arrSize; ++pArr)
    {
        while (bitMask != 0)
        {
            outputFile << (*pArr & bitMask ? '1' : '0');
            bitMask >>= 1;
        }

        bitMask = static_cast<word_t>(1) << (WORD_BITS - 1);
    }

    outputFile.close();

    LOG("done");
}

#pragma endregion
