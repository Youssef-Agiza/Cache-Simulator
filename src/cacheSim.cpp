#include "../headers/PCH.h"
#include "../headers/SetAssociativeCache.h"
#include "../headers/Random.h"

const char *msg[2] = {"Miss", "Hit"};

#define NO_OF_Iterations 10 // CHange to 1,000,000
int main()
{
    Random randomGen1;
    unsigned int numberOfWays;
    unsigned int lineSize;
    //HandleInput(numberOfWays, lineSize);
    unsigned int hit = 0;
    cacheResType r;

    unsigned int address;
    std::cout << "Cache Simulator\n";

    for (int inst = 0; inst < NO_OF_Iterations; inst++)
    {
        address = randomGen1.memGenB();
        r = MISS;
        if (r == HIT)
            hit++;
        std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex << address << " (" << msg[r] << ")\n";
    }
    std::cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << std::endl;
}