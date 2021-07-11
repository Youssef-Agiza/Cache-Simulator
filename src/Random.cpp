#include "../headers/Random.h"
Random::Random()
    : m_w(0xABCCAB99), m_z(0xDEAD6902)
{}

unsigned int Random::rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}
unsigned int Random::memGenA()
{
    static unsigned int address1 = 0;
    return (address1++) % (DRAM_SIZE);
}
unsigned int Random::memGenB()
{
    static unsigned int address2 = 0;
    return rand_() % (64 * 1024);
}
unsigned int Random::memGenC()
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

unsigned int Random::memGenD()
{
    return rand_() % (16 * 1024);
}
unsigned int Random::memGenE()
{
    static unsigned int address3 = 0;
    return (address3++) % (1024 * 64);
}
unsigned int Random::memGenF()
{
    static unsigned int address4 = 0;
    return (address4 += 64) % (64 * 4 * 1024);
}