#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays, unsigned int blockSize)
    : m_NumberOfWays(numberOfWays), m_BlockSize(blockSize)
{
    Initialize();
}

SetAssociativeCache::~SetAssociativeCache()
{
    delete[] m_Sets;
}

cacheResType SetAssociativeCache::TestCache(unsigned int address)
{
    unsigned int setNumber = address % m_NumberOfWays;
    if (m_Sets[setNumber].IsInSet(address))
        return HIT;
    else
    {
        m_Sets[setNumber].ReplaceLine(address);
        return MISS;
    }
}

void SetAssociativeCache::Initialize()
{
    unsigned int numOfLines = CACHE_SIZE / m_BlockSize;                 // gives out the number of lines
    unsigned int numOfTagBits = 32 - (unsigned int)(log2(m_BlockSize)); // 32 bit address - the number of bits we have for the words
    m_Sets = new Set[m_NumberOfWays];
    for (int i = 0; i < m_NumberOfWays; i++)
        m_Sets[i].Initialize(m_BlockSize, numOfLines, numOfTagBits);
}
