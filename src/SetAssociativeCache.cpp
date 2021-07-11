#include "../headers/SetAssociativeCache.h"

SetAssociativeCache::SetAssociativeCache(unsigned int numberOfWays, unsigned int lineSize)
    : m_NumberOfWays(numberOfWays), m_LineSize(lineSize)
{
    m_NumberOfSets = (unsigned int)(CACHE_SIZE) / (m_LineSize * m_NumberOfWays);
    m_NumOfTagBits = 32 - (unsigned int)(log2(m_LineSize) + log2(m_NumberOfSets)); // 32 bit address - the number of bits we have for the words
    InitalizeSets(m_LineSize);
}

SetAssociativeCache::~SetAssociativeCache()
{
    delete[] m_Sets;
}

bool SetAssociativeCache::IsInSet(unsigned int address)
{
    unsigned int setIndex = address % m_NumberOfWays;
    typename SetAssociativeCache::Set &set = m_Sets[setIndex];
    unsigned int tag = address >> (unsigned int)(log2(m_LineSize) + log2(m_NumberOfSets));

    for (uint32_t i = 0; i < m_NumberOfWays; i++)
        if (set.validBits[i] == VALID && set.tags[i] == tag)
            return true;

    return false;
}

cacheResType SetAssociativeCache::TestCache(unsigned int address)
{
    unsigned int setNumber = address % m_NumberOfWays;
    // if (m_Sets[setNumber].IsInSet(address))
    // return HIT;
    // else
    {
        // m_Sets[setNumber].ReplaceLine(address);
        return MISS;
    }
}

void SetAssociativeCache::ReplaceBlock(unsigned int address)
{
    unsigned int setIndex = address % m_NumberOfWays;
    unsigned int tag = address >> (32 - m_NumOfTagBits);
    typename SetAssociativeCache::Set &set = m_Sets[setIndex];

    set.tags[set.nextReplacementIndex] = tag;
    set.validBits[set.nextReplacementIndex] = VALID;
    set.nextReplacementIndex = (set.nextReplacementIndex + 1) % m_NumberOfWays;
}

void SetAssociativeCache::InitalizeSets(unsigned int lineSize)
{

    m_Sets = new typename SetAssociativeCache::Set[m_NumberOfSets];

    for (uint32_t i = 0; i < m_NumberOfWays; i++)
    {
        m_Sets[i].nextReplacementIndex = 0;

        m_Sets[i].tags = new unsigned int[m_NumberOfWays]; // in words
        memset(m_Sets[i].tags, 0, m_NumberOfWays);

        m_Sets[i].validBits = new unsigned short[m_NumberOfWays]; // in words
        memset(m_Sets[i].validBits, 0, m_NumberOfWays);           //All are invalid
    }
}