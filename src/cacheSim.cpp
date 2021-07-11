#include "../headers/PCH.h"
#include "../headers/SetAssociativeCache.h"

#define DBG 1
#define DRAM_SIZE (64 * 1024 * 1024)

/* The following implements a random number generator */
unsigned int m_w = 0xABCCAB99; /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0xDEAD6902; /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}

unsigned int memGenA()
{
    static unsigned int address = 0;
    return (address++) % (DRAM_SIZE);
}

unsigned int memGenB()
{
    static unsigned int address = 0;
    return rand_() % (64 * 1024);
}

unsigned int memGenC()
{
    static unsigned int a1 = 0, a0 = 0;
    a0++;
    if (a0 == 512)
    {
        a1++;
        a0 = 0;
    }
    if (a1 == 128)
        a1 = 0;
    return (a1 + a0 * 128);
}

unsigned int memGenD()
{
    return rand_() % (16 * 1024);
}

unsigned int memGenE()
{
    static unsigned int address = 0;
    return (address++) % (1024 * 64);
}

unsigned int memGenF()
{
    static unsigned int address = 0;
    return (address += 64) % (64 * 4 * 1024);
}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int address)
{
    // This function accepts the memory address for the memory transaction and
    // returns whether it caused a cache miss or a cache hit

    // The current implementation assumes there is no cache; so, every transaction is a miss
    return MISS;
}

cacheResType cacheSimSA(unsigned int address)
{

    return MISS;
}

void HandleInput(unsigned int &numberOfSets, unsigned int &lineSize)
{
    std::string answerS = "";
    while (answerS == "")
    {
        std::cout << "Enter the number of sets\n";
        std::cout << "1- 1 ways\n";
        std::cout << "2- 2 ways\n";
        std::cout << "3- 4 ways\n";
        std::cout << "4- 8 ways\n";
        std::cout << "5- 16 ways\n";
        std::cout << "6- 32 ways\n";
        std::cout << "Enter ONE DIGIT, only ONE digit will be counted\n";
        std::cin >> answerS;
        char answer = answerS[0];
        if (isdigit(answer))
        {
            answer = answer - '0';
        }
        else
        {
            std::cout << "You did not enter a digit\n";
            answerS = "";
            continue;
        }
        switch (answer)
        {
        case 1:
        {
            numberOfSets = 1;
            break;
        }
        case 2:
        {
            numberOfSets = 2;
            break;
        }
        case 3:
        {
            numberOfSets = 4;
            break;
        }
        case 4:
        {
            numberOfSets = 8;
            break;
        }
        case 5:
        {
            numberOfSets = 16;
            break;
        }
        case 6:
        {
            numberOfSets = 32;
            break;
        }
        default:
            answerS = "";
        }
    }

    answerS = "";
    while (answerS == "")
    {
        std::cout << "Enter the line size\n";
        std::cout << "1- 16 bytes\n";
        std::cout << "2- 32 bytes\n";
        std::cout << "3- 64 bytes\n";
        std::cout << "4- 128 bytes\n";
        std::cout << "Enter ONE DIGIT, only ONE digit will be counted\n";
        std::cin >> answerS;
        char answer = answerS[0];
        if (isdigit(answer))
        {
            answer = answer - '0';
        }
        else
        {
            std::cout << "You did not enter a digit\n";
            answerS = "";
            continue;
        }
        switch (answer)
        {
        case 1:
        {
            lineSize = 16;
            break;
        }
        case 2:
        {
            lineSize = 32;
            break;
        }
        case 3:
        {
            lineSize = 64;
            break;
        }
        case 4:
        {
            lineSize = 128;
            break;
        }
        default:
            answerS = "";
        }
    }
}

const char *msg[2] = {"Miss", "Hit"};

#define NO_OF_Iterations 10 // CHange to 1,000,000
int main()
{
    unsigned int numberOfWays;
    unsigned int lineSize;
    HandleInput(numberOfWays, lineSize);
    unsigned int hit = 0;
    cacheResType r;

    unsigned int address;
    std::cout << "Cache Simulator\n";

    for (int inst = 0; inst < NO_OF_Iterations; inst++)
    {
        address = memGenB();
        r = cacheSimDM(address);
        if (r == HIT)
            hit++;
        std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << address << " (" << msg[r] << ")\n";
    }
    std::cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << std::endl;
}