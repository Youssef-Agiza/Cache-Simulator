#include "../headers/Utilities.h"
#include <iomanip>
#include <array>
void TestBench()
{
    int answer;
    std::cout << "Replacement policy:\n1-random\n2-LFU\n3-LRU\n";
    std::cin >> answer;
    ReplacmentPolicy p;
    switch (answer)
    {
    case 0:
        p = ReplacmentPolicy::Random;
        break;
    case 1:
        p = ReplacmentPolicy::LFU;
        break;
    case 2:
        p = ReplacmentPolicy::LRU;
        break;
    }
    SetAssociativeCache cache(2u, 32u, p);
    uint32_t addresses[] = {0x070, 0x080, 0x068, 0x190, 0x084, 0x178, 0x08C, 0xF00, 0x064};
    uint32_t arrSize = sizeof(addresses) / sizeof(uint32_t);
    uint32_t hitNumber = 0;
    double hitRatio;
    int times = 0;
there:
    for (uint32_t i = 0; i < arrSize; i++)
    {
        std::cout << "+---------------------+\n NEW TEST CACHE\n";
        if (cache.TestCache(addresses[i]) == HIT)
        {
            std::cout << "The address: 0x" << std::hex << addresses[i] << " performs HIT\n";
            hitNumber++;
        }
        else
            std::cout << "The address: 0x" << std::hex << addresses[i] << " performs MISS\n";
    }
    times++;
    hitRatio = (100.0 * hitNumber) / (arrSize);
    std::cout << "Hit ratio is " << hitRatio << std::endl;
    hitNumber = 0;
    if (times == 1)
    {
        std::cout << "\n +-------------+\nGoing There\n";
        goto there;
    }
    std::cout << "End of the simulation\n";
}

int main()
{
    //HandleInput();
    TestBench();
    return 0;
}